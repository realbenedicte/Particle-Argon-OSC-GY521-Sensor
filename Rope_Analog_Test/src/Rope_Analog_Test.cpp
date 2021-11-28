/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/maximegordon/Desktop/ParticleMG/Rope_Analog_Test/src/Rope_Analog_Test.ino"
/*
 * Project Rope_Analog_Test
 * Description:
 * Author:
 * Date:
 */
/* PARTICLE CLOUD COMMUNICATION IN SEPARATE THREAD */
void setup();
void loop();
#line 8 "/Users/maximegordon/Desktop/ParticleMG/Rope_Analog_Test/src/Rope_Analog_Test.ino"
SYSTEM_THREAD(ENABLED);
/* CONTROL WiFi & INTERNET: AUTOMATIC, SEMI_AUTOMATIC, MANUAL */
SYSTEM_MODE(AUTOMATIC);
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
Serial.begin(9600);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
int stretchValue = analogRead(A0);
  // print out the value you read:
//  Serial.print(heartValue);
  Serial.print(" ");
  Serial.println(stretchValue);
  delay(1);        // delay in between reads for stability
}