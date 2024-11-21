/*
  This sketch reads a raw Stream of RGB565 pixels
  from the Serial port and displays the frame on
  the window.
  Use with the Examples -> CameraCaptureRawBytes Arduino sketch.
  This example code is in the public domain.
*/

import processing.serial.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

Serial myPort;

// must match resolution used in the sketch
final int cameraWidth = 320;
final int cameraHeight = 240;
final int cameraBytesPerPixel = 1;
final int cameraPixelCount = cameraWidth * cameraHeight;
final int bytesPerFrame = cameraPixelCount * cameraBytesPerPixel;

PImage myImage;
byte[] frameBuffer = new byte[bytesPerFrame];
int lastUpdate = 0;
boolean shouldRedraw = false;
int retryCount = 0;
final int MAX_RETRIES = 3;
final int TIMEOUT_MS = 1000; // Increased timeout
byte[] frameMarkerBuffer = new byte[1];

void setup() {
  size(640, 480);

  // if you have only ONE serial port active
  //myPort = new Serial(this, Serial.list()[0], 921600);          // if you have only ONE serial port active

  // if you know the serial port name
  //myPort = new Serial(this, "COM5", 921600);                    // Windows
  //myPort = new Serial(this, "/dev/ttyACM0", 921600);            // Linux
  myPort = new Serial(this, "/dev/cu.usbmodem1101", 921600);     // Mac

  // Don't buffer the serial port
  myPort.buffer(1);

  myImage = createImage(cameraWidth, cameraHeight, ALPHA);

  // Let the Arduino sketch know we're ready to receive data
  myPort.write(1);
}

void draw() {
  if (millis() - lastUpdate > TIMEOUT_MS) {
    retryCount++;
    if (retryCount > MAX_RETRIES) {
      println("Connection lost - Retrying...");
      myPort.clear();
      myPort.write(1);
      retryCount = 0;
    }
    lastUpdate = millis();
  }

  if(shouldRedraw){
    PImage img = myImage.copy();
    img.resize(640, 480);
    image(img, 0, 0);
    shouldRedraw = false;
  }
}

void serialEvent(Serial myPort) {
  lastUpdate = millis();
  retryCount = 0;

  // Wait for enough data
  int waitStart = millis();
  while (myPort.available() < bytesPerFrame + 2) { // +2 for markers
    if (millis() - waitStart > 500) {
      println("Timeout waiting for data");
      myPort.clear();
      myPort.write(1);
      return;
    }
  }

  // Look for start marker
  int startByte = myPort.read();
  if (startByte != 0xAA) {
    println("Invalid start marker: " + startByte);
    myPort.clear();
    myPort.write(1);
    return;
  }

  // Read exact number of bytes for the frame
  int count = 0;
  while (count < bytesPerFrame) {
    if (myPort.available() > 0) {
      frameBuffer[count++] = (byte)myPort.read();
    }
  }

  // Read end marker
  int endMarker = myPort.read();
  if (endMarker != 0xFF) {
    println("Invalid end marker: " + endMarker);
    myPort.clear();
    myPort.write(1);
    return;
  }

  // Process frame
  ByteBuffer bb = ByteBuffer.wrap(frameBuffer);
  int i = 0;
  while (bb.hasRemaining()) {
    byte pixelValue = bb.get();
    myImage.pixels[i++] = color(Byte.toUnsignedInt(pixelValue));
  }

  myImage.updatePixels();
  shouldRedraw = true;

  // Clear any remaining data
  myPort.clear();
  myPort.write(1);
}
