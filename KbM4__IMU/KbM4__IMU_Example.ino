#include "FastIMU.h"
#include "F_QMI8658.hpp"
#include <BleMouse.h>
#include <math.h>

QMI8658 IMU;
BleMouse bleMouse;

#define I2C_SDA 11
#define I2C_SCL 10
#define QMI8658_ADDR 0x6B

// Betaflight "Actual" rates parameters
#define RC_RATE_X 1.0f  // Range: 0.01-2.55
#define RC_RATE_Y 1.0f  // Range: 0.01-2.55
#define RATE_X 0.8f     // Range: 0.0-1.0
#define RATE_Y 0.8f     // Range: 0.0-1.0
#define EXPO_X 0.2f     // Range: 0.0-1.0
#define EXPO_Y 0.2f     // Range: 0.0-1.0

// Other customizable parameters
#define MAX_DELTA 1000.0f
#define UPDATE_INTERVAL 1
#define SMOOTHING_FACTOR 0.7f
#define DRIFT_THRESHOLD 0.1f

float baselineX, baselineY;
float smoothedDeltaX = 0, smoothedDeltaY = 0;

float applyBetaflightActualRates(float rcCommand, float rcRate, float rate, float expo) {
    float centerSens = rcRate * 200.0f;
    float stickMovement = rcCommand;
    float breakpoint = 1.0f / rate;
    
    if (rcCommand != 0) {
        if (rcCommand > 0) {
            if (rcCommand >= breakpoint) {
                stickMovement = 1.0f;
            } else {
                stickMovement = rcCommand * rate;
            }
        } else {
            if (rcCommand <= -breakpoint) {
                stickMovement = -1.0f;
            } else {
                stickMovement = rcCommand * rate;
            }
        }
    }
    
    float angleRate = 200.0f * rcRate * stickMovement;
    angleRate = angleRate * (1.0f + stickMovement * expo * (stickMovement * stickMovement - 1.0f));
    
    return angleRate;
}

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    
    calData cal;
    cal.valid = false;
    
    if (IMU.init(cal, QMI8658_ADDR) != 0) {
        Serial.println("IMU initialization failed");
        while (1) {
            delay(10);
        }
    }
    Serial.println("IMU initialized successfully");

    bleMouse.begin();
    Serial.println("BLE Mouse ready");

    calibrateBaseline();
}

void loop() {
    GyroData gyro;

    IMU.update();
    IMU.getGyro(&gyro);

    float deltaX = gyro.gyroY - baselineY;
    float deltaY = gyro.gyroX - baselineX;

    // Normalize gyro input to -1 to 1 range (assuming max gyro reading of 2000 deg/s)
    float normalizedX = constrain(deltaX / 2000.0f, -1.0f, 1.0f);
    float normalizedY = constrain(deltaY / 2000.0f, -1.0f, 1.0f);

    // Apply Betaflight "Actual" rates
    float rateX = applyBetaflightActualRates(normalizedX, RC_RATE_X, RATE_X, EXPO_X);
    float rateY = applyBetaflightActualRates(normalizedY, RC_RATE_Y, RATE_Y, EXPO_Y);

    // Scale rates to mouse movement
    float scaledDeltaX = rateX * MAX_DELTA / 1000.0f;
    float scaledDeltaY = rateY * MAX_DELTA / 1000.0f;

    // Apply smoothing
    smoothedDeltaX = SMOOTHING_FACTOR * scaledDeltaX + (1 - SMOOTHING_FACTOR) * smoothedDeltaX;
    smoothedDeltaY = SMOOTHING_FACTOR * scaledDeltaY + (1 - SMOOTHING_FACTOR) * smoothedDeltaY;

    // Move the mouse only if the change is above the drift threshold
    if (fabsf(smoothedDeltaX) > DRIFT_THRESHOLD || fabsf(smoothedDeltaY) > DRIFT_THRESHOLD) {
        if (bleMouse.isConnected()) {
            bleMouse.move(-smoothedDeltaX, smoothedDeltaY);
        }
    } else {
        // Reset smoothed values if below threshold to prevent accumulation
        smoothedDeltaX = 0;
        smoothedDeltaY = 0;
    }

    Serial.printf("Gyro: X=%.2f, Y=%.2f | Mouse: dX=%.2f, dY=%.2f\n", gyro.gyroX, gyro.gyroY, smoothedDeltaX, smoothedDeltaY);

    delay(UPDATE_INTERVAL);
}

void calibrateBaseline() {
    // ... (unchanged)
}
