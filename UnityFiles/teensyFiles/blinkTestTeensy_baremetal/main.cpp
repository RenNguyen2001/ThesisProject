//MADE IN VISUAL STUDIO CODE

#include <Arduino.h>
#include "IMXRT1060RM.h"

void GPIO_setup();

void setup() {
  // put your setup code here, to run once:
  GPIO_setup();
  //pinMode(13, OUTPUT);
  //digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  GPIO2_DR_SET = (1 << 3);
  delay(1000);
  GPIO2_DR_CLEAR = (1 << 3);
  delay(1000);
}

void GPIO_setup(){
  // 1. Set pads to GPIO mode using IOMUX registers 

    //IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03 = 0b101;  //setting to GPIO mode (mode 5)
    IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03 = 5;

  // 2. Setting the data direction register (make IO an output)

    IOMUXC_GPR_GPR27 = 0x00000000;  //GP27- GPIO2 and GPIO7 share the same IO MUX function
    //if GPR27 = 0, GPIO2 is selected, if GPR27 = 1 GPIO 7 is elected
    //I think this only applies to this pin since it blinks the on-board LED
    GPIO2_GDIR |= (1 << 3);
  
  
  
  Serial.print("Setting up");
}
