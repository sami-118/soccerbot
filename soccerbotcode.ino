#include <IBusBM.h>
 
// Create iBus Object
IBusBM ibus;
 
// Channel Values
 
int rcCH1 = 0; // Left - Right
int rcCH2 = 0; // Forward - Reverse
int rcCH3 = 0; // Acceleration
int rcCH5 = 0; // Spin Control
bool rcCH6 = 0; // Mode Control
 
// Motor A Control Connections
#define r_en 12
#define l_en 9
#define r_pwm 13
#define l_pwm 8
 
// Motor B Control Connections
#define r_en1 6
#define l_en1 3
#define r_pwm1 7
#define l_pwm1 2

 
// Motor Speed Values - Start at zero
int MotorSpeedA = 0;
int MotorSpeedB = 0;
 
// Motor Direction Values - 0 = backward, 1 = forward
int MotorDirA = 1;
int MotorDirB = 1;
 
// Control Motor A
void mControlA(int mspeed, int mdir) {
    digitalWrite(r_en1, HIGH);
  digitalWrite(l_en1, HIGH);

  if (mdir == 0) {
    // Motor backward
  // Control motor                                                                                                                                                                                                                                                                                                          
  analogWrite(r_pwm1, mspeed);
  analogWrite(l_pwm1, 0);
  } else {
    // Motor forward
      analogWrite(r_pwm1, 0);
  analogWrite(l_pwm1, mspeed);
  }
 
 
}
 
// Control Motor B
void mControlB(int mspeed, int mdir) {
    //To enable forward and backward
digitalWrite(r_en, HIGH);
  digitalWrite(l_en, HIGH);
  // Determine direction
   if (mdir == 0) {
      
    // Motor backward
  // Control motor
  analogWrite(r_pwm, mspeed);
  analogWrite(l_pwm, 0);
  } else {
    // Motor forward
      analogWrite(r_pwm, 0);
  analogWrite(l_pwm, mspeed);
  }
 

 
}
 
// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
 
// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}
 
void setup()
 
{
 
  // Start serial monitor for debugging
  Serial.begin(115200);
 
  // Attach iBus object to serial port
  ibus.begin(Serial1);
 
  // Set all the motor control pins to outputs
 
  pinMode(r_en, OUTPUT);
  pinMode(l_en, OUTPUT);
  pinMode(r_pwm, OUTPUT);
  pinMode(l_pwm, OUTPUT);
 pinMode(r_en1, OUTPUT);
  pinMode(l_en1, OUTPUT);
  pinMode(r_pwm1, OUTPUT);
  pinMode(l_pwm1, OUTPUT);


 
}
 
void loop() {
 
  // Get RC channel values
  rcCH1 = readChannel(0, -100, 100, 0);
  rcCH2 = readChannel(1, -100, 100, 0);
  rcCH3 = readChannel(2, 0, 155, 0);
  rcCH5 = readChannel(4, -100, 100, 0);
  rcCH6 = readSwitch(5, false);
 
  // Print values to serial monitor for debugging
  Serial.print("Ch1 = ");
  Serial.print(rcCH1);
 
  Serial.print(" Ch2 = ");
  Serial.print(rcCH2);
 
  Serial.print(" Ch3 = ");
  Serial.print(rcCH3);
 
  Serial.print(" Ch5 = ");
  Serial.print(rcCH5);
 
  Serial.print(" Ch6 = ");
  Serial.println(rcCH6);
 
  // Set speeds with channel 3 value
  MotorSpeedA = rcCH3;
  MotorSpeedB = rcCH3;
 
  // Set Mode with channel 6 value
  if (rcCH6) {
    // Spin Mode
 

    // Get Direction from channel 5 value
    if (rcCH5 >= 0) {
      //Clockwise
      MotorDirA = 0;
      MotorDirB = 1;
      Serial.println("Clockwise");
    } else {
      //Counter-Clockwise
      MotorDirA = 1;
      MotorDirB = 0;
      Serial.println("Counter-Clockwise");
    }
 
    // Add Motorspeed to channel 5 value
    MotorSpeedA = MotorSpeedA + abs(rcCH5);
    MotorSpeedB = MotorSpeedB + abs(rcCH5);
 
  } else {
    
    // Set forward/backward direction with channel 2 value
    if (rcCH2 >= 0) {
      //Forward
      MotorDirA = 1;
      MotorDirB = 1;
      Serial.println("Forward");
    } else {
      //Backward
      MotorDirA = 0;
      MotorDirB = 0;
      Serial.println("Backward");
    }
 
    // Add channel 2 speed
    MotorSpeedA = MotorSpeedA + abs(rcCH2);
    MotorSpeedB = MotorSpeedB + abs(rcCH2);
 
    // Set left/right offset with channel 1 value
    MotorSpeedA = MotorSpeedA - rcCH1;
    MotorSpeedB = MotorSpeedB + rcCH1;
 
  }
 
  // Ensure that speeds are between 0 and 255
  MotorSpeedA = constrain(MotorSpeedA, 0, 255);
  MotorSpeedB = constrain(MotorSpeedB, 0, 255);
 
  //Drive Motors
  mControlA(MotorSpeedA, MotorDirA);
  mControlB(MotorSpeedB, MotorDirB);
 
  // Print speed values to serial monitor for debugging
  Serial.print("Motor A Speed = ");
  Serial.print(MotorSpeedA);
  Serial.print(" | Motor B Speed = ");
  Serial.println(MotorSpeedB);
delay(50);
 
}