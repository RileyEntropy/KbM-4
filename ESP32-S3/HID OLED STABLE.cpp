// KBM4 Expressif Firmware 0.0.1
// by Riley Amber :)
// Upload to ESP32S3 Supermini (target ESP32S3 Dev Module)

//==============================================  add wireless serial monitor for debugging
//add AP Mode for captive portal firmware configuration 
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WebServer.h>

//const char* ssid = "KBM4_Carbine";
//const char* password = "12345678";

//============================================== add 2.4ghz wireless NRF24L01+
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

//RF24 radio(7, 8); // CE, CSN

//============================================== 

#include <Wire.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include <math.h>
#include <MPU9250.h>
#include <U8g2lib.h>

// Forward declarations
bool handleKeyboardInputs();
void handleJoystickInputs();
void handleIMUMovement();
void calibrateBaseline();
void handleGyroMouse();

//=============================
//          GPIO Default Pins
//=============================
// Gyro I2C
#define GYRO_SDA 1
#define GYRO_SCL 2
// 5D Hat Switch
#define FIVE_D_CENTER 3
#define FIVE_D_UP 4
#define FIVE_D_RIGHT 5
#define FIVE_D_LEFT 6
#define FIVE_D_DOWN 7
// Mouse I2C
#define OPTICAL_SDA 8
#define OPTICAL_SCL 9
// Joystick Axes
#define JOYSTICK_UD 10
#define JOYSTICK_LR 11
#define JOYSTICK_DEFAULT_DEADZONE 1500 // Initial deadzone for joystick
// OLED I2C
#define OLED_SDA 12
#define OLED_SCL 13

//=============================
//    GPIO Default Pins Fire Controls
//=============================
#define TRIGGER 40        
#define ADS_TOUCH 12
#define BOLT_CATCH_RELEASE 42
#define SELECTOR_R 45
#define SELECTOR_L 46
#define MAG_RELEASE 47

//=============================
// GPIO Default Outputs (Keyboard/Mouse Mappings)
//=============================
#define FIVE_D_LEFT_OUTPUT 'l'
#define FIVE_D_CENTER_OUTPUT 'c'
#define FIVE_D_DOWN_OUTPUT 'd'
#define FIVE_D_RIGHT_OUTPUT 'r'
#define FIVE_D_UP_OUTPUT 'u'
// Joystick
#define JOYSTICK_LR_LEFT 'a'
#define JOYSTICK_LR_RIGHT 'd'
#define JOYSTICK_UD_UP 'w'
#define JOYSTICK_UD_DOWN 's'
// Fire Controls
#define TRIGGER_OUTPUT MOUSE_LEFT   
#define ADS_TOUCH_OUTPUT MOUSE_RIGHT
#define MAG_RELEASE_OUTPUT 'r'
#define BOLT_CATCH_OUTPUT 'b'

//=============================
//     Gyro Custom Defines
//=============================
// Axis Definitions
#define YAW   1 // Yaw corresponds to horizontal mouse movement (X-axis)
#define ROLL  2 // Roll corresponds to horizontal cursor acceleration multiplier
#define PITCH 3 // Pitch corresponds to vertical mouse movement (Y-axis)

// Custom Defines for FPS Optimization
#define CURSOR_SPEED_MULTIPLIER 1.5 // Adjust this value to change cursor speed
#define GYRO_SENSITIVITY_FINE 1.0 // Sensitivity multiplier for fine movements
#define GYRO_SENSITIVITY_GROSS 3.0 // Sensitivity multiplier for gross movements
#define GYRO_DEADZONE_X 0.1 // Deadzone for horizontal movement
#define GYRO_DEADZONE_Z 0.1 // Deadzone for vertical movement
#define ACCELERATION_FACTOR 0.1 // Factor for acceleration based on gyro input
#define GROSS_MOVEMENT_THRESHOLD 0.5 // Threshold for gross movement sensitivity

