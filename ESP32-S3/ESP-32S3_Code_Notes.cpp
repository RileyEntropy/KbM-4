// USB CDC Enabled
// USB DFU Disabled
// JTAG USB DFU
// USB MSC Disabled
// Upload UART
// USB Mode OTG for serial UART for HID

// KbM-4 firmware 0.0.0.1
// by Riley Amber :) 
// Upload to ESP32S3 Supermini
// Target ESP32S3 Dev Module

#include <Wire.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include <MPU9250.h>
#include <math.h>
// #include <Adafruit_NeoPixel.h> // For controlling the WS2818 RGB (NeoPixel) LED (commented out)

// GPIO for WS2818 RGB LED (NeoPixel) DIN on GPIO 48
// #define LED_PIN 48           // GPIO 48 for the WS2818 LED DIN (commented out)
// #define NUMPIXELS 1          // We are controlling just 1 LED (commented out)
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800); // (commented out)

// GPIO Default Pins
#define GYRO_SDA 1
#define GYRO_SCL 2
#define OPTICAL_SDA 3
#define OPTICAL_SCL 4
#define FIVE_D_LEFT 5
#define FIVE_D_CENTER 6
#define FIVE_D_DOWN 7
#define FIVE_D_RIGHT 8
#define FIVE_D_UP 9
#define JOYSTICK_LR 10
#define JOYSTICK_UD 11
#define OLED_SDA 12
#define OLED_SCL 13
#define TRIGGER 40           // Changed to GPIO 40 for Trigger
#define MAG_RELEASE 47
#define SELECTOR_L 46
#define SELECTOR_R 45
#define BOLT_CATCH_RELEASE 42
#define ADS_TOUCH 41

// GPIO Default Outputs (Keyboard/Mouse Mappings)
#define FIVE_D_LEFT_OUTPUT 'p'
#define FIVE_D_CENTER_OUTPUT 'l'
#define FIVE_D_DOWN_OUTPUT 'o'
#define FIVE_D_RIGHT_OUTPUT 'k'
#define FIVE_D_UP_OUTPUT 'i'
#define JOYSTICK_LR_LEFT 'a'
#define JOYSTICK_LR_RIGHT 'd'
#define JOYSTICK_UD_UP 'w'
#define JOYSTICK_UD_DOWN 's'
#define TRIGGER_OUTPUT MOUSE_LEFT     // Trigger (on GPIO 40) = Left Mouse Button (LMB)
#define MAG_RELEASE_OUTPUT MOUSE_RIGHT
#define BOLT_CATCH_OUTPUT 'b'
// #define SELECTOR_L_SCROLL_UP 1       // (commented out)
// #define SELECTOR_L_SCROLL_DOWN -1    // (commented out)
// #define SELECTOR_R_LEFT KEY_LEFT_ARROW    // (commented out)
// #define SELECTOR_R_RIGHT KEY_RIGHT_ARROW  // (commented out)

// Create USB HID instances
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

// Store the previous state of each pin to detect changes
int gpioPins[] = {
  FIVE_D_LEFT, FIVE_D_CENTER, FIVE_D_DOWN, FIVE_D_RIGHT, FIVE_D_UP,
  TRIGGER, MAG_RELEASE, BOLT_CATCH_RELEASE
};
int previousStates[sizeof(gpioPins) / sizeof(gpioPins[0])];

// MPU9250 object
MPU9250 mpu;
bool mpuInitialized = false; // Flag to check if MPU is initialized

void setup() {
  // Start serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial to be ready
  }
  Serial.println("Serial initialized successfully");

  // Initialize USB HID
  Keyboard.begin();
  Mouse.begin();
  USB.begin();
  Serial.println("USB Mouse and Keyboard ready");

  // Initialize LED on GPIO 48 (commented out)
  // pixels.begin();
  // pixels.clear();  // Clear any previous LED data
  // pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Set LED to green initially
  // pixels.show();
  // Serial.println("NeoPixel initialized on GPIO 48 (DIN)");

  // Set up pin modes and initialize previous states
  for (int i = 0; i < sizeof(gpioPins) / sizeof(gpioPins[0]); i++) {
    pinMode(gpioPins[i], INPUT_PULLUP);
    previousStates[i] = HIGH; // Initialize all previous states to HIGH (not pressed)
  }

  // Initialize MPU9250
  Wire.begin();

  if (mpu.setup(0x68)) {
    mpuInitialized = true; // Set flag if MPU initializes successfully
    Serial.println("MPU9250 initialized successfully");
  } else {
    Serial.println("MPU9250 initialization failed!");
  }
  Serial.println("GPIO pins initialized, starting loop...");
}

void loop() {
  bool buttonPressed = false;

  // Handle keyboard and mouse inputs based on GPIO
  buttonPressed = handleKeyboardInputs();

  // If no button is pressed, you can add any default behavior here

  delay(50); // Short delay for debounce
}

bool handleKeyboardInputs() {
  bool anyButtonPressed = false;

  // Handle button inputs based on GPIO pins
  for (int i = 0; i < sizeof(gpioPins) / sizeof(gpioPins[0]); i++) {
    int currentState = digitalRead(gpioPins[i]);

    // Button pressed (transition from HIGH to LOW)
    if (currentState == LOW && previousStates[i] == HIGH) {
      Serial.print("GPIO ");
      Serial.print(gpioPins[i]);
      Serial.println(": Pressed (LOW)");

      // Uncomment to set LED color when button is pressed
      // setLEDColor(gpioPins[i]);

      // Trigger Keyboard/Mouse Presses for Specific Pins
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
          Mouse.press(TRIGGER_OUTPUT);  // Left Mouse Button (LMB) on GPIO 40
          break;
        case MAG_RELEASE:
          Mouse.press(MAG_RELEASE_OUTPUT);  // Right Mouse Button (RMB)
          break;
        case BOLT_CATCH_RELEASE:
          Keyboard.press(BOLT_CATCH_OUTPUT);  // 'b' keypress
          break;
        default:
          break;
      }

      anyButtonPressed = true;
      previousStates[i] = LOW; // Update state to LOW (pressed)
    }
    // Button released (transition from LOW to HIGH)
    else if (currentState == HIGH && previousStates[i] == LOW) {
      // Release the button when it goes back to HIGH
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
          Mouse.release(TRIGGER_OUTPUT);  // Release Left Mouse Button
          break;
        case MAG_RELEASE:
          Mouse.release(MAG_RELEASE_OUTPUT);  // Release Right Mouse Button
          break;
        case BOLT_CATCH_RELEASE:
          Keyboard.release(BOLT_CATCH_OUTPUT);  // Release 'b' keypress
          break;
        default:
          break;
      }

      previousStates[i] = HIGH; // Update state back to HIGH when released
    }
  }

  return anyButtonPressed;
}

// Uncomment the following function if you want to use the NeoPixel LED
/*
void setLEDColor(int gpioPin) {
  // Function body to set LED color based on gpioPi
  
