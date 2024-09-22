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

// Create TFT object
TFT_eSPI tft = TFT_eSPI();

// Create BLE mouse object
BleMouse bleMouse;

// Variables to store touch coordinates
uint16_t touchX = 0, touchY = 0;
uint16_t lastTouchX = 0, lastTouchY = 0;

void setup() {
  Serial.begin(115200);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  
  // Initialize the backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Initialize touch controller
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  pinMode(TOUCH_INT, INPUT_PULLUP);
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(10);
  digitalWrite(TOUCH_RST, HIGH);
  delay(50);

  // Initialize BLE mouse
  bleMouse.begin();
  Serial.println("BLE Mouse Initialized");
}

void loop() {
  if (digitalRead(TOUCH_INT) == LOW) {
    if (readTouch()) {
      // Calculate delta movements for X and Y
      int16_t deltaX = touchX - lastTouchX;
      int16_t deltaY = touchY - lastTouchY;

      // Update the last touch positions
      lastTouchX = touchX;
      lastTouchY = touchY;

      // Normalize the delta to avoid large jumps
      deltaX = constrain(deltaX, -10, 10);  // Limit movement to smooth out erratic movements
      deltaY = constrain(deltaY, -10, 10);

      // Map the deltas to mouse movement ranges
      int16_t mappedX = map(deltaX, -10, 10, -127, 127);
      int16_t mappedY = map(deltaY, -10, 10, -127, 127);

      // Move the BLE mouse with the mapped delta movements
      if (bleMouse.isConnected()) {
        bleMouse.move(mappedX, mappedY);
      }

      // Debugging output
      Serial.print("Touch X: ");
      Serial.print(touchX);
      Serial.print(", Y: ");
      Serial.print(touchY);
      Serial.print(", Delta X: ");
      Serial.print(deltaX);
      Serial.print(", Delta Y: ");
      Serial.print(deltaY);
      Serial.print(", Mapped X: ");
      Serial.print(mappedX);
      Serial.print(", Mapped Y: ");
      Serial.println(mappedY);

      // Display the touch coordinates on the screen
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(20, 20);
      tft.print("Touch at: ");
      tft.print(touchX);
      tft.print(", ");
      tft.print(touchY);
    }
  }

  delay(10); // Small delay to avoid spamming
}

bool readTouch() {
  Wire.beginTransmission(TOUCH_ADDR);
  Wire.write(0x02);  // Register for touch data
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  Wire.requestFrom(TOUCH_ADDR, 5);  // Request 5 bytes from touch controller
  if (Wire.available() == 5) {
    uint8_t gesture = Wire.read();        // Read gesture byte (ignore for now)
    uint8_t highByteX = Wire.read();      // High byte for X
    uint8_t lowByteX = Wire.read();       // Low byte for X
    uint8_t highByteY = Wire.read();      // High byte for Y
    uint8_t lowByteY = Wire.read();       // Low byte for Y

    // Combine high and low bytes to form 16-bit values for X and Y
    touchX = ((highByteX & 0x0F) << 8) | lowByteX;  // Mask out the top 4 bits
    touchY = ((highByteY & 0x0F) << 8) | lowByteY;  // Mask out the top 4 bits

    // Make sure X and Y are within screen bounds
    touchX = constrain(touchX, 0, SCREEN_WIDTH);
    touchY = constrain(touchY, 0, SCREEN_HEIGHT);

    return true;
  }

  return false;
}