// Screen Aspect Ratio (adjust based on your display)
#define SCREEN_ASPECT_RATIO_X 16.0 // Width of the screen
#define SCREEN_ASPECT_RATIO_Y 9.0  // Height of the screen

// Inversion Defines
#define INVERT_YAW   true // Set to true to invert Yaw
#define INVERT_PITCH true // Set to true to invert Pitch

// Gyro Orientation
#define GYRO_DEADZONE_Y 0.8    // Deadzone for gyro Y-axis

// Create USB HID instances
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

// Store the previous state of each pin to detect changes
const int gpioPins[] = {
  FIVE_D_LEFT, FIVE_D_CENTER, FIVE_D_DOWN, FIVE_D_RIGHT, FIVE_D_UP,
  TRIGGER, MAG_RELEASE, BOLT_CATCH_RELEASE, ADS_TOUCH
};
int previousStates[sizeof(gpioPins) / sizeof(gpioPins[0])];

// Variables
bool isQPressed = false;
bool isEPressed = false;

int joystickBaselineLR = 0;
int joystickBaselineUD = 0;
int joystickMinLR = 4095;
int joystickMaxLR = 0;
int joystickMinUD = 4095;
int joystickMaxUD = 0;
int joystickDeadzone = JOYSTICK_DEFAULT_DEADZONE;
bool calibrationComplete = false;

// Gyro baseline variables
float baselineX = 0;
float baselineY = 0;
float baselineZ = 0;

// MPU9250 object
MPU9250 mpu;

// Low-pass filter coefficient (adjust as needed)
const float ALPHA = 0.1; // Smoothing factor (0 < ALPHA < 1)

float filteredDeltaX = 0; // Filtered value for Roll
float filteredDeltaZ = 0; // Filtered value for Pitch

// OLED Display configuration
#define OLED_ADDRESS 0x3C
#define OLED_ROTATION U8G2_R3
#define OLED_FONT u8g2_font_5x7_tr
#define OLED_LINE_HEIGHT 8
#define OLED_CHAR_WIDTH 15
#define OLED_DISPLAY_LINES 10
#define OLED_SCROLL_LINES (OLED_DISPLAY_LINES / 5)

// OLED object
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(OLED_ROTATION, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

// OLED display buffer
char displayBuffer[OLED_DISPLAY_LINES][OLED_CHAR_WIDTH + 1];
int currentLine = 0;

void setup() {
  // Remove or comment out Serial.begin and related lines
  // Serial.begin(115200);
  // while (!Serial) {
  //   delay(10);
  // }
  // delay(2000);
  // Serial.println("Serial initialized successfully");

  // Initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!u8g2.begin()) {
    while (1); // If OLED fails to initialize, halt the program
  }
  u8g2.setFont(OLED_FONT);
  addToDisplay("OLED initialized");

  addToDisplay("Waiting for joystick...");
  while (abs(analogRead(JOYSTICK_LR) - joystickBaselineLR) > JOYSTICK_DEFAULT_DEADZONE * 2 || 
         abs(analogRead(JOYSTICK_UD) - joystickBaselineUD) > JOYSTICK_DEFAULT_DEADZONE * 2) {
    addToDisplay("Center joystick");
    delay(500);
  }
  addToDisplay("Joystick stabilized");

  // Initialize USB HID
  Keyboard.begin();
  Mouse.begin();
  USB.begin();
  addToDisplay("USB HID ready");

  // Set up pin modes and initialize previous states
  for (int i = 0; i < sizeof(gpioPins) / sizeof(gpioPins[0]); i++) {
    pinMode(gpioPins[i], INPUT_PULLUP);
    previousStates[i] = HIGH;
  }
  addToDisplay("GPIO pins initialized");

  // Calibrate joystick baseline
  delay(2000);
  joystickBaselineLR = analogRead(JOYSTICK_LR);
  joystickBaselineUD = analogRead(JOYSTICK_UD);
  char joyMsg[22];
  snprintf(joyMsg, sizeof(joyMsg), "Joy LR:%d UD:%d", joystickBaselineLR, joystickBaselineUD);
  addToDisplay(joyMsg);

  // Initialize MPU9250
  Wire.begin(GYRO_SDA, GYRO_SCL);
  if (!mpu.setup(0x68)) {
    addToDisplay("MPU9250 init failed!");
    while (1);
  }
  addToDisplay("MPU9250 initialized");

  // Calibrate gyro baseline
  calibrateBaseline();
}

