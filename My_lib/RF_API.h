#ifndef RF_API_H
#define RF_API_H

#include "BC3603_CMD_REG.h"
#include "rf_parameter.h"  
#include "SYS.h"
#include "Spi.h"
#include "BC3603.h"
/*------------------------------------------------------------------*/
/*							Variable								*/
/*------------------------------------------------------------------*/
extern unsigned char RF_TXFIFO[];		//FIFO length defined in Configuration.h
extern unsigned char RF_RXFIFO[];		//FIFO length defined in Configuration.h
extern unsigned char RF_IRQ;
extern unsigned char mRFSync[];			//SYNC length defined in Configuration.h
extern unsigned char mRFSync1[];
/*#if RF_Payload_Length>64				//Extend Mode*/
extern unsigned char ExtTxLength;
extern unsigned char ExtRxLength;
/*#endif*/
/*------------------------------------------------------------------*/
/*							Define									*/
/*------------------------------------------------------------------*/
#define	RF_DeepSleep			0x00
#define	RF_Idle					0x01
#define	RF_LightSleep			0x02
#define	RF_Standby				0x03
#define	RF_TX					0x04
#define	RF_RX					0x05
#define	RF_VCO					0x06


/*------------------------------------------------------------------*/
/*							Type Define								*/
/*------------------------------------------------------------------*/
typedef struct 
{
//	unsigned char	rf_band;
//	unsigned long	freq;
	unsigned char	mode;
	unsigned char	step;
	unsigned char	pairing;
	unsigned char	*sync_word;
	unsigned char	sync_word_len;
	unsigned char	irq_status;

	unsigned char	data_rate;
		
	unsigned char	tx_power;
	unsigned char	tx_preamble_len;
	unsigned int	tx_packet_len;
	unsigned char	*tx_payload_buffer;
	
	unsigned char	tx_irq_f;
	unsigned char	tx_fail_f;
	
//	unsigned char	en_wor;
	unsigned char	rx_preamble_len;
	unsigned int	rx_packet_len;
	unsigned int	rec_data_len;
	unsigned char	*rx_payload_buffer;
	unsigned char	rx_irq_f;
	unsigned char	rssi;
//	unsigned char	env_rssi;
} _BC3603_device_;

void RF_Parameter_loading(void);
void SimpleFIFO_TX_Process(_BC3603_device_ *BC3603_T);
void ATR_WOR_Process(_BC3603_device_ *BC3603_T);

/*------------------------------------------------------------------*/
/*							General Function						*/
/*------------------------------------------------------------------*/
#define  BC3603_StrobeCommand(cmd) 			SpiWriteStrobeCMD(cmd)
#define  BC3603_WriteRegister(reg,wd) 		SpiWriteRegCMD(reg,wd)
#define  BC3603_ReadRegister(reg) 			SpiReadRegCMD(reg)
#define  BC3603_RegisterBank(BC3603_BANKn)	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3603_BANKn);
#define  BC3603_SetTxPayloadWidth(wd)      	SpiWriteRegCMD(PKT5_REGS,wd)
#define  BC3603_SetRxPayloadWidth(wd)      	SpiWriteRegCMD(PKT6_REGS,wd)
#define  BC3603_GetRxPayloadWidth()        	SpiReadRegCMD(PKT6_REGS)
#define  BC3603_SetTxPreambleWidth(wd)     	SpiWriteRegCMD(PKT1_REGS,wd)
#define  BC3603_SetTxPayloadSAddr(ad)      	SpiWriteRegCMD(FIFO1_REGS,ad)
#define  BC3603_GetIRQState()               SpiReadRegCMD(IRQ3_REGS)
#define  BC3603_ClearIRQFlag(sts)           SpiWriteRegCMD(IRQ3_REGS,sts)  
#define  BC3603_GetOperatState()            SpiReadRegCMD(STA1_REGS)
#define  BC3603_ReadRxPayload(pbuf,len)  	RFReadBuf2(READ_FIFO_CMD,len,pbuf)
#define  BC3603_WriteTxPayload(pbuf,len)	RFWriteBuf2(WRITE_FIFO_CMD,len,pbuf)
#define  BC3603_ReadSyncWord(pbuf,len)   	RFReadBuf2(READ_SYNCWORD_CMD,len,pbuf)
#define  BC3603_ReadChipID(pbuf)   			RFReadBuf2(READ_CHIPID_CMD,4,pbuf)
#define  BC3603_WriteSyncWord(pbuf,len)  	RFWriteBuf2(WRITE_SYNCWORD_CMD,len,pbuf)

#endif