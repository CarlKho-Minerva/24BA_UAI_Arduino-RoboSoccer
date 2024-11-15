// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize digital pin LED_BUILTIN as an output.
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
