#include <Keyboard.h>
#include <Mouse.h>
#include <math.h>

// Button pin definitions
const int LEFT_MOUSE_PIN = 15;
const int MIDDLE_MOUSE_PIN = 6;
const int R_KEY_PIN = 14;
const int SHIFT_PIN = 16;
const int SPACEBAR_PIN = 10;
const int LED_PIN = 13;
const int SPEED_REDUCE_PIN = 9;

// Joystick pin definitions
const int xPin1 = A2;
const int yPin1 = A1;
const int xPin2 = A3;
const int yPin2 = A0;

// Deadzone configuration
int deadZoneX = 30;
int deadZoneY = 30;
int deadZoneX2 = 10;
int deadZoneY2 = 10;

// Mouse movement parameters
const float SENSITIVITY = 1.3 * 0.8 * 3.0;
const float MAX_RATE_X = 0.7;
const float MAX_RATE_Y = 0.2;
const float EXPO = 0.5;
const float MAX_VELOCITY = 50.0;
const float DIAGONAL_SMOOTHING = 0.5;
const float MMB_SPEED_MULTIPLIER = 0.6;
const float SHIFT_SPEED_MULTIPLIER = 1.0 / 3.0;
const float SPEED_REDUCE_MULTIPLIER = 0.5;

// Calibration variables
int xCenter2 = 512;
int yCenter2 = 512;

// Button state variables
bool lastLeftMouseState = HIGH;
bool lastMiddleMouseState = HIGH;
bool lastRKeyState = HIGH;
bool lastShiftState = HIGH;
bool lastSpacebarState = HIGH;

// WASD state variables
bool wPressed = false;
bool aPressed = false;
bool sPressed = false;
bool dPressed = false;

// Mouse button states
bool leftMousePressed = false;
bool middleMousePressed = false;

// Speed reduce button state
bool speedReducePressed = false;

// Shift key state
bool shiftPressed = false;

// Add custom defines for inversion
#define INVERT_A_D 0  // Set to 1 to invert A/D movement
#define INVERT_W_S 0  // Set to 1 to invert W/S movement

void setup() {
  Keyboard.begin();
  Mouse.begin();
  Serial.begin(9600);

  pinMode(LEFT_MOUSE_PIN, INPUT_PULLUP);
  pinMode(MIDDLE_MOUSE_PIN, INPUT_PULLUP);
  pinMode(R_KEY_PIN, INPUT_PULLUP);
  pinMode(SHIFT_PIN, INPUT_PULLUP);
  pinMode(SPACEBAR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEED_REDUCE_PIN, INPUT_PULLUP);

  calibrateJoystick();
}

void loop() {
  mapJoystick1ToWASD();
  checkSpeedReduceButton();
  mapJoystick2ToMouse();
  checkButtonStates();
  delay(10);
}

void calibrateJoystick() {
  long xSum = 0, ySum = 0;
  const int samples = 64;
  for (int i = 0; i < samples; i++) {
    xSum += analogRead(xPin2);
    ySum += analogRead(yPin2);
    delay(10);
  }
  xCenter2 = xSum / samples;
  yCenter2 = ySum / samples;
}

void mapJoystick1ToWASD() {
  int xValue = analogRead(xPin1);
  int yValue = analogRead(yPin1);

  // S key (swapped with W)
  if (yValue < 512 - deadZoneY && !sPressed) {
    Keyboard.press('s');
    sPressed = true;
  } else if (yValue >= 512 - deadZoneY && sPressed) {
    Keyboard.release('s');
    sPressed = false;
  }

  // W key (swapped with S)
  if (yValue > 512 + deadZoneY && !wPressed) {
    Keyboard.press('w');
    wPressed = true;
  } else if (yValue <= 512 + deadZoneY && wPressed) {
    Keyboard.release('w');
    wPressed = false;
  }

  // Swap A and D key mapping
  // D key (now on the left)
  if (xValue < 512 - deadZoneX && !dPressed) {
    Keyboard.press('d');
    dPressed = true;
  } else if (xValue >= 512 - deadZoneX && dPressed) {
    Keyboard.release('d');
    dPressed = false;
  }

  // A key (now on the right)
  if (xValue > 512 + deadZoneX && !aPressed) {
    Keyboard.press('a');
    aPressed = true;
  } else if (xValue <= 512 + deadZoneX && aPressed) {
    Keyboard.release('a');
    aPressed = false;
  }
}


float applyExpo(float input) {
  return input * (EXPO * input * input + (1 - EXPO));
}

