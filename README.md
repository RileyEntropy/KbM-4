# KbM-4
The KbM-4 (KeyBoard/Mouse- M4) is a highly modular, highly configurable and highly upgradable controller based on the AR-15 platform. It is compatible with many AR accessories and attachments such as optics, stocks, grips, etc. It also utilizes common custom keyboard and mouse switches, such as Kailh Choc V2s, and GM8.0 switches, but can be replaced with any switch of a similar footprint. 

AR-15 spec placement of fire controls
AR-15 compatible buffer tube (Mil-Spec threading) and grip (M5 bolt)
Full Upper Rail compatible with RIS accessories.
Sturdy 2020 profile aluminum extrusions are compatible with M-Lok accessories (using M5 bolts and 2020 Profile M5 Nuts)

Up to 40 programmable switch/key inputs
Adjustable Hall-Effect Trigger. Code adjustable trigger break and pretravel
Mechanically adjustable trigger weight and overtravel
Low Profile Kailh switch compatible Magazine Release
Low Profile Kailh foregrip switches
Capacitive Touch Strip cheek weld switch

9mm Rotary Encoder Scroll wheel selector switch.
Muzzle device rotary encoder

Gyro aiming and Peek/Lean
DPI and Gyro sensitivity adjustable via mechanical knobs on sights.

Holographic or Ejection Port mounted OLED display



