#define DECLINATION -5.55

#include <Wire.h>
#include <MMC34160PJ.h>

#define I2C_ADDRESS 0x30

MMC34160PJ magnetometer(I2C_ADDRESS);

void angleFinder(int *pX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1e6);
  Wire.begin();
  if(!magnetometer.init()){
    //Serial.println("MMC34160PJ not connected!");
  }
  magnetometer.setDeclination(DECLINATION);
}

void loop() {
  // put your main code here, to run repeatedly:
  char byteH, byteL;
  int directions[] = {254, 400, 555};
  int x, y, z;
  
  //magnetometer.readXYZData(&x, &y, &z); 
  //directions[0] = x;
  //directions[1] = y;  
  angleFinder(&x);
  directions[0] = x;  

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

void angleFinder(int *pX){
  float angle = 0;
  if(magnetometer.readData()){
    angle = magnetometer.getAngle();
    //Serial.print("Angle: "); Serial.println(angle);
  } else {
    //Serial.println("Failed to retrieve angle");
  }
  *pX = angle;
}