void loop() {
  handleJoystickInputs();
  bool buttonPressed = false;

  // Handle keyboard and mouse inputs based on GPIO
  buttonPressed = handleKeyboardInputs();

  // Handle IMU Movement
  if (mpu.update()) {
    handleIMUMovement();
  }

  // Update OLED display
  updateDisplay();

  delay(10); // Short delay for debounce
}

void handleJoystickInputs() {
  static bool isJoystickLRPressed = false;
  static bool isJoystickUDPressed = false;

  // Read joystick values and adjust for baseline
  int joystickLR = analogRead(JOYSTICK_LR) - joystickBaselineLR;
  int joystickUD = analogRead(JOYSTICK_UD) - joystickBaselineUD;

  // Update min and max values during calibration
  if (!calibrationComplete) {
    if (joystickLR < joystickMinLR) {
      joystickMinLR = joystickLR;
      addToDisplay("New min LR recorded");
    }
    if (joystickLR > joystickMaxLR) {
      joystickMaxLR = joystickLR;
      addToDisplay("New max LR recorded");
    }
    if (joystickUD < joystickMinUD) {
      joystickMinUD = joystickUD;
      addToDisplay("New min UD recorded");
    }
    if (joystickUD > joystickMaxUD) {
      joystickMaxUD = joystickUD;
      addToDisplay("New max UD recorded");
    }

    if ((joystickMaxLR - joystickMinLR > JOYSTICK_DEFAULT_DEADZONE * 2) &&
        (joystickMaxUD - joystickMinUD > JOYSTICK_DEFAULT_DEADZONE * 2)) {
      calibrationComplete = true;
      joystickDeadzone = JOYSTICK_DEFAULT_DEADZONE / 2;
      addToDisplay("Calibration complete");
    }
  } else {
    // Handle joystick inputs after calibration
    if (abs(joystickLR) > joystickDeadzone * 1.2f) {
      if (joystickLR < -joystickDeadzone) {
        if (!isJoystickLRPressed) {
          Keyboard.press(JOYSTICK_LR_LEFT);
          isJoystickLRPressed = true;
        }
      } else if (joystickLR > joystickDeadzone) {
        if (!isJoystickLRPressed) {
          Keyboard.press(JOYSTICK_LR_RIGHT);
          isJoystickLRPressed = true;
        }
      }
    } else {
      if (isJoystickLRPressed) {
        Keyboard.release(JOYSTICK_LR_LEFT);
        Keyboard.release(JOYSTICK_LR_RIGHT);
        isJoystickLRPressed = false;
      }
    }

    if (abs(joystickUD) > joystickDeadzone * 1.2f) {
      if (joystickUD < -joystickDeadzone) {
        if (!isJoystickUDPressed) {
          Keyboard.press(JOYSTICK_UD_UP);
          isJoystickUDPressed = true;
        }
      } else if (joystickUD > joystickDeadzone) {
        if (!isJoystickUDPressed) {
          Keyboard.press(JOYSTICK_UD_DOWN);
          isJoystickUDPressed = true;
        }
      }
    } else {
      if (isJoystickUDPressed) {
        Keyboard.release(JOYSTICK_UD_UP);
        Keyboard.release(JOYSTICK_UD_DOWN);
        isJoystickUDPressed = false;
      }
    }
  }
}

