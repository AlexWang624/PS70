#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();  // Create display object
bool displayOn = true;      // Variable to track whether text should be displayed
int buttonPin = 33;         // Button connected to pin 33
unsigned long lastDebounceTime = 0;  // Last time the button was toggled
unsigned long debounceDelay = 50;    // Debounce delay in milliseconds
int lastButtonState = HIGH;          // Last stable state of the button
int lastPotValue = -1;               // Last read potentiometer value to detect changes

void setup() {
  Serial.begin(9600);
  tft.init();
  tft.invertDisplay(1);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  pinMode(buttonPin, INPUT_PULLUP);
  displayText("Hi");
}

void loop() {
  int readButton = digitalRead(buttonPin);

  if (readButton != lastButtonState && millis() - lastDebounceTime > debounceDelay) {
    if (readButton == HIGH && lastButtonState == LOW) {
      displayOn = !displayOn;
      Serial.println(displayOn ? "Display ON" : "Display OFF");
      lastDebounceTime = millis();  // Reset debounce timer on action
      if (!displayOn) clearDisplay();
    }
    lastButtonState = readButton;
  }

  if (displayOn) {
    updateDisplay();
  }
}

void displayText(const char* text) {
  static const char* lastText = "";  // Keep track of the last displayed text
  if (strcmp(text, lastText) != 0) {
    tft.fillRect(10, 190, 320, 30, TFT_BLACK);
    tft.setCursor(10, 200);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.print(text);
    lastText = text;
    Serial.println("Text updated: " + String(text));
  }
}

void updateDisplay() {
  int potValue = analogRead(25);
  if (potValue != lastPotValue) {  // Check if potentiometer value changed
    int segment = map(potValue, 0, 4095, 0, 4);
    const char* texts[] = {"Hi", "How are you", "What are you up to lately", "Miss me?", "Bye there"};
    displayText(texts[segment]);
    lastPotValue = potValue;  // Update the last value
  }
}

void clearDisplay() {
  tft.fillRect(10, 190, 320, 30, TFT_BLACK);
  Serial.println("Display cleared.");
}
