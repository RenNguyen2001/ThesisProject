#include <LSM6DSV16XSensor.h>

#define SENSOR_ODR 104.0f // In Hertz
#define ACC_FS 2 // In g
#define GYR_FS 2000 // In dps
//FS is range
#define MEASUREMENT_TIME_INTERVAL (1000.0f/SENSOR_ODR) // In ms
#define FIFO_SAMPLE_THRESHOLD 199
#define FLASH_BUFF_LEN 8192

#define CS_PIN 25
#define SPI_MOSI  11
#define SPI_MISO  12
#define SPI_SCK 13

LSM6DSV16XSensor AccGyr(&SPI, CS_PIN);
volatile uint8_t fullFlag = 0; // FIFO full flag
uint8_t status = 0;
unsigned long timestamp_count = 0;
bool acc_available = false;
bool gyr_available = false;
int32_t acc_value[3];
int32_t gyr_value[3];
char buff[FLASH_BUFF_LEN];
uint32_t pos = 0;

void setup() {
  Serial.begin(115200);
  SPI.begin();

  // Initialize LSM6DSV16X.
  AccGyr.begin();
  
  //status |= AccGyr.Enable_X();
  AccGyr.Write_Reg(0x10, 0x01); //enable acc by setting the odr 
  //status |= AccGyr.Enable_G();
  AccGyr.Write_Reg(0x11, 0x01); //enable gyro by setting the odr

  gameSetup();
  
  // Configure ODR and FS of the acc and gyro
  AccGyr.Write_Reg(0x01, 0x00000000 + 0b00000000); //disable the embed reg access
  status |= AccGyr.Set_X_ODR(SENSOR_ODR);
  status |= AccGyr.Set_X_FS(ACC_FS);
  status |= AccGyr.Set_G_ODR(SENSOR_ODR);
  status |= AccGyr.Set_G_FS(GYR_FS);
  
  // Configure FIFO BDR for acc and gyro
  
  //status |= AccGyr.FIFO_Set_X_BDR(SENSOR_ODR);
  //status |= AccGyr.FIFO_Set_G_BDR(SENSOR_ODR);

  //AccGyr.Write_Reg(0x66, 0b00000010); //sflp initialisation request
  
  // Set FIFO in Continuous mode
  status |= AccGyr.FIFO_Set_Mode(LSM6DSV16X_STREAM_MODE);
  
  if(status != LSM6DSV16X_OK) {
    Serial.println("LSM6DSV16X Sensor failed to init/configure");
    while(1);
  }
  Serial.println("LSM6DSV16X FIFO Demo");

}

void gameSetup(){
  AccGyr.Write_Reg(0x01, 0x00000000 + 0b10000000); //enable the embed reg access
  AccGyr.Write_Reg(0x02, 0x00000001 + 0b00000000); //turning page to embed page
  status |= AccGyr.Write_Reg(0x04, 0b00000010); //set the SFLP_game_EN bit in the EMB_FUNC_EN_A reg
  status |= AccGyr.Write_Reg(0x5E, 0b01000011 + 0b00011000); //sflp odr set
  status |= AccGyr.Write_Reg(0x66, 0b00000010); //sflp initialisation request
  status |= AccGyr.Write_Reg(0x44, 0b00000010); //enable sflp game batching to fifo
}

void unityDataPrep(int16_t gameArr[]){
  int8_t byteH, byteL;
  int16_t directions[] = {254, 400, 555};  //needs to be two bytes to store values up to 65000
  int x, y, z;
  directions[0] = gameArr[0]; directions[1] = gameArr[1]; directions[2] = gameArr[2];   
  //Serial.print("Size of normal int is:"); Serial.println(sizeof(directions[0]));  Serial.print("Size of acc int is:"); Serial.println(sizeof(accArr[0]));

  Serial.write(255);  Serial.write(110);  //sending the dummy byte(s) to mark the first byte
  //need to seperate each interger into two bytes to send
  for(int i = 0; i < sizeof(directions)/sizeof(directions[0]); i++)
  {
    //for each loop, the data is seperated into two bytes and then sent
        
    byteL = directions[i];
        
    byteH = (signed short)directions[i] >> 8;
        
    Serial.write(byteL);  Serial.write(byteH);
   }
   Serial.write('\n');
   delayMicroseconds(100);
  
}

void loop() {
  uint8_t tag, temp, gameData[6];
  uint16_t quart[3];
  uint16_t temp16;
  int32_t temp32;

  //checkGameRegs();
  
  AccGyr.Write_Reg(0x01, 0x00000000 + 0b00000000); //disable the embed reg access
  AccGyr.FIFO_Get_Tag(&tag);  Serial.print("Tag: "); Serial.println(tag, HEX);
  AccGyr.FIFO_Get_Full_Status(&temp);  Serial.print("Status: "); Serial.println(temp);
  AccGyr.FIFO_Get_Num_Samples(&temp16); Serial.print("Num of Samples: "); Serial.println(temp16);
  AccGyr.FIFO_Get_Data(gameData); Serial.println("Game Vector: "); 
  for(int i = 0; i < 3; i++)
  {
    /*
     quart[0] = (uint16_t)(data[0] << 8) + data[1];
     quart[1] = (uint16_t)(data[2] << 8) + data[3];
     quart[2] = (uint16_t)(data[4] << 8) + data[5];

     quart[i] = (uint16_t)(data[i*2] << 8) + data[i*2+1];
     */
     quart[i] = (uint16_t)(gameData[i*2] << 8) + gameData[i*2+1];
  }
  Serial.print("X: "); Serial.println(quart[0]);  //for some reason this command causes the fifo tage and num of samples to be 0
  Serial.print("Y: "); Serial.println(quart[1]);
  Serial.print("Z: "); Serial.println(quart[2]);

  unityDataPrep(quart);
}

void checkGameRegs(){
  uint8_t tag, temp;
  uint16_t temp16;
  int32_t temp32;
  AccGyr.Write_Reg(0x01, 0x00000000 + 0b10000000); //enable the embed reg access
  AccGyr.Write_Reg(0x02, 0x00000001 + 0b00000000); //turning page to embed page
  Serial.println("Checking registers:");
  AccGyr.Read_Reg(0x04,&temp);  Serial.print("Game enable register:"); Serial.println(temp, BIN);
  AccGyr.Read_Reg(0x66,&temp);  Serial.print("Game initialise:"); Serial.println(temp, BIN);
  AccGyr.Read_Reg(0x44,&temp);  Serial.print("Game fifo batch:"); Serial.println(temp, BIN);
  AccGyr.Read_Reg(0x5E,&temp);  Serial.print("Game ODR:"); Serial.println(temp, BIN);
  AccGyr.Read_Reg(0x07,&temp);  Serial.print("Game running?(1 if true):"); Serial.println(temp, BIN);
}