bool handleKeyboardInputs() {
  bool anyButtonPressed = false;

  // Handle button inputs based on GPIO pins
  for (int i = 0; i < sizeof(gpioPins) / sizeof(gpioPins[0]); i++) {
    int currentState = digitalRead(gpioPins[i]);
    if (currentState != previousStates[i]) {
      if (currentState == LOW) {
        // Button pressed
        switch (gpioPins[i]) {
          case FIVE_D_LEFT:
            Keyboard.press(FIVE_D_LEFT_OUTPUT);
            addToDisplay("Left pressed");
            break;
          case FIVE_D_CENTER:
            Keyboard.press(FIVE_D_CENTER_OUTPUT);
            addToDisplay("Center pressed");
            break;
          case FIVE_D_DOWN:
            Keyboard.press(FIVE_D_DOWN_OUTPUT);
            addToDisplay("Down pressed");
            break;
          case FIVE_D_RIGHT:
            Keyboard.press(FIVE_D_RIGHT_OUTPUT);
            addToDisplay("Right pressed");
            break;
          case FIVE_D_UP:
            Keyboard.press(FIVE_D_UP_OUTPUT);
            addToDisplay("Up pressed");
            break;
          case TRIGGER:
            Mouse.press(TRIGGER_OUTPUT);
            addToDisplay("Trigger pressed");
            break;
          case MAG_RELEASE:
            Keyboard.press(MAG_RELEASE_OUTPUT);
            addToDisplay("Mag release pressed");
            break;
          case BOLT_CATCH_RELEASE:
            Keyboard.press(BOLT_CATCH_OUTPUT);
            addToDisplay("Bolt catch pressed");
            break;
          case ADS_TOUCH:
            Mouse.press(ADS_TOUCH_OUTPUT);
            addToDisplay("ADS touch pressed");
            break;
        }
        anyButtonPressed = true;
      } else {
        // Button released
        switch (gpioPins[i]) {
          case FIVE_D_LEFT:
            Keyboard.release(FIVE_D_LEFT_OUTPUT);
            addToDisplay("Left released");
            break;
          case FIVE_D_CENTER:
            Keyboard.release(FIVE_D_CENTER_OUTPUT);
            addToDisplay("Center released");
            break;
          case FIVE_D_DOWN:
            Keyboard.release(FIVE_D_DOWN_OUTPUT);
            addToDisplay("Down released");
            break;
          case FIVE_D_RIGHT:
            Keyboard.release(FIVE_D_RIGHT_OUTPUT);
            addToDisplay("Right released");
            break;
          case FIVE_D_UP:
            Keyboard.release(FIVE_D_UP_OUTPUT);
            addToDisplay("Up released");
            break;
          case TRIGGER:
            Mouse.release(TRIGGER_OUTPUT);
            addToDisplay("Trigger released");
            break;
          case MAG_RELEASE:
            Keyboard.release(MAG_RELEASE_OUTPUT);
            addToDisplay("Mag release released");
            break;
          case BOLT_CATCH_RELEASE:
            Keyboard.release(BOLT_CATCH_OUTPUT);
            addToDisplay("Bolt catch released");
            break;
          case ADS_TOUCH:
            Mouse.release(ADS_TOUCH_OUTPUT);
            addToDisplay("ADS touch released");
            break;
        }
      }
      previousStates[i] = currentState;
    }
  }

  return anyButtonPressed;
}

void calibrateBaseline() {
  addToDisplay("Calibrating gyro...");
  float sumX = 0, sumY = 0, sumZ = 0;
  const int samples = 100;

  for (int i = 0; i < samples; i++) {
    if (mpu.update()) {
      sumX += mpu.getGyroX();
      sumY += mpu.getGyroY();
      sumZ += mpu.getGyroZ();
    }
    delay(10);
  }

  baselineX = sumX / samples;
  baselineY = sumY / samples;
  baselineZ = sumZ / samples;

  char baselineMsg[22];
  snprintf(baselineMsg, sizeof(baselineMsg), "BL X:%.2f Y:%.2f Z:%.2f", baselineX, baselineY, baselineZ);
  addToDisplay(baselineMsg);
}

