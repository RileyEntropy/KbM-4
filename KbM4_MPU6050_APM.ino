
//To implement absolute positioning for the roll axis (X-axis), ensuring the controller maintains Q or E pressed until the controller tilts back to its neutral position, we need to adjust how the roll angle is calculated and how the keypress is managed.

//Desired Behavior:
//Roll Left (past +10 degrees): Hold Q until the controller returns to a neutral (0-degree) position.
//Roll Right (past -10 degrees): Hold E until the controller returns to neutral.
//Avoid immediate toggling between Q and E when crossing the 0-degree point.
//We'll track the absolute position of the roll axis and implement logic to hold the key until the tilt moves back toward 0 degrees.

//Solution:
//Track the roll angle (X-axis) and compare it to 10 degrees for Q (left tilt) and -10 degrees for E (right tilt).
// flags to track whether Q or E is being held, and ensure they aren't toggled too quickly
#include <Wire.h>
#include <Mouse.h>
#include <Keyboard.h>  // Include Keyboard library for Q and E key presses
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// IMU Settings
Adafruit_MPU6050 mpu;  // Declare an MPU6050 object
#define TILT_THRESHOLD 20.0f  // 20-degree tilt threshold for Q and E keypresses
#define RETURN_THRESHOLD 5.0f  // 5-degree buffer to return to neutral

// Variables
bool isQPressed = false;
bool isEPressed = false;

void setup() {
  Serial.begin(115200);

  // Initialize I2C for IMU
  Wire.begin();  // Use default I2C pins (SDA on Pin 2, SCL on Pin 3)

  // Initialize MPU6050 using the default I2C address (0x68)
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Initialize USB Mouse and Keyboard
  Mouse.begin();
  Keyboard.begin();
  Serial.println("USB Mouse and Keyboard Initialized");
}

void loop() {
  // Handle IMU cursor movement and keypress
  handleIMUMovement();

  delay(10);  // Adjust delay for smooth operation
}

void handleIMUMovement() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);  // Get gyroscope and accelerometer data

  // **X-axis (pitch)** for up-down movement of the cursor
  float deltaX = g.gyro.x;  // Pitch (X-axis) for up-down movement

  // **Z-axis (yaw)** for left-right movement of the cursor
  float deltaZ = g.gyro.z;  // Yaw (Z-axis) for left-right movement

  // Apply a multiplier to enhance sensitivity (adjust as necessary)
  float pitchMovement = deltaX * 8.0f;  // Adjust multiplier as needed
  float yawMovement = deltaZ * 8.0f;

  // Move the cursor
  Mouse.move(-yawMovement, pitchMovement);

  // **Y-axis (roll)** for detecting tilt and triggering Q or E keys
  float roll = g.gyro.y;  // Use Y-axis for tilt detection (roll)

  // If roll is greater than 20 degrees, press and hold Q
  if (roll > TILT_THRESHOLD && !isQPressed) {
    Keyboard.press('Q');
    isQPressed = true;
    isEPressed = false;  // Ensure E is not pressed
    Serial.println("Q pressed due to tilt left");
  }

  // If roll is less than -20 degrees, press and hold E
  if (roll < -TILT_THRESHOLD && !isEPressed) {
    Keyboard.press('E');
    isEPressed = true;
    isQPressed = false;  // Ensure Q is not pressed
    Serial.println("E pressed due to tilt right");
  }

  // Release Q when roll returns closer to neutral
  if (isQPressed && roll < RETURN_THRESHOLD) {
    Keyboard.release('Q');
    isQPressed = false;
    Serial.println("Q released, returned to neutral");
  }

  // Release E when roll returns closer to neutral
  if (isEPressed && roll > -RETURN_THRESHOLD) {
    Keyboard.release('E');
    isEPressed = false;
    Serial.println("E released, returned to neutral");
  }
}
