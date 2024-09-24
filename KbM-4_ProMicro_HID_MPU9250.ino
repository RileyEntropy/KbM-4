#include <MPU9250.h>
#include <HID-Project.h>
#include <Wire.h>
#include <math.h>

// MPU9250 object
MPU9250 mpu;

// Response Curve parameters
#define RC_RATE_X 2.0f  // Range: 0.01-2.55 (for X-axis, vertical movement) default 2.0
#define RC_RATE_Z 2.0f  // Range: 0.01-2.55 (for Z-axis, horizontal movement) default 2.0
#define RATE_X 0.8f     // Range: 0.0-1.0 (for X-axis, vertical movement) default 0.8
#define RATE_Z 0.8f     // Range: 0.0-1.0 (for Z-axis, horizontal movement) default 0.8
#define EXPO_X 0.5f     // Range: 0.0-1.0 (for X-axis, vertical movement) default 0.5
#define EXPO_Z 0.5f     // Range: 0.0-1.0 (for Z-axis, horizontal movement) default 0.5
#define MASTER_MULTIPLIER 2.5f // Range: 0.1-5.0, (Final multiplier applied to cursor movement) default 2.5

// Other customizable parameters
#define MAX_DELTA 1000.0f          // Range: 100-5000, Maximum cursor movement per update
#define UPDATE_INTERVAL 1          // Range: 1-20 ms, Time between updates
#define SMOOTHING_FACTOR 0.7f      // Range: 0.0-1.0, Higher values make movement smoother but less responsive
#define DRIFT_THRESHOLD 0.1f       // Range: 0.01-1.0, Minimum movement to register (reduces unintended movement)
#define RATE_MULTIPLIER_FACTOR 0.5f // Range: 0.1-2.0, Impact of Y-axis on sensitivity

// New options for inverting joystick axes
#define INVERT_WS 0  // Set to 1 to invert W and S keys, 0 for normal operation
#define INVERT_AD 0  // Set to 1 to invert A and D keys, 0 for normal operation

//Pro Micro Pinout
//------------------------------- EDGE OF BOARD ----------------- VCC SIDE
const int SW1_10 = 10; // (A10/PWM) Thin-film pressure sensor Analog Pin
const int SW2_16 = 16; // (MOSI) Foregrip Switch 1
const int MAG_RELEASE = 14; //(MISO) "r"  Mag Release
const int TRIGGER = 15; // (SCLK)  "LMB" Trigger
const int PLUG_5P_1 = 18; // (A0) Pin 1 (blue) on 5 pin Molex connector
const int Y_JS = 19; // (A1) Joystick Y pin definition for "w" "s" //Joystick Y
const int X_JS = 20; // A2 Joystick pin definition for "a" "d" //Joystick X
const int PRESSURE_SENSOR_PIN = 21;  // (A3) MMB // Thin-film pressure sensor Analog Pin

//----------------------------- EDGE OF BOARD -------------------- I2C SIDE
const int JS_B_PIN = 9; //(A9/PWM) Joystick Button
//const int 8_PIN = 8; //(A8) 5D Hat Up
//const int 7_PIN = 7; // 5D Hat Down
//const int 6_PIN = 6; //(A7/PWM) 5D Hat Left
//const int 5_PIN = 5; //(PWM) 5D Hat Right
//const int 4_PIN = 4; //(A6) 5D Hat Center
const int SCL_PIN = 3; //(SCL) I2C
const int SDA_PIN = 2; //(SDA) I2C
//const int TX_PIN = 1; //(RX) to ESP-32-S3 TX
//const int RX_PIN = 0; //(TX) to ESP-32-S3 RX

const int LED_PIN = 13; //Onboard LED

// Joystick thresholds
const int JS_THRESHOLD = 100;

float baselineX, baselineY, baselineZ;
float smoothedDeltaX = 0, smoothedDeltaZ = 0;

// Response Curve function
float applyResponseCurve(float input, float rcRate, float rate, float expo) {
    float expoCurve = input * input * input * expo + input * (1 - expo);
    float rcRateCurve = rcRate * expoCurve;
    float outputRate = 200.0f * rcRateCurve * rate;
    return outputRate;
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!mpu.setup(0x68)) {
        Serial.println("MPU9250 initialization failed!");
        while (1);
    }
    Serial.println("MPU9250 initialized successfully");

    Mouse.begin();
    Keyboard.begin();
    Serial.println("USB Mouse and Keyboard ready");

    // Set up pin modes
    pinMode(SW1_10, INPUT_PULLUP);
    pinMode(SW2_16, INPUT_PULLUP);
    pinMode(MAG_RELEASE, INPUT_PULLUP);
    pinMode(TRIGGER, INPUT_PULLUP);
    pinMode(PLUG_5P_1, INPUT_PULLUP);
    pinMode(JS_B_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    calibrateBaseline();
}

void loop() {
    if (mpu.update()) {
        handleGyroMouse();
    }
    
    handleKeyboardInputs();
    handleJoystick();

    delay(UPDATE_INTERVAL);
}

void handleGyroMouse() {
    // ... (keep this function unchanged)
}

void handleKeyboardInputs() {
    // ... (keep this function unchanged)
}

void handleJoystick() {
    int xValue = analogRead(X_JS);
    int yValue = analogRead(Y_JS);

    if (INVERT_AD) {
        // Invert A and D keys
        if (xValue > 512 + JS_THRESHOLD) {
            Keyboard.press('a');
        } else if (xValue < 512 - JS_THRESHOLD) {
            Keyboard.press('d');
        } else {
            Keyboard.release('a');
            Keyboard.release('d');
        }
    } else {
        // Normal A and D keys
        if (xValue < 512 - JS_THRESHOLD) {
            Keyboard.press('a');
        } else if (xValue > 512 + JS_THRESHOLD) {
            Keyboard.press('d');
        } else {
            Keyboard.release('a');
            Keyboard.release('d');
        }
    }

    if (INVERT_WS) {
        // Invert W and S keys
        if (yValue > 512 + JS_THRESHOLD) {
            Keyboard.press('s');
        } else if (yValue < 512 - JS_THRESHOLD) {
            Keyboard.press('w');
        } else {
            Keyboard.release('w');
            Keyboard.release('s');
        }
    } else {
        // Normal W and S keys
        if (yValue < 512 - JS_THRESHOLD) {
            Keyboard.press('s');
        } else if (yValue > 512 + JS_THRESHOLD) {
            Keyboard.press('w');
        } else {
            Keyboard.release('w');
            Keyboard.release('s');
        }
    }
}

void calibrateBaseline() {
    // ... (keep this function unchanged)
}
