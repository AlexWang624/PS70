int buttonPin = 33;  // Button connected to pin 33
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);  // Button with internal pull-up
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);  // Print button state to Serial Monitor
  delay(200);  // Slight delay to reduce serial output speed
}
