#include <TFT_eSPI.h>
#include <BleMouse.h>
#include <Wire.h>

#define SCREEN_WIDTH 320  // Adjust for landscape
#define SCREEN_HEIGHT 240 // Adjust for landscape

#define TOUCH_SDA 11
#define TOUCH_SCL 10
#define TOUCH_INT 14
#define TOUCH_RST 13

#define TOUCH_ADDR 0x15  // I2C address of CST816S

TFT_eSPI tft = TFT_eSPI();
BleMouse bleMouse;

uint16_t touchX = 0, touchY = 0;
uint16_t lastTouchX = 0, lastTouchY = 0;
bool isTouching = false;
bool hasMoved = false;

void setup() {
  Serial.begin(115200);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(1);  // Landscape, USB port facing downward (correct text orientation)
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  
  // Turn on the backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Initialize I2C for the touch controller
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  pinMode(TOUCH_INT, INPUT_PULLUP);
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(10);
  digitalWrite(TOUCH_RST, HIGH);
  delay(50);

  // Initialize the BLE mouse
  bleMouse.begin();
  Serial.println("BLE Mouse Initialized");
}

void loop() {
  bool touchDetected = digitalRead(TOUCH_INT) == LOW;

  if (touchDetected && readTouch()) {
    if (!isTouching) {
      lastTouchX = touchX;
      lastTouchY = touchY;
      isTouching = true;
      hasMoved = false;
    }

    // Map touch coordinates to screen dimensions
    uint16_t mappedX = map(touchX, 0, 4095, 0, SCREEN_WIDTH - 1);
    uint16_t mappedY = map(touchY, 0, 4095, 0, SCREEN_HEIGHT - 1);

    // Calculate relative movement (delta)
    int16_t deltaX = mappedX - lastTouchX;
    int16_t deltaY = mappedY - lastTouchY;

    // If there is significant movement, update the mouse position
    if (abs(deltaX) > 1 || abs(deltaY) > 1) {
      hasMoved = true;

      // Adjust sensitivity and apply constraints for smooth and controlled movement
      float sensitivity = 0.5;  // Adjust this for higher or lower sensitivity
      int16_t moveX = constrain(deltaX * sensitivity, -127, 127);
      int16_t moveY = constrain(deltaY * sensitivity, -127, 127);

      if (bleMouse.isConnected()) {
        // Move the cursor relative to the touch movement
        bleMouse.move(moveX, moveY);
      }

      // Update last touch coordinates for the next movement calculation
      lastTouchX = mappedX;
      lastTouchY = mappedY;

      Serial.printf("Touch X: %d, Y: %d, Mapped X: %d, Y: %d, Move X: %d, Y: %d\n",
                    touchX, touchY, mappedX, mappedY, moveX, moveY);

      // Display touch coordinates for debugging purposes
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(20, 20);
      tft.printf("Touch: %d, %d", mappedX, mappedY);
      tft.setCursor(20, 50);
      tft.printf("Move: %d, %d", moveX, moveY);
    }
  } else if (!touchDetected && isTouching) {
    isTouching = false;
    Serial.println("Touch released.");
  }

  delay(5);  // Small delay for responsiveness
}

bool readTouch() {
  Wire.beginTransmission(TOUCH_ADDR);
  Wire.write(0x02);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  Wire.requestFrom(TOUCH_ADDR, 6);
  if (Wire.available() == 6) {
    Wire.read(); // Gesture byte (ignored)
    touchX = (Wire.read() << 8) | Wire.read();
    touchY = (Wire.read() << 8) | Wire.read();
    Wire.read(); // Pressure (ignored)
    return true;
  }

  return false;
}