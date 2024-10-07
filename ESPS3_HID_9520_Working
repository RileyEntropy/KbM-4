// KBM4 Expressif Firmware 0.0.1
// by Riley Amber :)
// Upload to ESP32S3 Supermini
// Target ESP32S3 Dev Module
//Gyro axis needs reassignment

#include <Wire.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include <math.h>
#include <MPU9250.h>

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
//Gyro Orientation
#define Rotate90deg_Left 0
#define Rotate90deg_Right 0
#define Rotate90deg_Up 0
#define Rotate90deg_Down 0
#define GYRO_SENSITIVITY 1    // Sensitivity multiplier for gyro movement
#define GYRO_DEADZONE_X 0.8    // Deadzone for gyro X-axis
#define GYRO_DEADZONE_Y 0.8    // Deadzone for gyro Y-axis
#define GYRO_DEADZONE_Z 0.8    // Deadzone for gyro Z-axis

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

void setup() {
  // Safe start condition: Wait for user input to stabilize
  // Replacing delay with a non-blocking approach
  unsigned long startMillis = millis();
  while (millis() - startMillis < 2000) {
    // Waiting for stabilization
  }
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial connection
  }
  delay(2000); // Small delay to ensure serial connection is established
  Serial.println("Serial initialized successfully");

  Serial.println("Waiting for joystick and buttons to stabilize...");
  while (abs(analogRead(JOYSTICK_LR) - joystickBaselineLR) > JOYSTICK_DEFAULT_DEADZONE * 2 || abs(analogRead(JOYSTICK_UD) - joystickBaselineUD) > JOYSTICK_DEFAULT_DEADZONE * 2) {
    Serial.println("Please ensure the joystick is centered.");
    delay(500);
  }
  Serial.println("Joystick stabilized, proceeding...");

  // Initialize USB HID
  Keyboard.begin();
  Mouse.begin();
  USB.begin();
  Serial.println("USB Mouse and Keyboard ready");

  // Set up pin modes and initialize previous states
  for (int i = 0; i < sizeof(gpioPins) / sizeof(gpioPins[0]); i++) {
    pinMode(gpioPins[i], INPUT_PULLUP);
    previousStates[i] = HIGH; // Initialize all previous states to HIGH (not pressed)
  }

  Serial.println("GPIO pins initialized, starting loop...");

  // Calibrate joystick baseline
  delay(2000);  // Allow some time for joystick to settle before reading baseline
  joystickBaselineLR = analogRead(JOYSTICK_LR);
  joystickBaselineUD = analogRead(JOYSTICK_UD);
  Serial.print("Joystick baseline LR: ");
  Serial.println(joystickBaselineLR);
  Serial.print("Joystick baseline UD: ");
  Serial.println(joystickBaselineUD);

  // Initialize MPU9250
  Wire.begin(GYRO_SDA, GYRO_SCL);
  if (!mpu.setup(0x68)) {
    Serial.println("MPU9250 initialization failed!");
    while (1);
  }
  Serial.println("MPU9250 initialized successfully");

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
      Serial.println("New min LR recorded");
    }
    if (joystickLR > joystickMaxLR) {
      joystickMaxLR = joystickLR;
      Serial.println("New max LR recorded");
    }
    if (joystickUD < joystickMinUD) {
      joystickMinUD = joystickUD;
      Serial.println("New min UD recorded");
    }
    if (joystickUD > joystickMaxUD) {
      joystickMaxUD = joystickUD;
      Serial.println("New max UD recorded");
    }

    // Check if calibration is complete (both axes have been moved sufficiently)
    if ((joystickMaxLR - joystickMinLR > JOYSTICK_DEFAULT_DEADZONE * 2) &&
        (joystickMaxUD - joystickMinUD > JOYSTICK_DEFAULT_DEADZONE * 2)) {
      calibrationComplete = true;
      joystickDeadzone = JOYSTICK_DEFAULT_DEADZONE / 2; // Reduce deadzone after calibration
      Serial.println("Calibration complete. Deadzone reduced.");
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
            break;
          case FIVE_D_CENTER:
            Keyboard.press(FIVE_D_CENTER_OUTPUT);
            break;
          case FIVE_D_DOWN:
            Keyboard.press(FIVE_D_DOWN_OUTPUT);
            break;
          case FIVE_D_RIGHT:
            Keyboard.press(FIVE_D_RIGHT_OUTPUT);
            break;
          case FIVE_D_UP:
            Keyboard.press(FIVE_D_UP_OUTPUT);
            break;
          case TRIGGER:
            Mouse.press(TRIGGER_OUTPUT);
            break;
          case MAG_RELEASE:
            Keyboard.press(MAG_RELEASE_OUTPUT);
            break;
          case BOLT_CATCH_RELEASE:
            Keyboard.press(BOLT_CATCH_OUTPUT);
            break;
          case ADS_TOUCH:
            Mouse.press(ADS_TOUCH_OUTPUT);
            break;
        }
        anyButtonPressed = true;
      } else {
        // Button released
        switch (gpioPins[i]) {
          case FIVE_D_LEFT:
            Keyboard.release(FIVE_D_LEFT_OUTPUT);
            break;
          case FIVE_D_CENTER:
            Keyboard.release(FIVE_D_CENTER_OUTPUT);
            break;
          case FIVE_D_DOWN:
            Keyboard.release(FIVE_D_DOWN_OUTPUT);
            break;
          case FIVE_D_RIGHT:
            Keyboard.release(FIVE_D_RIGHT_OUTPUT);
            break;
          case FIVE_D_UP:
            Keyboard.release(FIVE_D_UP_OUTPUT);
            break;
          case TRIGGER:
            Mouse.release(TRIGGER_OUTPUT);
            break;
          case MAG_RELEASE:
            Keyboard.release(MAG_RELEASE_OUTPUT);
            break;
          case BOLT_CATCH_RELEASE:
            Keyboard.release(BOLT_CATCH_OUTPUT);
            break;
          case ADS_TOUCH:
            Mouse.release(ADS_TOUCH_OUTPUT);
            break;
        }
      }
      previousStates[i] = currentState;
    }
  }

  return anyButtonPressed;
}

