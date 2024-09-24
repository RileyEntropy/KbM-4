//Pro Micro Pinout
//------------------------------- EDGE OF BOARD ----------------- VCC SIDE
const int SW1_10 = 10; // (A10/PWM) Thin-film pressure sensor Analog Pin
const int SW2_16 = 16; // (MOSI) Foregrip Switch 1
const int MAG_RELEASE= 14; //(MISO) "r"  Mag Release
const int TRIGGER = 15; // (SCLK)  "LMB" Trigger
const int PLUG_5P_1 =18 // (A0) Pin 1 (blue) on 5 pin Molex connector
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
