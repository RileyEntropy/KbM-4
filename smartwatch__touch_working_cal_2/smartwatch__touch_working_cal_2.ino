#include <TFT_eSPI.h>
#include <BleMouse.h>
#include <Wire.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 280

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
  tft.init();
  tft.setRotation(1);  // Make sure screen is also in the correct orientation
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  pinMode(TOUCH_INT, INPUT_PULLUP);
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(10);
  digitalWrite(TOUCH_RST, HIGH);
  delay(50);

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

    // Rotate the touch coordinates by 90 degrees clockwise
    uint16_t adjustedX = constrain(SCREEN_HEIGHT - touchY, 0, SCREEN_HEIGHT - 1);  // Invert Y (now X)
    uint16_t adjustedY = constrain(touchX, 0, SCREEN_WIDTH - 1);  // X is now Y

    int16_t deltaX = adjustedX - lastTouchX;
    int16_t deltaY = adjustedY - lastTouchY;

    if (abs(deltaX) > 1 || abs(deltaY) > 1) {
      hasMoved = true;
    }

    if (hasMoved) {
      // Adjust sensitivity for smoother movement
      float sensitivity = 1.5;  // Increased sensitivity for smoother response
      int16_t mappedX = constrain(deltaX * sensitivity, -127, 127);
      int16_t mappedY = constrain(deltaY * sensitivity, -127, 127);

      // Move the BLE mouse
      if (bleMouse.isConnected()) {
        bleMouse.move(mappedX, mappedY);
      }

      // Update last touch positions
      lastTouchX = adjustedX;
      lastTouchY = adjustedY;

      // Debugging output
      Serial.printf("Touch X: %d, Y: %d, Delta X: %d, Delta Y: %d, Mapped X: %d, Mapped Y: %d\n",
                    adjustedX, adjustedY, deltaX, deltaY, mappedX, mappedY);

      // Display touch coordinates on TFT
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(20, 20);
      tft.printf("Touch: %d, %d", adjustedX, adjustedY);
    }
  } else if (!touchDetected && isTouching) {
    isTouching = false;
    Serial.println("Touch released.");
  }

  delay(10);  // Small delay to improve touch response time
}

// Read touch data from CST816S
bool readTouch() {
  Wire.beginTransmission(TOUCH_ADDR);
  Wire.write(0x02);  // Register for touch data
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  Wire.requestFrom(TOUCH_ADDR, 5);w
    touchX = ((Wire.read() & 0x0F) << 8) | Wire.read();
    touchY = ((Wire.read() & 0x0F) << 8) | Wire.read();
    return true;
  }

  return false;
}


