#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();

// Touch I2C address
#define TOUCH_I2C_ADD 0x15

// Pin definitions
#define TFT_BL   21  // Backlight pin
#define TOUCH_SDA 19
#define TOUCH_SCL 22  // SCL for both display and touch

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setup starting");

  // Initialize I2C for touch
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  Serial.println("I2C initialized");

  // Initialize display
  tft.init();
  Serial.println("Display initialized");

  // Setup backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  Serial.println("Backlight on");

  // Basic drawing test
  tft.fillScreen(TFT_RED);
  Serial.println("Screen filled with red");
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  Serial.println("Screen filled with green");
  delay(1000);
  tft.fillScreen(TFT_BLUE);
  Serial.println("Screen filled with blue");
  delay(1000);

  Serial.println("Setup complete");
}

void loop() {
  static unsigned long lastPrint = 0;
  unsigned long now = millis();

  // Print a message every 5 seconds
  if (now - lastPrint >= 5000) {
    Serial.println("Loop running...");
    lastPrint = now;

    // Draw a rectangle on the screen
    tft.fillRect(random(tft.width()), random(tft.height()), 20, 20, TFT_YELLOW);
  }

  delay(10);
}