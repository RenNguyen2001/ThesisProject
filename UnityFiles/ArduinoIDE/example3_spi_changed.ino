/*
  example3-spi

  This example shows the basic settings and functions for retrieving accelerometer
  and gyroscopic data but using the SPI interface. Everything other than that is 
  identical to the "basic" example.

  Written by Elias Santistevan @ SparkFun Electronics, May 2022

  Products:

  SparkFun 6DoF LSM6DSV16X (Qwiic):
    https://www.sparkfun.com/products/21325

  SparkFun Micro 6DoF LSM6DSV16X (Qwiic): 
    https://www.sparkfun.com/products/21336 

  Repository:
    https://github.com/sparkfun/SparkFun_LSM6DSV16X_Arduino_Library

  SparkFun code, firmware, and software is released under the MIT 
  License (http://opensource.org/licenses/MIT).
*/

#include <SPI.h>
#include "SparkFun_LSM6DSV16X.h"

// SPI instance class call
SparkFun_LSM6DSV16X_SPI myLSM; 

// Structs for X,Y,Z data
sfe_lsm_data_t accelData; 
sfe_lsm_data_t gyroData; 

// Set your chip select pin according to your setup. 
int chipSelect = 28;

void setup(){

  SPI.begin();

  Serial.begin(115200);
  while(!Serial) {}
  Serial.println("LSM6DSV16X Example 3 - Basic Readings SPI");

  IMU_setup(25);
  //IMU_setup(30);
  //IMU_setup(5);

}

void IMU_setup(char csPin){
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);


  if( !myLSM.begin(csPin) ){
    Serial.println("Did not begin, check your wiring and/or I2C address!");
    while(1);
  }

  // Reset the device to default settings. This if helpful is you're doing multiple
  // uploads testing different settings. 
  myLSM.deviceReset();

  // Wait for it to finish reseting
  while( !myLSM.getDeviceReset() ){ 
    delay(1);
  } 

  Serial.println("Board has been Reset.");
  Serial.println("Applying settings.");
  
  // Accelerometer and Gyroscope registers will not be updated
  // until read. 
  myLSM.enableBlockDataUpdate();
  
  // Set the output data rate and precision of the accelerometer
  myLSM.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
  myLSM.setAccelFullScale(LSM6DSV16X_16g); 

  // Set the output data rate and precision of the gyroscope
  myLSM.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
  myLSM.setGyroFullScale(LSM6DSV16X_2000dps);

  // Enable filter settling.
  myLSM.enableFilterSettling();

  // Turn on the accelerometer's filter and apply settings. 
  myLSM.enableAccelLP2Filter();
  myLSM.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);

  // Turn on the gyroscope's filter and apply settings. 
  myLSM.enableGyroLP1Filter();
  myLSM.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);
  
  Serial.println("Ready.");
  digitalWrite(csPin, HIGH);
}

void unityDataPrep(){
  //Convert the data into an angle between 0 and 360
  
  
}

void loop(){
  int tempAcc[3], tempGyro[3];
 
  char byteH, byteL;
  int directions[] = {254, 400, 555};
  int x, y, z;
   
  for(char i = 0; i < 1; i++)
  {
    int cs_Pin;
    switch(i)
    {
      case 0: cs_Pin = 25;  break;
      case 1: cs_Pin = 30;  break;
      case 2: cs_Pin = 5; break;
    }
    //digitalWrite(cs_Pin, LOW); Serial.print("Case: "); Serial.println(cs_Pin);
    //chars can't be printed properly
    
    // Check if both gyroscope and accelerometer data is available.
    if( myLSM.checkStatus() ){
      myLSM.getAccel(&accelData);
      myLSM.getGyro(&gyroData);

      tempAcc[0] = accelData.xData; tempAcc[1] = accelData.yData; tempAcc[2] = accelData.zData;   
      tempGyro[0] = gyroData.xData; tempGyro[1] = gyroData.yData; tempGyro[2] = gyroData.zData;

      /*
      Serial.println("Accelerometer: ");  
      Serial.print("X: ");  Serial.print(tempAcc[0]);  Serial.println(" ");
      Serial.print("Y: ");  Serial.print(tempAcc[1]);  Serial.println(" ");
      Serial.print("Z: ");  Serial.print(tempAcc[2]);  Serial.println(" ");
      
      Serial.println("Gyroscope: ");
      Serial.print("X: ");  Serial.print(tempGyro[0]); Serial.println(" ");
      Serial.print("Y: ");  Serial.print(tempGyro[1]); Serial.println(" ");
      Serial.print("Z: ");  Serial.print(tempGyro[2]); Serial.println(" ");
      Serial.println(" ");
      */

      directions[0] = tempAcc[0]; directions[1] = tempAcc[1]; directions[2] = tempAcc[2];   

      Serial.write(255);  Serial.write(110);  //sending the dummy byte(s) to mark the first byte
      //need to seperate each interger into two bytes to send
      for(int i = 0; i < sizeof(directions)/sizeof(int); i++)
      {
        //for each loop, the data is seperated into two bytes and then sent
        
        byteL = directions[i];
        
        byteH = (unsigned int)directions[i] >> 8;
        
        Serial.write(byteL);  Serial.write(byteH);
      }
      Serial.write('\n');
      delayMicroseconds(50);
  
    }
    else
    {
      //Serial.println("Error");
    }
    digitalWrite(cs_Pin, HIGH);
    delay(100);
  }
}