Open Source Hardware and Firmware for limitless customizations and modifications for a fully open ecosystem. Possible QMK firmware planned in the future.
![KbMPx v21 1](https://github.com/user-attachments/assets/8e4f258f-be59-4979-b7c2-9054f3af653d)
![KbMPx v21](https://github.com/user-attachments/assets/a2594e8f-1f6e-4c10-bafa-147ca9359f00)
![KbMPx_2024-Nov-01_08-02-45PM-000_CustomizedView29259553379](https://github.com/user-attachments/assets/44f9d7b5-8dce-42a8-bb6f-506b6ca83ab1)
![KbMPx_2024-Nov-01_08-03-40PM-000_CustomizedView11831597286](https://github.com/user-attachments/assets/91813a00-2b95-46e7-8895-e0742da34def)
![KbMPx_2024-Nov-01_09-48-59AM-000_CustomizedView13123542985](https://github.com/user-attachments/assets/6311cddb-fd21-43ab-b087-781f9720c3d9)
![KbMPx v42MARS](https://github.com/user-attachments/assets/c5686ca2-6ba8-45bf-8e1b-a2825d4c4c72)
![KbMPx v42MARS2](https://github.com/user-attachments/assets/91e3b104-f358-44ab-955c-981d449c77c3)
![KbMPx_2024-Nov-16_01-25-42AM-000_CustomizedView423223413](https://github.com/user-attachments/assets/ccf72de7-96e5-44d2-8568-612bd129ad49)
![KbMPx_2024-Nov-01_09-51-20AM-000_CustomizedView8402114409](https://github.com/user-attachments/assets/37a3a35e-4eed-4606-9141-38658e7802bc)


![KbMPx v11345](https://github.com/user-attachments/assets/c664e908-ea09-498c-8d15-50025cf3fad6)
![KbMPx v1134](https://github.com/user-attachments/assets/20b67eed-07ed-425b-8081-921cfcc6fdca)

WebUI Configurator Example
![Screenshot 2024-09-27 144149](https://github.com/user-attachments/assets/c6710a22-6b89-464d-8bdf-e41195aa0227)
![Screenshot 2024-09-27 144138](https://github.com/user-attachments/assets/88090bb4-260a-4af3-81f8-db22db2a80d3)

Time for 4am ramblings:
But why tho?
Sim racing rigs have steering wheels, flight sim rigs have throttle and stick, but shooter games? I would say at best the options, when available, are limited, overpriced and feel like big box store toy guns. What if there was a controller, built by shooters, for shooters? Not just in FPS games, but people with real time behind the rifle. A controller worthy of Chris Costa, Travis Haley, and Papa Thumb. A controller where you can smoke some kids online after a long range session and have some of that muscle memory actually transfer back and forth? 

Maybe it’s just me, but the first time I reloaded in a PC shooter game, using the actual magazine release button on my controller, with correct AR-15 placement, it just hits different… 

I did a deep dive of research into aiming mechanics and mouse ergonomics. The firearms industry has not only had the competitive incentive to make ergonomics and weapons handling both an art and science, but also the necessity of survival. Over the last two decades, the study of firearms manipulation has made great strides, but controllers and mice are the tactical equivalent of 80’s SWAT movies. 

Availability: A platform, not a product
Open source. Easy to build on your own. No kickstarter, no waitlist, no back order, no overhyped broken promises. Buy it or build it, and if there’s anything you don’t like about it, you can change it with pretty minimal effort. 

Realism: in expectations
I don’t think the KbM-4 will revolutionize the gaming industry, or even be the right answer for most people. For those who want a more realistic shooting experience while still being able to keep up with optical mice, this might be a superior option. Of course everything has a learning curve, this is no exception. While it is built to be intuitive, it will take some getting used to. But once it clicks and you can get into the flow it feels right for those with time behind the trigger.

Performance wise, the goal is to match the gold standard of the aiming capability of optical gaming mice. To accomplish this, one or more IMU's are used for gyro control, which is nothing new as many gaming controllers now feature gyro aiming. I have tested FPV gimbals, analog joysticks, trackpads, and even drawing tablets in pursuit of a proper match for IMU aiming.
Realistic and intuitive

ESP-32-S3 Dev Kit

![80d21868184b4678d52fb7005db5317ee075ab77](https://github.com/user-attachments/assets/474f419e-a822-45d9-9ca5-12ba9c21a866)
![OIP (5)](https://github.com/user-attachments/assets/60c41aa9-2a9f-48a2-979b-5abcffac2c84)

// Working COnfig to get USB HID and Serial Working
//took 3 fucking days to figure out
// USB CDC Enabled
// USB DFU Disabled
// JTAG USB DFU
// USB MSC Disabled
// Uploade UART
// USB Mode OTG
#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"

Kailh Choc V2 Low Profile Switch Blue Clicky, 4-Pin 50gf RGB SMD Gaming Mechanical Keyboard Switch (RED TESTED)
https://a.co/d/78XaSLT

![612L5qs+4xL _SL1500_](https://github.com/user-attachments/assets/bd297e81-d4bf-4e4c-bcbd-33c29e65cbbc)
![202004151351066711915064c54f7d9dba66f572e3baff](https://github.com/user-attachments/assets/f425ae15-a0d9-4d53-a941-89cb40b58a53)

Kailh Hot-swap PCB Socket CPG151101S11
https://a.co/d/eFxBCiE

![71HhVmByGHL _AC_SL1500_](https://github.com/user-attachments/assets/5c50026e-387a-4387-b186-06b04ec8e7b6)

Kailh GM8.0 Micro Switch (UNTESTED, NEEDS PARTS DESIGN)
https://a.co/d/g16Y8mO

![61dG6Rh-S3L _AC_SL1500_](https://github.com/user-attachments/assets/2622c3d3-63d9-4ad9-b472-2d5af0ef5377)
![GM-8 0-BLACK-MAMBA-Switch-dimension](https://github.com/user-attachments/assets/14d36f32-ef45-4fcb-9441-894b98ab41f6)

GY-MPU9250 9 Axis Gyroscope/Magnetometer (TESTED, WORKING. INTEGRATE MAG) VDD 2.4V to 3.6V
https://a.co/d/3rONOaM

![51bMyPpej4L](https://github.com/user-attachments/assets/b244d416-54c5-4a9b-a65d-07f3e46f5b53)

9mm Mouse Wheel Encoder (UNTESETD, NEEDS NEW PRINTS/CAD UPDATE)
https://a.co/d/gq9IQqk

![61nDSCEeuxL _AC_SL200_](https://github.com/user-attachments/assets/221c83db-4ba1-4406-b032-a0d91cfa81c0)
![201903121040238222735](https://github.com/user-attachments/assets/8f91e286-f98c-4e87-8ef3-dc67f7848413)

PSP 1000 Replacment Analog Joystick (UNTESTED, NEED CAD AND PARTS DESIGN) 3.3v to 5v
https://a.co/d/bAqEFN5

![61yaEL76dML _AC_SL1500_](https://github.com/user-attachments/assets/eabf3b32-2ad7-4e88-97eb-fad86f1760d5)
![IMG_0681-2](https://github.com/user-attachments/assets/09190d37-3962-405c-860a-a4e1473c717b)


10x10x9mm 6 Pin 5 Way Momentary SMT Tactile Switch
https://a.co/d/gYcASHC

![31OAXA+rCgL _AC_](https://github.com/user-attachments/assets/7c50d6ed-7385-4c55-b4cd-bf645dbbfe23)
![raspberry_pi_5way-schematic](https://github.com/user-attachments/assets/6f133512-cb0a-475a-823d-e3cd39a87f00)
![a4ffbdc67daf6e641a9c50c5940088a4142fa074](https://github.com/user-attachments/assets/1cdb554c-0504-42f7-8447-ad9f568c7df3)

1.3 Inch I2C OLED Display Module 128x64 Pixel SH1106 Screen Module (3V~5V)
https://a.co/d/2yZsSMA
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#define OLED_SDA_PIN 12
#define OLED_SCL_PIN 13
#define OLED_ADDRESS 0x3C

![61i9iP73FmL _AC_SL1200_](https://github.com/user-attachments/assets/062c0c48-f493-4d27-bb19-4e2abdf85515)


TCA9548A I2C  8 Channel Multiplexer Expansion Breakout Board
https://a.co/d/4ZwDTRi

![61w4KYkAFdL _SL1000_](https://github.com/user-attachments/assets/b8593109-225b-4350-ac27-a07111813c07)

