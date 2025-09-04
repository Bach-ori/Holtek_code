#include <string.h>
#include <stdint.h>

#include "BC3603.h"
#include "BC3603_CONFIG_TABLE.h"
#include "BC3603_CMD_REG.h"
#include "Spi.h"
#include "Function.h"

unsigned char ExtTxLength=255;
unsigned char ExtRxLength;
unsigned char RF_TXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_RXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h

unsigned char RF_IRQ;
unsigned char mRFSync[RF_SYNC_Length];			//SYNC length defined in Configuration.h

void SpiWriteStrobeCMD(uint8_t CMD)
{
    uint8_t tx[1] = { CMD };
    uint8_t rx[1] = { 0 };
    SpiTransfer(tx, rx, 1);
}

uint8_t SpiReadRegCMD(uint8_t reg) 
{
    uint8_t tx[2] = { (reg) | READ_REGS_CMD, 0x00 };
    uint8_t rx[2] = {0};
    SpiTransfer(tx, rx, 2);
    usleep(2000);
    return rx[1];
}

void SpiWriteRegCMD(uint8_t reg, uint8_t value) 
{
    uint8_t tx[2] = { (reg) | WRITE_REGS_CMD, value };
    uint8_t rx[2] = {0};
    SpiTransfer(tx, rx, 2);
    usleep(2000);
}


void RFReadBuf2(uint8_t CMD, uint8_t length, uint8_t *data)
{
    uint8_t tx[length + 1];
    uint8_t rx[length + 1];

    tx[0] = CMD;
    memset(&tx[1], 0xFF, length);

    if (SpiTransfer(tx, rx, length + 1) < 0) {
        perror("RFReadBuf2: SPI transfer failed");
        return;
    }

    memcpy(data, &rx[1], length);
}

void RFWriteBuf2(uint8_t CMD, uint8_t length, const uint8_t *data)
{
    uint8_t tx[length + 1];
    uint8_t rx[length + 1];  // có thể bỏ qua nếu không cần đọc

    tx[0] = CMD;
    memcpy(&tx[1], data, length);

    if (SpiTransfer(tx, rx, length + 1) < 0) {
        perror("RFWriteBuf2: SPI transfer failed");
    }
}

void RFEnATR(void)
{
    unsigned char a=0;
	a  = SpiReadRegCMD(ATR1_REGS);
	a |= 0x01;
	SpiWriteRegCMD(ATR1_REGS,a);
}

void RFXtalReady(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(RC1_REGS);
		a &= 0x20;
	}
}

void RFWriteFreqTABLE(void)
{
	unsigned char a=0;
	a = SpiReadRegCMD(OM_REGS);
	a = (a&0x9f)|_OM_;
	SpiWriteRegCMD(OM_REGS,a);
	
	for(a=0;a<(sizeof Frequency_REGS_TABLE/2);a++)	SpiWriteRegCMD(((Frequency_REGS_TABLE[a]>>8)), Frequency_REGS_TABLE[a]);
}

void RFCalibration(void)
{
	volatile unsigned char a=0;
	a=SpiReadRegCMD(OM_REGS);
	a |= 0x08;
	SpiWriteRegCMD(OM_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(OM_REGS);
		a &= 0x08;
	}
}

void LircCalibration(void)
{
	volatile unsigned char a=0;
	a=SpiReadRegCMD(XO3_REGS);
	a |= 0x81;
	SpiWriteRegCMD(XO3_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(XO3_REGS);
		a &= 0x80;
	}
}

unsigned char RFGetClrRFIrq(void)
{
	unsigned char irq;
	
	irq=SpiReadRegCMD(IRQ3_REGS);
	SpiWriteRegCMD(IRQ3_REGS,irq);
	
	return irq;
}

void RFSetPower(unsigned char band_sel,unsigned char power)
{
	SpiWriteRegCMD(TX2_REGS,TxPowerValue[band_sel][power]);
}

