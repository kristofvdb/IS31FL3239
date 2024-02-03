/* 07/02/2021 Copyright Tlera Corporation
 *  
 *  Created by Kris Winer
 *  
 This is a simple library for the IS31FL3246 36-led driver from Lumissil
 
 Library may be used freely and without limit with attribution.
 
*/

#include "IS31FL3246A.h"
#include "I2CDev.h"

IS31FL3246A::IS31FL3246A(I2Cdev* i2c_bus, uint8_t addr) {
  _i2c_bus = i2c_bus;
  m_addr = addr;
}


void IS31FL3246A::reset()  // return to default registers/conditions
{
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_RESET, 0x00);
}


void IS31FL3246A::disable() {
  uint8_t temp = _i2c_bus->readByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL);
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL, temp & ~(0x01));  //clear bit 0 to shut down
}

void IS31FL3246A::allLedOn(uint8_t addr, uint8_t value) {
  printf("+allLedOn\n");
  uint8_t startSL[] = { 0x4c, 0x52, 0x58, 0x5c, 0x62, 0x68 };
  for (int j = 0; j < 6; j++) 
  {
    for (int k = 0; k < 4; k++) {
      _i2c_bus->writeByte(addr, startSL[j] + k, 0x8f);  //set bit 0 to enable
       //delay(100);
    }
    //delay(100);
  }

  uint8_t startGLC[] = { 0x05, 0x11, 0x1d, 0x25, 0x31, 0x3d };
  for (int s = 0; s < 6; s++) {
    for (int k = 0; k < 8; k++) {
      _i2c_bus->writeByte(addr, startGLC[s] + k, 0xf0);  //set bit 0 to enable
 //     _i2c_bus->writeByte(addr, startGLC[j]+k+1, 0x8f); //set bit 0 to enable

      //delay(100);
    }
    //delay(100);

    //_i2c_bus->writeByte(addr, 0x2c, 0xff); //set bit 0 to enable
    //_i2c_bus->writeByte(addr, 0x2b, 0xff); //set bit 0 to enable
    //_i2c_bus->writeByte(addr, 0x5f, 0xff); //set bit 0 to enable
  }

  //_i2c_bus->writeByte(addr, 0x49, 0x00);  //set bit 0 to update

  printf("-allLedOn\n");
}


void IS31FL3246A::enable() {
  printf("+enable\n");
  uint8_t addr[] = { 0x34, 0x37 };

  for (int i = 0; i < sizeof(addr); i++) 
  {
    printf("initializing 0x%x\n", addr[i]);
    uint8_t temp = _i2c_bus->readByte(addr[i], 0);
    printf("enable = 0x%02x\n", temp);
    //temp = _i2c_bus->readByte(addr[i], 0);
    //printf("enable = 0x%02x\n", temp);

   //_i2c_bus->writeByte(addr[i], 0x7F, 0x01);  //reset
  //  delay(2000);
    _i2c_bus->writeByte(addr[i], 0x7F, 0x00);  //reset
    printf("reset done\n");
    delay(100);
    _i2c_bus->writeByte(addr[i], 0x00, 0x01);  //set bit 0 to enable
    //delay(100);
    allLedOn(addr[i], 0x7F);
    //_i2c_bus->writeByte(addr[i], 0x2c, 0xff); //set bit 0 to enable
    //_i2c_bus->writeByte(addr[i], 0x2b, 0xff); //set bit 0 to enable
    //_i2c_bus->writeByte(addr[i], 0x5f, 0xff); //set bit 0 to enable

    //_i2c_bus->writeByte(addr[i], 0x44, 0xff); //set bit 0 to enable
    //_i2c_bus->writeByte(addr[i], 0x43, 0xff); //set bit 0 to enable
    //_i2c_bus->writeByte(addr[i], 0x6b, 0xff); //set bit 0 to enable

    //_i2c_bus->writeByte(addr[i], 0x25, 0x8f); //set bit 0 to enable green4
    //_i2c_bus->writeByte(addr[i], 0x27, 0x8f); //set bit 0 to enable red4
    //_i2c_bus->writeByte(addr[i], 0x29, 0x8f); //set bit 0 to enable blue4
    //_i2c_bus->writeByte(addr[i], 0x2b, 0x8f); //set bit 0 to enable green3
    //_i2c_bus->writeByte(addr[i], 0x31, 0x8f); //set bit 0 to enable red3
    //_i2c_bus->writeByte(addr[i], 0x33, 0x8f); //set bit 0 to enable blue3
    //_i2c_bus->writeByte(addr[i], 0x35, 0x8f); //set bit 0 to enable green 2
    //_i2c_bus->writeByte(addr[i], 0x37, 0x8f); //set bit 0 to enable red 2
    //_i2c_bus->writeByte(addr[i], 0x3d, 0x8f); //set bit 0 to enable blue 2
    //delay(500);
    _i2c_bus->writeByte(addr[i], 0x6e, 0xff);  //global current control
    _i2c_bus->writeByte(addr[i], 0x49, 0x00);  //update register

    //while(true);
  }

  printf("done\n");
  while (true)
    ;
}


void IS31FL3246A::mode(uint8_t resolution, uint8_t frequency, uint8_t ledMode) {
  //set ledMode (bit 6), HF frequency (bits 4 and 5), and resolution (bit 1)
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL, resolution << 1 | frequency << 4 | ledMode << 6);  // bit 0 is set to disable here
}


void IS31FL3246A::setHFPWM(uint8_t channel, uint8_t FMS, uint8_t HFPWMLevel_H, uint8_t HFPWMLevel_L) {
  if (channel >= 1 && channel <= 24) {
    _i2c_bus->writeByte(IS31FL3246A_ADDRESS, (2 * channel - 1), HFPWMLevel_L);           // set lower bytes of HF PWM level (0 - 255) for specified led channel
    _i2c_bus->writeByte(IS31FL3246A_ADDRESS, (2 * channel), (FMS << 2) | HFPWMLevel_H);  // set freq control mode and upper 2 bits of HF PWM level for specified led channel
  }
}


void IS31FL3246A::setLFPWM(uint8_t channel, uint8_t PWMLevel) {
  if (channel >= 1 && channel <= 36) {
    _i2c_bus->writeByte(IS31FL3246A_ADDRESS, channel + 0x48, PWMLevel);  // set LF PWM level (0 - 255) for specified led channel
  }
}


void IS31FL3246A::setGlobalCurrent(uint8_t IR, uint8_t IG, uint8_t IB) {
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_GCC_R, IR);  // set red current maximum
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_GCC_G, IG);  // set green current maximum
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_GCC_B, IB);  // set blue current maximum
}


void IS31FL3246A::PWMUpdate() {
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_UPDATE, 0x00);  // update led array
}
