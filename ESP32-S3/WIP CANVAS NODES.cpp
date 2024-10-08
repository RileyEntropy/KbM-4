//==============================================  add wireless serial monitor for debugging
//add AP Mode for captive portal firmware configuration 
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WebServer.h>

//const char* ssid = "KBM4_Carbine";
//const char* password = "";

//============================================== add magnotimeter integration
// ensure that vertical and horizontal cursor movment remains on axis relitive to the position of the carbine
// ie if the carbine is tilted on the roll axis, but the user moves the barrel horizontally with no deflection
// the cursor will move horizontally with no deflection as well, as opposed to the unwanted behaviour of 
// the cursor moving diagonally due to the tilt on the roll axis 

//Consider adding a 3d accelerometer to measure the position of the carbine in 3d space
// Magnotimeter to measure the position of the carbine relative to the target
// gyro to measure the position of the carbine relative to the target

//============================================= Final Form Factor
// Ultimately the goal will be to fit everything into two modular parts:
//Modular foregrip
//modular trigger group/ half receiver with grip and removavle stock attachment

//============================================== add 2.4ghz wireless NRF24L01+
//implement low latency communication protocol for HID inputs
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

//RF24 radio(7, 8); // CE, CSN

//============================================== add ESP-NOW HUB/NODES
// Carbine acts  as a wireless HID bridge between multiple ESP NOW devices called "Nodes" such as:
//throwables, GLs/Launchers, NOD flip down and activation etc. 
//ESP Nodes should be a small and universal device that can be easily added to multiple weapons systems and pieces of equipment
//Nodes will communicate with the main carbine via ESP-NOW and will relay keyboard and mouse inputs to the computer
//The nodes should be very cheap, simple and easy to program, and should focus on being very reliable and low power consumption. 
//Main drawback of node would be slight laitency, may not be good for sidearm or melee weapon or shooty-shooty-bo-bang-bang
//but would be ideal for auxillary actions such as tuning on NODS, etc.
//For nodes that would be used as weapon systems, an IMU will be required. 
//This will allow for more advanced features such as: 
// - Using a Button Node (BNode) to select, on/off
// - Using Throwable node (TNode) to select, aim and release grenades
// - Using Launcher node (LNode) to select, aim and release AT/GL rounds

// - Possible hardware additions: recoil simulation
// - More accurate weapon aiming



//============================================== add I2C Expansion Accessories
//Wired Nodes would be refered to as expansions and should use 12C to communicate with the main carbine
//Explansions would be ideal for ingame deployment of lights and IR lasers or other devices that would benifit from being low laitency/hardwired 
//This may also be benifitial for Pump/Bolt Action/ Belt Fed integratons


//============================================== add hardware setting adjustments
//#include <EEPROM.h>
//Add bolt catch and charging handle buttons
//BC+CH = to turn off controller
//CH to power on controller if in off state
//CHx3 = Disable controller
//CHx1 = Enable controller

//Use bolt catch to act as settings/functions button
//LED will flash s//BC hold to enter settings mode
//BC HOLD: Settings Mode> BTN PRSS: Select Setting> 
//slowly in orange awaiting selection
// LED will flash slowly in orange awaiting selection
// User will press a control, such as trigger
// LED will change to a new color to represent the control
// LED will blink at the speed associated with the current setting value
// User will use 5d hat switch to increase or decrease, or cycle through the setting
// LED will blink fast or slow based on level of 
//5d hat switch 