void RFSetDRPram(void)
{
	unsigned char a=0;
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK0);
	for(a=0;a<(sizeof DM_REGS_TABLE/2);a++)	SpiWriteRegCMD(((DM_REGS_TABLE[a]>>8)), DM_REGS_TABLE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK1);
	for(a=0;a<(sizeof FCF_REGS_TABLE/2);a++)	SpiWriteRegCMD(((FCF_REGS_TABLE[a]>>8)), FCF_REGS_TABLE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK2);
	for(a=0;a<(sizeof CBPF_REGS_TABLE/2);a++)	SpiWriteRegCMD(((CBPF_REGS_TABLE[a]>>8)), CBPF_REGS_TABLE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK0);
}

void BC3603_Config(void)
{
	unsigned char a=0;
	//	BC3601 IRQ/IO Configure	//
	for(a=0;a<(sizeof IRQIO_REGS_TABLE/2);a++)	SpiWriteRegCMD(((IRQIO_REGS_TABLE[a]>>8)), IRQIO_REGS_TABLE[a]);
	//	BC3603 packet format Configure	//
	for(a=0;a<(sizeof PACKET_REGS_TABLE/2);a++)	SpiWriteRegCMD(((PACKET_REGS_TABLE[a]>>8)), PACKET_REGS_TABLE[a]);
	//	BC3603 common Configure	//
	for(a=0;a<(sizeof COMMON_REGS_TABLE/2);a++)	SpiWriteRegCMD(((COMMON_REGS_TABLE[a]>>8)), COMMON_REGS_TABLE[a]);
	
	//	BC3603 Bank0 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK0);
	for(a=0;a<(sizeof BANK0_REGS_TABLE/2);a++)	SpiWriteRegCMD(((BANK0_REGS_TABLE[a]>>8)), BANK0_REGS_TABLE[a]);
	//	BC3603 Bank1 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK1);
	for(a=0;a<(sizeof BANK1_REGS_TABLE/2);a++)	SpiWriteRegCMD(((BANK1_REGS_TABLE[a]>>8)), BANK1_REGS_TABLE[a]);
	//	BC3603 Bank2 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK2);
	for(a=0;a<(sizeof BANK2_REGS_TABLE/2);a++)	SpiWriteRegCMD(((BANK2_REGS_TABLE[a]>>8)), BANK2_REGS_TABLE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK0);
}

void RFWriteBuf(uint8_t CMD, uint8_t length, const uint8_t data[])
{
    uint8_t tx[length + 1];
    uint8_t rx[length + 1];  // dùng để nhận dữ liệu dummy (nếu cần)

    tx[0] = CMD;               // byte lệnh
    memcpy(&tx[1], data, length);  // copy dữ liệu vào buffer

    if(SpiTransfer(tx, rx, length + 1) < 0)
    {
        perror("RFWriteBuf: SPI transfer failed");
    }
}

void RFWriteSyncword(void)
{
	unsigned char a;	
	for(a=0;a<RF_SYNC_Length;a++)
		mRFSync[a] = BC3603_SYNCWORD[a];
	RFWriteBuf(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
}

void BC3603_SetupSPI4Wires(void)
{
	SpiWriteRegCMD(IO1_REGS,0x69);//GIO1:SDO GIO2:IRQ
	SpiWriteRegCMD(IO2_REGS,0x00);	
	SpiWriteRegCMD(IO3_REGS,0xF9);	//???EN ??GIO1,2 pull-up
}

void RF_Init(void)
{
    SpiInit();
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK0);//
	BC3603_SetupSPI4Wires();
	SpiWriteStrobeCMD(SOFT_RESET_CMD);							// RF software reset
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK2);	
	SpiWriteRegCMD(0x39,0x9C);	
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANK0);		
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);							// Entry Lightsleep mode
	SpiWriteRegCMD(IRQ1_REGS,0xDA);	
	BC3603_Config();

	RFWriteSyncword();
	
	RFWriteFreqTABLE();									 		// Set RF working frequency
	RFSetDRPram();												// Set RF datarate
	RFSetPower(RF_BAND,RF_TxPower);								// Set RF output power

	RFXtalReady();												// Whaiting XTAL ready
	RFCalibration();											// RF Calibration
	LircCalibration();											// LIRC Calibration
	RFGetClrRFIrq();											// Clear RF IRQ status

	SpiWriteStrobeCMD(DEEP_SLEEP_CMD);
}

