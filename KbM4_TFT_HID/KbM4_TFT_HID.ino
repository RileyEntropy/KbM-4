#include <TFT_eSPI.h>
#include <BleMouse.h>
#include <Wire.h>

#define TOUCH_SDA 11
#define TOUCH_SCL 10
#define TOUCH_INT 14
#define TOUCH_RST 13
#define TOUCH_ADDR 0x15

#define MULTIPLIER 5.0  // Adjust this to change cursor movement range

TFT_eSPI tft = TFT_eSPI();
BleMouse bleMouse;

uint16_t touchX = 0, touchY = 0;
uint16_t lastTouchX = 0, lastTouchY = 0;
bool isTouching = false;

void setup() {
  Serial.begin(115200);
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
    }

    int16_t deltaX = touchY - lastTouchY;  // Y controls X movement
    int16_t deltaY = touchX - lastTouchX;  // X controls Y movement

    int16_t moveX = deltaX * MULTIPLIER;
    int16_t moveY = deltaY * MULTIPLIER;

    if (bleMouse.isConnected()) {
      bleMouse.move(moveX, -moveY);  // Invert Y for natural movement
    }

    lastTouchX = touchX;
    lastTouchY = touchY;

    Serial.printf("Touch X: %d, Y: %d, Move X: %d, Y: %d\n", touchX, touchY, moveX, -moveY);
  } else if (!touchDetected && isTouching) {
    isTouching = false;
    Serial.println("Touch released.");
  }

  delay(10);
}

bool readTouch() {
  Wire.beginTransmission(TOUCH_ADDR);
  Wire.write(0x02);
  if (Wire.endTransmission(false) != 0) return false;
  
  Wire.requestFrom(TOUCH_ADDR, 6);
  if (Wire.available() == 6) {
    Wire.read();
    touchX = (Wire.read() << 8) | Wire.read();
    touchY = (Wire.read() << 8) | Wire.read();
    Wire.read();
    return true;
  }
  return false;
}