void smoothDiagonal(float &x, float &y) {
  float magnitude = sqrt(x * x + y * y);
  if (magnitude > 0) {
    float ratio = min(abs(x), abs(y)) / magnitude;
    float smoothing = DIAGONAL_SMOOTHING * (1 - ratio);
    x *= (1 - smoothing);
    y *= (1 - smoothing);
  }
}

void mapJoystick2ToMouse() {
  int xValue = analogRead(xPin2);
  int yValue = analogRead(yPin2);

  float xDist = (xValue - xCenter2) / 512.0;
  float yDist = (yValue - yCenter2) / 512.0;

  if (abs(xDist) < deadZoneX2 / 512.0) xDist = 0;
  if (abs(yDist) < deadZoneY2 / 512.0) yDist = 0;

  float xMove, yMove;
  if (middleMousePressed && leftMousePressed) {
    xMove = xDist;
    yMove = yDist;
  } else if (middleMousePressed) {
    xMove = xDist;  // No expo when MMB is pressed
    yMove = yDist;
  } else {
    xMove = applyExpo(xDist);
    yMove = applyExpo(yDist);
  }

  smoothDiagonal(xMove, yMove);

  float currentSensitivity = SENSITIVITY;
  if (middleMousePressed && leftMousePressed) {
    // No modifiers when both MMB and LMB are pressed
  } else if (middleMousePressed) {
    currentSensitivity *= MMB_SPEED_MULTIPLIER;
  } else if (shiftPressed) {
    currentSensitivity *= SHIFT_SPEED_MULTIPLIER;
  }

  if (speedReducePressed && !(middleMousePressed && leftMousePressed)) {
    currentSensitivity *= SPEED_REDUCE_MULTIPLIER;
  }

  xMove *= currentSensitivity * MAX_RATE_X * MAX_VELOCITY;
  yMove *= currentSensitivity * MAX_RATE_Y * MAX_VELOCITY;

  // Apply max velocity limit
  float magnitude = sqrt(xMove * xMove + yMove * yMove);
  if (magnitude > MAX_VELOCITY) {
    float scale = MAX_VELOCITY / magnitude;
    xMove *= scale;
    yMove *= scale;
  }

  if (abs(xMove) > 0.1 || abs(yMove) > 0.1) {
    // Move the cursor. X is normal, Y is inverted
    Mouse.move(int(xMove), int(-yMove), 0);
  }
}

void checkButtonStates() {
  // Left Mouse Button
  bool currentLeftMouseState = digitalRead(LEFT_MOUSE_PIN);
  if (currentLeftMouseState != lastLeftMouseState) {
    if (currentLeftMouseState == LOW) {
      Mouse.press(MOUSE_LEFT);
      leftMousePressed = true;
    } else {
      Mouse.release(MOUSE_LEFT);
      leftMousePressed = false;
    }
    lastLeftMouseState = currentLeftMouseState;
  }

  // Middle Mouse Button
  bool currentMiddleMouseState = digitalRead(MIDDLE_MOUSE_PIN);
  if (currentMiddleMouseState != lastMiddleMouseState) {
    if (currentMiddleMouseState == LOW) {
      Mouse.press(MOUSE_MIDDLE);
      middleMousePressed = true;
    } else {
      Mouse.release(MOUSE_MIDDLE);
      middleMousePressed = false;
    }
    lastMiddleMouseState = currentMiddleMouseState;
  }

  // R Key
  bool currentRKeyState = digitalRead(R_KEY_PIN);
  if (currentRKeyState != lastRKeyState) {
    if (currentRKeyState == LOW) {
      Keyboard.press('r');
    } else {
      Keyboard.release('r');
    }
    lastRKeyState = currentRKeyState;
  }

  // Shift Key
  bool currentShiftState = digitalRead(SHIFT_PIN);
  if (currentShiftState != lastShiftState) {
    if (currentShiftState == LOW) {
      Keyboard.press(KEY_LEFT_SHIFT);
      shiftPressed = true;
    } else {
      Keyboard.release(KEY_LEFT_SHIFT);
      shiftPressed = false;
    }
    lastShiftState = currentShiftState;
  }

  // Spacebar
  bool currentSpacebarState = digitalRead(SPACEBAR_PIN);
  if (currentSpacebarState != lastSpacebarState) {
    if (currentSpacebarState == LOW) {
      Keyboard.press(' ');
    } else {
      Keyboard.release(' ');
    }
    lastSpacebarState = currentSpacebarState;
  }
}

void checkSpeedReduceButton() {
  speedReducePressed = digitalRead(SPEED_REDUCE_PIN) == LOW;

// Optional: You can add some debug prints to monitor the state of the button
  if (speedReducePressed) {
    Serial.println("Speed reduce button pressed.");
  }
}
