#include <MPU9250.h>
#include <HID-Project.h>
#include <Wire.h>
#include <math.h>

// MPU9250 object
MPU9250 mpu;

// Response Curve parameters
#define RC_RATE_X 2.0f  // Range: 0.01-2.55 (for X-axis, vertical movement) default 2.0
#define RC_RATE_Z 2.0f  // Range: 0.01-2.55 (for Z-axis, horizontal movement) defualt 2.0
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
    Serial.println("USB Mouse ready");

    calibrateBaseline();
}

void loop() {
    if (mpu.update()) {
        float gyroX = mpu.getGyroX();  // X-axis for vertical movement
        float gyroY = mpu.getGyroY();  // Y-axis for rate multiplier
        float gyroZ = mpu.getGyroZ();  // Z-axis for horizontal movement

        float deltaX = gyroX - baselineX;
        float deltaY = gyroY - baselineY;
        float deltaZ = gyroZ - baselineZ;

        // Normalize gyro input to -1 to 1 range (assuming max gyro reading of 2000 deg/s)
        float normalizedX = constrain(deltaX / 2000.0f, -1.0f, 1.0f);
        float normalizedZ = constrain(deltaZ / 2000.0f, -1.0f, 1.0f);

        // Calculate rate multiplier based on Y-axis
        float rateMultiplier = 1.0f + (fabsf(deltaY) / 2000.0f) * RATE_MULTIPLIER_FACTOR;

        // Apply Response Curve
        float rateX = applyResponseCurve(normalizedX, RC_RATE_X, RATE_X, EXPO_X) * rateMultiplier;
        float rateZ = applyResponseCurve(normalizedZ, RC_RATE_Z, RATE_Z, EXPO_Z) * rateMultiplier;

        // Scale the rates to mouse movement
        float scaledDeltaX = rateX * MAX_DELTA / 1000.0f;
        float scaledDeltaZ = rateZ * MAX_DELTA / 1000.0f;

        // Apply smoothing
        smoothedDeltaX = SMOOTHING_FACTOR * scaledDeltaX + (1 - SMOOTHING_FACTOR) * smoothedDeltaX;
        smoothedDeltaZ = SMOOTHING_FACTOR * scaledDeltaZ + (1 - SMOOTHING_FACTOR) * smoothedDeltaZ;

        // Apply master multiplier
        smoothedDeltaX *= MASTER_MULTIPLIER;
        smoothedDeltaZ *= MASTER_MULTIPLIER;

        // Move the mouse if above drift threshold
        if (fabsf(smoothedDeltaX) > DRIFT_THRESHOLD || fabsf(smoothedDeltaZ) > DRIFT_THRESHOLD) {
            Mouse.move(-smoothedDeltaZ, smoothedDeltaX);  // Invert Z for horizontal, don't invert X for vertical
        } else {
            smoothedDeltaX = 0;
            smoothedDeltaZ = 0;
        }

        // Debug output
        Serial.print("Gyro: X=");
        Serial.print(gyroX, 2);
        Serial.print(", Y=");
        Serial.print(gyroY, 2);
        Serial.print(", Z=");
        Serial.print(gyroZ, 2);
        Serial.print(" | Mouse: dX=");
        Serial.print(-smoothedDeltaZ, 2);
        Serial.print(", dY=");
        Serial.print(smoothedDeltaX, 2);
        Serial.print(" | Multiplier: ");
        Serial.println(rateMultiplier, 2);
    }

    delay(UPDATE_INTERVAL);
}

void calibrateBaseline() {
    const int numSamples = 1000;
    float sumX = 0, sumY = 0, sumZ = 0;

    for (int i = 0; i < numSamples; i++) {
        if (mpu.update()) {
            sumX += mpu.getGyroX();
            sumY += mpu.getGyroY();
            sumZ += mpu.getGyroZ();
        }
        delay(1);
    }

    baselineX = sumX / numSamples;
    baselineY = sumY / numSamples;
    baselineZ = sumZ / numSamples;

    Serial.print("Baseline calibrated: X=");
    Serial.print(baselineX, 2);
    Serial.print(", Y=");
    Serial.print(baselineY, 2);
    Serial.print(", Z=");
    Serial.println(baselineZ, 2);
}
