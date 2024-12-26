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
final int cameraBytesPerPixel = 2; // Changed for RGB565 (2 bytes per pixel)
final int cameraPixelCount = cameraWidth * cameraHeight;
final int bytesPerFrame = cameraPixelCount * cameraBytesPerPixel;

PImage myImage;
byte[] frameBuffer = new byte[bytesPerFrame];
int lastUpdate = 0;
boolean shouldRedraw = false;

void setup() {
  size(640, 480);

  // Replace with your serial port
  myPort = new Serial(this, "/dev/cu.usbmodem101", 921600);     // Mac

  // wait for full frame of bytes
  myPort.buffer(bytesPerFrame);

  myImage = createImage(cameraWidth, cameraHeight, RGB); // Changed to RGB

  // Let the Arduino sketch know we're ready to receive data
  myPort.write(1);
}

void draw() {
  // Time out after 1.5 seconds and ask for new data
  if (millis() - lastUpdate > 1500) {
    println("Connection timed out.");
    myPort.clear();
    myPort.write(1);
  }

  if (shouldRedraw) {
    PImage img = myImage.copy();
    img.resize(640, 480);
    image(img, 0, 0);
    shouldRedraw = false;
  }
}

void serialEvent(Serial myPort) {
  lastUpdate = millis();

  // read the received bytes
  myPort.readBytes(frameBuffer);

  // Access raw bytes via byte buffer
  ByteBuffer bb = ByteBuffer.wrap(frameBuffer);

  // Ensure proper endianness of the data for 16-bit values
  bb.order(ByteOrder.LITTLE_ENDIAN); // Assuming little-endian from Arduino (check datasheet)

  int i = 0;

  while (bb.hasRemaining()) {
    // read 16-bit pixel (RGB565)
    short pixelValue = bb.getShort();

    // Extract RGB components from RGB565
    int r = (pixelValue >> 11) & 0x1F;
    int g = (pixelValue >> 5) & 0x3F;
    int b = pixelValue & 0x1F;

    // Convert 5-bit and 6-bit components to 8-bit
    r = (r << 3) | (r >> 2); // Scale 5-bit to 8-bit
    g = (g << 2) | (g >> 4); // Scale 6-bit to 8-bit
    b = (b << 3) | (b >> 2); // Scale 5-bit to 8-bit
    
    // set pixel color in PImage
    myImage.pixels[i++] = color(r, g, b);
  }

  myImage.updatePixels();

  // Ensures that the new image data is drawn in the next draw loop
  shouldRedraw = true;

  // Let the Arduino sketch know we received all pixels
  // and are ready for the next frame
  myPort.write(1);
}
