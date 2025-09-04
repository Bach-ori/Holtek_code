#ifndef RF_PARAMETER_H
#define RF_PARAMETER_H

#include "BC3603_CMD_REG.h"
#include "rf_parameter.h"

/*------------------------------------------------------------------*/
/*							RF Config								*/
/*------------------------------------------------------------------*/
#define 	XTAL_FREQ					16				// *1MHz
//	BC3603 Frequency	Configure//
#define		RF_BAND						_433MHz_			// [315/433/470/868/915]MHz
#define  	RF_FREQ						44444
//	RF Crystal C-Load Tune
#define  	RF_CYL_COARSE_TUNE_			(1)					//	Coarse Tune<0-3>
#define  	RF_CYL_FINE_TUNE_			(3)					//	Fine Tune<00-31>


#define		RF_TxPower					_10dBm_				// [0/10/13/17/19]dBm
#define		RF_Datarate					_125kbps_			// [2/10/50/125/250]kbps

#define		RF_Payload_Length			10					// [1-255]bytes
#define		RF_SYNC_Length				4					// [1~8]bytes
#define		RF_SYNC_BCHcoding			0					// [1:ON 0:OFF]		must be set same in BCH32.asm
#define		RF_SYNC_L					0xB8CB2B05			// SYNC[3-0]bytes
#define		RF_SYNC_H					0x753DE91C			// SYNC[7-4]bytes

#define		RF_Trailer_EN				0					// [1:ON 0:OFF]
#define		RF_TxPreamble_Length		1					// [0-255]+1 bytes
#define		RF_Preamble_Pattern_EN		1					// [1:Preamble pattern is derived from PMLPAT[15:0] 0:Preamble pattern is derived from ¡§SYNCWORD MSB+1/0 toggle¡¨ ]
#define		RF_TX_PREAMBLE_CLASS		0					// [1:2Byte Pattern(low byte first) 0:1Byte Pattern]
#define		RF_TX_PREAMBLE_PATTERN		0xAAAA				// Pattern Value<0x0000-0xFFFF>
#define		RF_PLLEN_EN					0					// [1:ON 0:OFF]
#define		RF_Header_EN				0					// [1:ON 0:OFF]
#define		RF_Header_Length			0					// [1:2bytes 0:1byte]
#define		RF_PLHAC_EN                 0					// [1:ON 0:OFF]
#define		RF_PLHA_Address				0x0000				// PLHEA[7:0] + PLHA[5:0]


#define		RF_Manchester_EN			0					// [1:ON 0:OFF]
#define		RF_FEC_EN					0					// [1:ON 0:OFF]
#define		RF_CRC_EN					1					// [1:ON 0:OFF]
#define		RF_CRC_Format				0					// [1:IBC-16-CRC 0:CCITT-16-CRC]
#define  	RF_CRC_INVERTED				0					// [1:Inverted Enable 0:Inverted Disable]
#define		RF_CRC_BYTE_ORDER			0					// [1:Low Byte First  0:High Byte First ]
#define		RF_CRC_BIT_ORDER			0					// [1:CRC LSB First  0:CRC MSB First] 		
#define		RF_CRC_SEED					0xFFFF				// CRC Seed<0x0000-0xFFFF>			

#define		RF_whitening_EN				0					// [1:ON 0:OFF]
#define		RF_whitening_Format			0					// [0:360X 1:PN7 2:PN9-CCIT 4:PN9-IBM]
#define		RF_whitening_Seed			0x00				// [0-127]	

/*ATR MODE*/
#define		RF_ATR_Enable				1					// [1:ON 0:OFF]
#define		RF_ATR_Cycle				1000				// [1-8000]ms
#define		RF_ATR_RX_Window			2000				// [250-511000]us
#define		RF_ATR_RX_extra				2000					// [1-16380]ms

/*ARK MODE*/
#define		RF_ARK_Enable				0					// [1:ON 0:OFF]		if ARK on,RF payload length must be set less than 64 .
#define		RF_ARK_Resend_Count			3					// [0-15]+1 count
#define		RF_ARK_RX_Window			10000				// [250-63750]us

#endif