void handleIMUMovement() {
    float rawDeltaX = mpu.getGyroX() - baselineX; // Raw Roll
    float rawDeltaZ = mpu.getGyroZ() - baselineZ; // Raw Pitch

    // Apply deadzone
    if (abs(rawDeltaX) < GYRO_DEADZONE_X) rawDeltaX = 0; // Deadzone for horizontal movement
    if (abs(rawDeltaZ) < GYRO_DEADZONE_Z) rawDeltaZ = 0; // Deadzone for vertical movement

    // Invert values based on defines
    if (INVERT_YAW)   rawDeltaX = -rawDeltaX;   // Invert horizontal movement (Roll)
    if (INVERT_PITCH) rawDeltaZ = -rawDeltaZ;   // Invert Pitch if defined

    // Determine sensitivity based on movement magnitude
    float sensitivity = (abs(rawDeltaX) > GROSS_MOVEMENT_THRESHOLD || abs(rawDeltaZ) > GROSS_MOVEMENT_THRESHOLD) 
                        ? GYRO_SENSITIVITY_GROSS 
                        : GYRO_SENSITIVITY_FINE;

    // Calculate mouse movement based on raw gyro data
    int moveX = static_cast<int>(rawDeltaX * sensitivity * CURSOR_SPEED_MULTIPLIER); // Use Roll for horizontal movement
    int moveY = static_cast<int>(-rawDeltaZ * sensitivity * CURSOR_SPEED_MULTIPLIER); // Use Pitch for vertical movement

    // Normalize movement based on aspect ratio
    float aspectRatio = SCREEN_ASPECT_RATIO_X / SCREEN_ASPECT_RATIO_Y;
    if (aspectRatio > 1.0) {
        moveY = static_cast<int>(moveY / aspectRatio); // Adjust vertical movement for wider screens
    } else {
        moveX = static_cast<int>(moveX * aspectRatio); // Adjust horizontal movement for taller screens
    }

    // Apply acceleration based on movement
    moveX += static_cast<int>(moveX * ACCELERATION_FACTOR); // Simple acceleration model
    moveY += static_cast<int>(moveY * ACCELERATION_FACTOR); // Simple acceleration model

    // Limit the maximum movement to prevent erratic behavior
    const int MAX_MOVE = 20; // Adjust this value as needed for FPS responsiveness
    moveX = constrain(moveX, -MAX_MOVE, MAX_MOVE);
    moveY = constrain(moveY, -MAX_MOVE, MAX_MOVE);

    // Move the mouse
    Mouse.move(moveX, moveY);

    // Add OLED output for significant movements
    if (abs(moveX) > 5 || abs(moveY) > 5) {
      char buffer[22];
      snprintf(buffer, sizeof(buffer), "Move: X%d Y%d", moveX, moveY);
      addToDisplay(buffer);
    }
}

// New function to add text to OLED display buffer
void addToDisplay(const char* message) {
  strncpy(displayBuffer[currentLine], message, OLED_CHAR_WIDTH);
  displayBuffer[currentLine][OLED_CHAR_WIDTH] = '\0';
  currentLine = (currentLine + 1) % OLED_DISPLAY_LINES;
  updateDisplay();
}

// New function to update OLED display
void updateDisplay() {
  u8g2.clearBuffer();
  for (int i = 0; i < OLED_DISPLAY_LINES; i++) {
    int line = (currentLine + i) % OLED_DISPLAY_LINES;
    u8g2.drawStr(0, (i + 1) * OLED_LINE_HEIGHT, displayBuffer[line]);
  }
  u8g2.sendBuffer();
}