void calibrateBaseline() {
  Serial.println("Calibrating gyro baseline...");
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

  Serial.print("Baseline X: ");
  Serial.println(baselineX);
  Serial.print("Baseline Y: ");
  Serial.println(baselineY);
  Serial.print("Baseline Z: ");
  Serial.println(baselineZ);
}

void handleIMUMovement() {
    float deltaX = mpu.getGyroX() - baselineX;
    float deltaY = mpu.getGyroY() - baselineY;
    float deltaZ = mpu.getGyroZ() - baselineZ;

    // Apply rotation adjustments
    if (Rotate90deg_Left) {
        float temp = deltaX;
        deltaX = -deltaY;
        deltaY = temp;
    } else if (Rotate90deg_Right) {
        float temp = deltaX;
        deltaX = deltaY;
        deltaY = -temp;
    } else if (Rotate90deg_Up) {
        deltaY = -deltaY;
    } else if (Rotate90deg_Down) {
        deltaX = -deltaX;
    }

    // Apply deadzone
    if (abs(deltaX) < GYRO_DEADZONE_X) deltaX = 0;
    if (abs(deltaY) < GYRO_DEADZONE_Y) deltaY = 0;
    if (abs(deltaZ) < GYRO_DEADZONE_Z) deltaZ = 0;

    // Move mouse based on gyro data
    int moveX = static_cast<int>(deltaX * GYRO_SENSITIVITY); // Adjust sensitivity as needed
    int moveY = static_cast<int>(-deltaY * GYRO_SENSITIVITY);

    // Move the mouse
    Mouse.move(moveX, moveY);
}
