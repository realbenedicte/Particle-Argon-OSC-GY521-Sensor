/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/maximegordon/Desktop/being-with/ParticleMG/sarah-harness/src/sarah-harness.ino"
//Sarah Harness Accelerometer Code
//AddressName: sarah
//PortNumber: 2233 
//uses GY521 sensor

//REFERENCES:
// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
// https://github.com/pkourany/I2CDEV_MPU6050
// https://github.com/CEEESSLAB/CART461-VYV

/* PARTICLE CLOUD COMMUNICATION IN SEPARATE THREAD */
void setup();
void loop();
#line 12 "/Users/maximegordon/Desktop/being-with/ParticleMG/sarah-harness/src/sarah-harness.ino"
SYSTEM_THREAD(ENABLED);
/* CONTROL WiFi & INTERNET: AUTOMATIC, SEMI_AUTOMATIC, MANUAL */
SYSTEM_MODE(AUTOMATIC); //Automatic for automatic connection to wifi

#include "simple-OSC.h" //osc library
#include "math.h" //math library
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include <I2Cdev.h> //I2c library
#include <MPU6050.h> //sensor library

//setting up udp for communication with Max
UDP udp;
IPAddress outIp(192, 168, 1, 106); //your computer IP//Need to reconfigure for each new wifi network ! 
unsigned int outPort = 2233; //computer port -- use this in Max 

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE) && !defined (PARTICLE)
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
int16_t ax, ay, az;
int16_t gx, gy, gz;

float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;

#if defined (PARTICLE)
#define LED_PIN D7 // (Particle is D7)
#else
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
#endif

bool blinkState = false;

//setup()
//join I2C bus
//start Serial communication
//blink LED to show its sending 
//initialize UDP communication
void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);

     // Serial.begin(115200);
    udp.begin(0);//necessary even for sending only.
    while (!WiFi.ready())
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

//loop()
//
//read acceleration from the sensor 
//send accel values via OSC
//blink LED to show its sending 
//
void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    //divide by 16384 according to datasheet of sensor (see https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/)
    AccX = ax/ 16384.0;
    AccY = ay/ 16384.0;
    AccZ= az/16384.0;
    
    // GyroX = gx/ 131.0;
    // GyroY = gy/ 131.0;
    // GyroZ= gz/ 131.0;;

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    //SEND OSC (to Max) 
    OSCMessage outMessage("/sarah");
    outMessage.addFloat(AccX);
    outMessage.addFloat(AccY);
    outMessage.addFloat(AccZ);
    outMessage.send(udp,outIp,outPort);
};
