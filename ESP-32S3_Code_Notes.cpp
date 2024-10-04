// USB CDC Enabled
// USB DFU Disabled
// JTAG USB DFU
// USB MSC Disabled
// Upload UART
// USB Mode OTG for serial UART for HID

#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"

USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

//took me three fucking days to figure that one out...
//Using ChatGPT o1 Preview, cursorAI running Claude3.5, ChatGPT 4o

// KbM-4 firmware 0.0.0.1
// by Riley Amber :) 
// ChatGPT 4o w/ "Code Copilot" 
// and a whole lot of arguing back and forth 
// Upload to ESP32S3 Supermini
// Target ESP32S3 Dev Module

// GPIO for WS2818 RGB LED (NeoPixel) DIN on GPIO 48
#define LED_PIN 48           // GPIO 48 for the WS2818 LED DIN
#define NUMPIXELS 1          // We are controlling just 1 LED
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

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
#define TRIGGER 40          
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
#define TRIGGER_OUTPUT MOUSE_LEFT  
#define MAG_RELEASE_OUTPUT MOUSE_RIGHT
#define BOLT_CATCH_OUTPUT 'b'
#define SELECTOR_L_SCROLL_UP 1
#define SELECTOR_L_SCROLL_DOWN -1
#define SELECTOR_R_LEFT KEY_LEFT_ARROW
#define SELECTOR_R_RIGHT KEY_RIGHT_ARROW

