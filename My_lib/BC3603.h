#ifndef BC3603_H
#define BC3603_H_

#include <stdint.h>

void SpiWriteRegCMD(unsigned char REG,unsigned char DATA);
void SpiWriteStrobeCMD(unsigned char CMD);
unsigned char SpiReadRegCMD(unsigned char REG);
void RFWriteBuf2(uint8_t CMD, uint8_t length, const uint8_t *data);
void RFReadBuf2(uint8_t CMD, uint8_t length, uint8_t *data);
void RFXtalReady(void);
void RFEnATR(void);
void BC3603_SetupSPI4Wires(void);
void RF_Init(void);
void BC3603_Config(void);
void RFWriteFreqTABLE(void);
void RFSetPower(unsigned char band_sel,unsigned char power);
void RFSetDRPram(void);
void RFCalibration(void);
void LircCalibration(void);
unsigned char RFGetClrRFIrq(void); 
void RFWriteBuf(uint8_t CMD, uint8_t length, const uint8_t data[]);
void RFWriteSyncword(void);

#endif