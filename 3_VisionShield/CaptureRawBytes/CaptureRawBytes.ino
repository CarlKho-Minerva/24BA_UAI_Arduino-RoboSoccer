#include "camera.h"

/*-----Uncomment the library and class for your specific hardware-----*/

// #include "himax.h"  // API to read from the Himax camera found on the Portenta Vision Shield Rev.1
// HM01B0 himax;

#include "hm0360.h" // API to read from the Himax camera found on the Portenta Vision Shield Rev.2
HM0360 himax;

Camera cam(himax);
#define IMAGE_MODE CAMERA_GRAYSCALE
FrameBuffer fb(320, 240, 2);

unsigned long lastUpdate = 0;

void setup()
{
  Serial.begin(921600);
  // Init the cam QVGA, 30FPS
  cam.begin(CAMERA_R320x240, IMAGE_MODE, 30);
}

void loop()
{
  if (!Serial)
  {
    Serial.begin(921600);
    while (!Serial)
      ;
  }

  // Clear any stale data
  while (Serial.available())
  {
    Serial.read();
  }

  bool timeoutDetected = millis() - lastUpdate > 150;

  // Read serial input and store it
  int serialValue = Serial.read();

  // Only proceed if we get a valid acknowledge or timeout
  if (!timeoutDetected && serialValue != 1)
  {
    return;
  }

  lastUpdate = millis();

  // Send frame start marker
  Serial.write(0xAA);
  Serial.flush(); // Ensure marker is sent

  // Grab frame and write to serial
  int grabResult = cam.grabFrame(fb, 3000);
  if (grabResult == 0)
  {
    // Send frame data in chunks
    const uint8_t *buffer = fb.getBuffer();
    size_t remaining = cam.frameSize();
    while (remaining > 0)
    {
      size_t chunk = min(64, remaining); // Send max 64 bytes at a time
      Serial.write(buffer, chunk);
      Serial.flush();
      buffer += chunk;
      remaining -= chunk;
    }
    Serial.write(0xFF);
    Serial.flush();
  }
  else
  {
    Serial.write(0xEE);
    Serial.write(0xFF);
    Serial.flush();
  }
}