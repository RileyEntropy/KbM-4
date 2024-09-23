
//To implement absolute positioning for the roll axis (X-axis), ensuring the controller maintains Q or E pressed until the controller tilts back to its neutral position, we need to adjust how the roll angle is calculated and how the keypress is managed.

//Desired Behavior:
//Roll Left (past +10 degrees): Hold Q until the controller returns to a neutral (0-degree) position.
//Roll Right (past -10 degrees): Hold E until the controller returns to neutral.
//Avoid immediate toggling between Q and E when crossing the 0-degree point.
//We'll track the absolute position of the roll axis and implement logic to hold the key until the tilt moves back toward 0 degrees.

//Solution:
//Track the roll angle (X-axis) and compare it to 10 degrees for Q (left tilt) and -10 degrees for E (right tilt).
// flags to track whether Q or E is being held, and ensure they aren't toggled too quickly
