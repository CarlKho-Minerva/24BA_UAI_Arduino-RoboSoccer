# Documentation for Portenta-H7_RoboSoccer

## Commit History

### Commit 1: Initial Setup and Basic Blink Example

- **Files Modified**: `Blink/Blink.ino`
- **Description**: Added the basic blink example to control the built-in LED on the Portenta H7.
- **Code**:

  ```cpp
  void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
  }

  void loop() {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
  }
  ```

### Commit 2: Dual Core Processing Example

- **Files Modified**: `BlinkBothCores/BlinkBothCores.ino`
- **Description**: Added an example to demonstrate dual-core processing by blinking the RGB LED in different colors using both M7 and M4 cores.
- **Code**:

  ```cpp
  int myLED;

  void setup() {
    randomSeed(analogRead(0));

    #ifdef CORE_CM7
      bootM4();
      myLED = LEDB; // built-in blue LED
    #endif

    #ifdef CORE_CM4
      myLED = LEDG; // built-in green LED
    #endif

    pinMode(myLED, OUTPUT);
  }

  void loop() {
    digitalWrite(myLED, LOW); // turn the LED on
    delay(200);
    digitalWrite(myLED, HIGH); // turn the LED off
    delay(rand() % 2000 + 1000); // wait for a random amount of time between 1 and 3 seconds.
  }
  ```

### Commit 3: Blink Red LED on M7 Core

- **Files Modified**: `BlinkRedLed_M7/BlinkRedLed_M7.ino`
- **Description**: Added an example to blink the red LED on the M7 core of the Portenta H7.
- **Code**:

  ```cpp
  // the setup function runs once when you press reset or power the board
  void setup() {
    bootM4();
    pinMode(LEDR, OUTPUT);
  }

  // the loop function runs over and over again forever
  void loop() {
    digitalWrite(LEDR, HIGH); // turn the LED off (HIGH is the voltage level)
    delay(250); // wait for 200 milliseconds
    digitalWrite(LEDR, LOW); // turn the LED on by making the voltage LOW
    delay(250); // wait for 200 milliseconds
  }
  ```

### Commit 4: Phone Controlled LED
- **Files Modified**: `SimpleWebServer.ino/SimpleWebServer.ino.ino`
- **Description**: Added a web server example to control the red, green, and blue LEDs on the Portenta H7 using a phone.
- **High-Level Flow**:
  - Set up the Wi-Fi access point using the Portenta H7.
  - Initialize the web server to listen on port 80.
  - Create a web page with buttons to control the red, green, and blue LEDs.
  - Handle HTTP GET requests to turn the LEDs on and off based on the button clicks.
  - Update the LED states and provide visual feedback on the web page.


## License

This project is licensed under the [MIT License](LICENSE).
