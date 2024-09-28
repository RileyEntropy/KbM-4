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


![KbM-PCCv433443](https://github.com/user-attachments/assets/34bc65c5-8de1-45f4-876d-6c6494b32a2d)
![KbM-PCC v19](https://github.com/user-attachments/assets/7502686d-dec9-4849-b173-c402479f51a9)
![KbM-PCC v192](https://github.com/user-attachments/assets/b215e9c1-c8ad-4cb6-95c8-6f801adbf45d)
WebUI Configurator Example
![Screenshot 2024-09-27 144149](https://github.com/user-attachments/assets/c6710a22-6b89-464d-8bdf-e41195aa0227)
![Screenshot 2024-09-27 144138](https://github.com/user-attachments/assets/88090bb4-260a-4af3-81f8-db22db2a80d3)

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

ESP-32-S3 Dev Kit (UNTESTED HID MCU)
https://a.co/d/4kGZ8nL

![R (5)](https://github.com/user-attachments/assets/2864237d-043f-48c7-a45a-ef3ba21ce350)

Pro Micro Board ATmega32U4 Leonardo 5V/16MHz Module Board Type C (ALTERNATE HID MCU)
https://a.co/d/3GjWkje

![71PDKi3EgiL _AC_SL1500_](https://github.com/user-attachments/assets/d724f343-c59d-4d7f-8d5b-c001146a0ebc)

Kailh Choc V2 Low Profile Switch Blue Clicky, 4-Pin 50gf RGB SMD Gaming Mechanical Keyboard Switch (RED TESTED)
https://a.co/d/78XaSLT

![612L5qs+4xL _SL1500_](https://github.com/user-attachments/assets/bd297e81-d4bf-4e4c-bcbd-33c29e65cbbc)

Kailh Hot-swap PCB Socket CPG151101S11
https://a.co/d/eFxBCiE

![71HhVmByGHL _AC_SL1500_](https://github.com/user-attachments/assets/5c50026e-387a-4387-b186-06b04ec8e7b6)

Kailh GM8.0 Micro Switch (UNTESTED, NEEDS PARTS DESIGN)
https://a.co/d/g16Y8mO

![61dG6Rh-S3L _AC_SL1500_](https://github.com/user-attachments/assets/2622c3d3-63d9-4ad9-b472-2d5af0ef5377)

GY-MPU9250 9 Axis Gyroscope/Magnetometer (TESTED, WORKING. INTEGRATE MAG)
https://a.co/d/3rONOaM

![51bMyPpej4L](https://github.com/user-attachments/assets/b244d416-54c5-4a9b-a65d-07f3e46f5b53)

9mm Mouse Wheel Encoder (UNTESETD, NEEDS NEW PRINTS/CAD UPDATE)
https://a.co/d/gq9IQqk

![61nDSCEeuxL _AC_SL200_](https://github.com/user-attachments/assets/221c83db-4ba1-4406-b032-a0d91cfa81c0)

PSP 1000 Replacment Analog Joystick (UNTESTED, NEED CAD AND PARTS DESIGN)
https://a.co/d/bAqEFN5

![61yaEL76dML _AC_SL1500_](https://github.com/user-attachments/assets/eabf3b32-2ad7-4e88-97eb-fad86f1760d5)
![IMG_0681-2](https://github.com/user-attachments/assets/09190d37-3962-405c-860a-a4e1473c717b)



10x10x9mm 6 Pin 5 Way Momentary SMT Tactile Switch
https://a.co/d/gYcASHC

![31OAXA+rCgL _AC_](https://github.com/user-attachments/assets/7c50d6ed-7385-4c55-b4cd-bf645dbbfe23)

Thin Film Capacitive Pressure Sensor
https://a.co/d/25u2H3w

![51NCF0LNl9L _SL1500_](https://github.com/user-attachments/assets/021fb3ac-19c7-4640-9cbc-5a0493f375bc)

Blackberry Trackball Module
https://a.co/d/2LdUVgh

![61lx3oXyw9L _SL200_](https://github.com/user-attachments/assets/11227fcc-8df0-4109-ba00-54f3d92ca870)

1.3 Inch I2C OLED Display Module 128x64 Pixel SH1106 Screen Module
https://a.co/d/2yZsSMA

![61i9iP73FmL _AC_SL1200_](https://github.com/user-attachments/assets/062c0c48-f493-4d27-bb19-4e2abdf85515)


TCA9548A I2C  8 Channel Multiplexer Expansion Breakout Board
https://a.co/d/4ZwDTRi

![61w4KYkAFdL _SL1000_](https://github.com/user-attachments/assets/b8593109-225b-4350-ac27-a07111813c07)

Thin Film Pressure Sensor Strip
https://a.co/d/13WvZUT

![61tQLWFO8zL _SL200_](https://github.com/user-attachments/assets/ab1c51a3-e5e6-4910-99ae-2e8c501f3b59)

