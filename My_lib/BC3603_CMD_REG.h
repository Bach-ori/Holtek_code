#ifndef BC3603_CMD_REG_H
#define BC3603_CMD_REG_H

/*------------------------------------------------------------------*/
/*						BC3603 strobe command 						*/
/*------------------------------------------------------------------*/
#define WRITE_REGS_CMD			0x40 //Register write command
#define READ_REGS_CMD			0xc0 //Register read command
#define REGS_BANK_CMD      		0x20 //Set Register Bank command
#define WRITE_SYNCWORD_CMD  	0x10 //Write Sync-Word command
#define READ_SYNCWORD_CMD   	0x90 //Read Sync-Word command
#define WRITE_FIFO_CMD      	0x11 //Write TX FIFO command
#define READ_FIFO_CMD       	0x91 //Read RX FIFO command
#define SOFT_RESET_CMD      	0x08 //Software reset command
#define	READ_CHIPID_CMD			0x9F //Read Sync-Word command
#define REST_TX_POS_CMD     	0x09 //TX FIFO address pointer reset command
#define REST_RX_POS_CMD     	0x89 //RX FIFO address pointer reset command
#define DEEP_SLEEP_CMD      	0x0A //Enter Deep Sleep mode
#define IDLE_MODE_CMD       	0x0B //Enter Idle mode
#define LIGHT_SLEEP_CMD     	0x0C //Enter Light Sleep mode
#define STANDBY_MODE_CMD    	0x0D //Enter Standby mode
#define TX_MODE_CMD         	0x0E //Enter TX mode
#define RX_MODE_CMD         	0x8E //Enter RX mode
#define REGSADDR_MASK       	0x3F
	
	
//-----------------  Interrupt Control Register 2/3 -----------------
#define	_TXFSHI_				0x01	// TX Finish IRQ 
#define	_RXFSHI_         		0x02  // RX Finish IRQ with no error 
#define	_CALFSHI_      			0x04  // Calibration Finish IRQ. 
#define	_RXDETI_         		0x08  // RX Carry/Preamble/Syncword detected. 
#define	_RXFAILI_           	0x10  // RX failure,(CRC_EN and CRC failure) or (RX FIFO overwrite) 
#define	_TRXFFMGI_          	0x20  // TX/RX FIFO length less/greater than threshold 
#define	_WOTWKPI_        		0x40  
#define	_ARKFAILI_          	0x80  // ARK TX Failure 


/*------------------------------------------------------------------*/
/*						BC3603 Register Bank 						*/
/*------------------------------------------------------------------*/
#define BC3603_BANK0       		0x00
#define BC3603_BANK1      		0x01
#define BC3603_BANK2      		0x02

/*------------------------------------------------------------------*/
/*						BC3603 RF Band_SEL	 						*/
/*------------------------------------------------------------------*/
#define BAND_315MHz      		0x00
#define BAND_433MHz      		0x01
#define BAND_470MHz      		0x02
#define BAND_868MHz      		0x03
#define BAND_915MHz      		0x03

#define _315MHz_     			0x00
#define _433MHz_      			0x01
#define _470MHz_      			0x02
#define _868MHz_      			0x03
#define _915MHz_      			0x04

/*------------------------------------------------------------------*/
/*					  	  BC3603 RF MODE	 						*/
/*------------------------------------------------------------------*/
#define	RF_DeepSleep			0x00
#define	RF_Idle					0x01
#define	RF_LightSleep			0x02
#define	RF_Standby				0x03
#define	RF_TX					0x04
#define	RF_RX					0x05
#define	RF_VCO					0x06

/*------------------------------------------------------------------*/
/*					 BC3603 RF Power & DR SEL	 					*/
/*------------------------------------------------------------------*/
#define _0dBm_      			0x00
#define _10dBm_      			0x01
#define _13dBm_      			0x02
#define _17dBm_      			0x03
#define _20dBm_      			0x03

#define _2kbps_ 				0x00
#define _10kbps_ 				0x01
#define _50kbps_      			0x02
#define _125kbps_      			0x03
#define _250kbps_      			0x04

/*------------------------------------------------------------------*/
/*						 BC3603 Register	 						*/
/*------------------------------------------------------------------*/
//Common register
#define  CFG1_REGS            	0x00	/* Configuration Control Register */
#define  RC1_REGS             	0x01	/* Reset/Clock Control Register */
#define  IRQ1_REGS            	0x02	/* Interrupt Control Register */
#define  IRQ2_REGS            	0x03	/* Interrupt enable register */
#define  IRQ3_REGS            	0x04	/* interrupt status register */
#define  IO1_REGS             	0x06	/* GPIO 1/2 control register */
#define  IO2_REGS             	0x07	/* GPIO 3 control register */
#define  IO3_REGS             	0x08	/* SPI/GPIO pull high control register */
#define  FIFO1_REGS           	0x09	/* TX FIFO start address register */
#define  FIFO2_REGS           	0x0A	/* FIFO control 2 regster */
#define  PKT1_REGS            	0x0B	/* Packet preamble length register */
#define  PKT2_REGS            	0x0C	/* Packet Control Register 2 */
#define  PKT3_REGS            	0x0D	/* Packet Control Register 3 */
#define  PKT4_REGS            	0x0E	/* Data whitening control Register */
#define  PKT5_REGS            	0x0F	/* Tx Payload length Register */
#define  PKT6_REGS            	0x10	/* Rx Payload length Register */
#define  PKT7_REGS            	0x11	/* Packet Control Register 7 */
#define  PKT8_REGS            	0x12	/* Payload Header address b5~b0 */
#define  PKT9_REGS            	0x13	/* Payload Header address b13~b6 */
#define  MOD1_REGS            	0x14	/* Modulator Control Register 1 */
#define  MOD2_REGS            	0x15	/* Modulator Control Register 2 */
#define  MOD3_REGS            	0x16	/* Modulator Control Register 3 */
#define  DM1_REGS             	0x17	/* Demodulator Control Register 1 */
#define  DM2_REGS             	0x18	/* Demodulator Control Register 2 */
#define  DM3_REGS             	0x19	/* Demodulator Control Register 3 */
#define  DM4_REGS             	0x1A	/* Demodulator Control Register 4 */
#define  DM5_REGS             	0x1B	/* Demodulator Control Register 5 */
#define  DM8_REGS             	0x1E	/* Demodulator Control Register 8 */
//BANK0 register
#define  OM_REGS              	0x20	/* Operation Mode Control Register */
#define  SX1_REGS             	0x22    /* Fractional-N Synthesizer Control Register 1 */
#define  SX2_REGS             	0x23    /* Fractional-N Synthesizer Control Register 2 */
#define  SX3_REGS             	0x24    /* Fractional-N Synthesizer Control Register 3 */
#define  SX4_REGS             	0x25    /* Fractional-N Synthesizer Control Register 4 */
#define  STA1_REGS            	0x26	/* Status Register */
#define  RSSI2_REGS           	0x28	/* RSSI Control Register */
#define  RSSI3_REGS           	0x29	/* RSSI Control Register */
#define  RSSI4_REGS           	0x2A	/* RSSI Control Register */
#define  ATR1_REGS           	0x2B    /* Auto TX/RX Control Register 1 */
#define  ATR2_REGS           	0x2C    /* Auto TX/RX Control Register 2 */
#define  ATR3_REGS           	0x2D    /* Auto TX/RX Control Register 3 */
#define  ATR4_REGS           	0x2E    /* Auto TX/RX Control Register 4 */
#define  ATR5_REGS           	0x2F    /* Auto TX/RX Control Register 5 */
#define  ATR6_REGS           	0x30    /* Auto TX/RX Control Register 6 */
#define  ATR7_REGS           	0x31    /* Auto TX/RX Control Register 7 */
#define  ATR8_REGS           	0x32    /* Auto TX/RX Control Register 8 */
#define  ATR9_REGS           	0x33    /* Auto TX/RX Control Register 9 */
#define  ATR10_REGS           	0x34    /* Auto TX/RX Control Register 10 */
#define  ATR11_REGS           	0x35    /* Auto TX/RX Control Register 11 */
#define  PKT10_REGS				0x36	/* Packet Control Register 10 */
#define  PKT11_REGS				0x37	/* Preamble pattern low byte */
#define  PKT12_REGS				0x38	/* Preamble pattern hight byte */
#define  PKT13_REGS				0x39	/* CRC seed low byte */
#define  PKT14_REGS				0x3A	/* CRC seed high byte */
#define  PKT15_REGS				0x3B	/* Packet Control Register 15 */
#define  XO1_REGS           	0x3C    /* XO Control Register 1 */
#define  XO2_REGS           	0x3D    /* XO Control Register 2 */
#define  XO3_REGS           	0x3E    /* XO Control Register 3 */
#define  TX2_REGS           	0x3F    /* TX Control Register */
//BANK1 register
#define  AGC1_REGS           	0x20	/*AGC Coefficient Control Register1*/
#define  AGC2_REGS           	0x21	/*AGC Coefficient Control Register2*/
#define  AGC3_REGS           	0x22	/*AGC Coefficient Control Register3*/
#define  AGC4_REGS           	0x23	/*AGC Coefficient Control Register4*/
#define  AGC7_REGS           	0x26	/*AGC Coefficient Control Register7*/
#define  FCF1_REGS           	0x2C	/*Filter Coefficient Control Register1*/
#define  FCF2_REGS           	0x2D	/*Filter Coefficient Control Register2*/
#define  FCF3_REGS           	0x2E	/*Filter Coefficient Control Register3*/
#define  FCF4_REGS           	0x2F	/*Filter Coefficient Control Register4*/
#define  FCF5_REGS           	0x30	/*Filter Coefficient Control Register5*/
#define  FCF6_REGS           	0x31	/*Filter Coefficient Control Register6*/
#define  FCF7_REGS           	0x32	/*Filter Coefficient Control Register7*/
#define  FCF8_REGS           	0x33	/*Filter Coefficient Control Register8*/
#define  FCF9_REGS           	0x34	/*Filter Coefficient Control Register9*/
#define  FCF10_REGS           	0x35	/*Filter Coefficient Control Register10*/
#define  FCF11_REGS           	0x36	/*Filter Coefficient Control Register11*/
#define  FCF12_REGS           	0x37	/*Filter Coefficient Control Register12*/
#define  FCF13_REGS           	0x38	/*Filter Coefficient Control Register13*/
#define  FCF14_REGS           	0x39	/*Filter Coefficient Control Register14*/
#define  FCF15_REGS           	0x3A	/*Filter Coefficient Control Register15*/
#define  FCF16_REGS           	0x3B	/*Filter Coefficient Control Register16*/
#define  FCF17_REGS           	0x3C	/*Filter Coefficient Control Register17*/
#define  FCF18_REGS           	0x3D	/*Filter Coefficient Control Register18*/
#define  FCF19_REGS           	0x3E	/*Filter Coefficient Control Register19*/
//BANK2 register
#define	 RXG_REGS				0x21	
#define	 RX1_REGS				0x2E
#define	 RX2_REGS				0x2F
#define	 TX3_REGS				0x33
#define	 CA1_REGS				0x34
#define	 LDC1_REGS				0x39


/* Definitions of CFG register																						*/
#define	CFG_BANK_MASK					(unsigned char)(3 << 0)
#define	CFG_DIR_EN					  	(unsigned char)(1 << 4)
#define	CFG_RXCON_EN					(unsigned char)(1 << 5)
#define	CFG_AGC_EN						(unsigned char)(1 << 6)
#define	CFG_OOK_EN						(unsigned char)(1 << 7)

/* Definitions of RC register                                                                 	*/
#define	RC_RST_LL						(unsigned char)(1 << 0)
#define	RC_FSYCK_EN						(unsigned char)(1 << 1)
#define	RC_FSYCK_DIV_MASK				(unsigned char)(3 << 2)
#define	RC_XCLK_EN						(unsigned char)(1 << 4)
#define	RC_XCLK_RDY						(unsigned char)(1 << 5)
#define	RC_FSYCK_RDY					(unsigned char)(1 << 6)
#define	RC_PWRON						(unsigned char)(1 << 7)

/* Definitions of IRQ register                                                                 	*/
#define	IRQ_POLARITY					(unsigned char)(1 << 0)
#define	IRQ_CLR_POR						(unsigned char)(1 << 1)
#define	IRQ_RXDETS_MASK					(unsigned char)(3 << 2)
#define	IRQ_RXCRCF						(unsigned char)(1 << 4)
#define	IRQ_RXFFOW						(unsigned char)(1 << 6)
#define	IRQ_RXTO						(unsigned char)(1 << 7)
/* Definitions of Interrupt control register                                                   	*/
#define	IRQ_TXCMPIE						(unsigned char)(1 << 0)
#define	IRQ_RXCMPIE						(unsigned char)(1 << 1)
#define	IRQ_CALCMPIE					(unsigned char)(1 << 2)
#define	IRQ_RXDETIE						(unsigned char)(1 << 3)
#define	IRQ_RXERRIE						(unsigned char)(1 << 4)
#define	IRQ_FIFOLTIE					(unsigned char)(1 << 5)
#define	IRQ_ATRCTIE						(unsigned char)(1 << 6)
#define	IRQ_ARKTFIE						(unsigned char)(1 << 7)
/* Definitions of Interrupt status register                                                   	*/
#define	IRQ_TXCMPIF						(unsigned char)(1 << 0)
#define	IRQ_RXCMPIF						(unsigned char)(1 << 1)
#define	IRQ_CALCMPIF					(unsigned char)(1 << 2)
#define	IRQ_RXDETIF						(unsigned char)(1 << 3)
#define	IRQ_RXERRIF						(unsigned char)(1 << 4)
#define	IRQ_FIFOLTIF					(unsigned char)(1 << 5)
#define	IRQ_ATRCTIF						(unsigned char)(1 << 6)
#define	IRQ_ARKTFIF						(unsigned char)(1 << 7)
/* Definitions of IO control register                                                   			*/
#define	IO_GIO1S_MASK					(unsigned char)(7 << 0)
#define	IO_GIO2S_MASK					(unsigned char)(7 << 3)
#define	IO_PADDS_MASK					(unsigned char)(3 << 6)
#define	IO_GIO3S_MASK					(unsigned char)(0x0F << 0)
#define	IO_GIO4S_MASK					(unsigned char)(0x0F << 4)
#define	IO_GIO1PU						(unsigned char)(1 << 1)
#define	IO_GIO2PU						(unsigned char)(1 << 2)
#define	IO_GIO3PU						(unsigned char)(1 << 3)
#define	IO_SPIPU						(unsigned char)(1 << 6)
#define	IO_SDO_TEN						(unsigned char)(1 << 7)
/* Definitions of FIFO control register                                                   		*/
#define	FIFO_FFMG_MASK					(unsigned char)(3 << 0)
#define	FIFO_FFMG_EN					(unsigned char)(1 << 2)
#define	FIFO_FFINF_EN					(unsigned char)(1 << 3)
#define	FIFO_RXPL2F_EN					(unsigned char)(1 << 4)
/* Definitions of Packet control register                                                   		*/
#define	PKT_SYNCLEN_MASK				(unsigned char)(3 << 2)
#define	PKT_WHTFMT0						(unsigned char)(1 << 4)
#define	PKT_TRAILER_EN					(unsigned char)(1 << 5)
#define	PKT_TXPID_MASK					(unsigned char)(3 << 6)

#define	PKT_PLHF_EN						(unsigned char)(1 << 0)
#define	PKT_PLHLEN						(unsigned char)(1 << 1)
#define	PKT_PLHAC_EN					(unsigned char)(1 << 2)
#define	PKT_PLLEN_EN					(unsigned char)(1 << 3)
#define	PKT_CRCFMT						(unsigned char)(1 << 4)
#define	PKT_CRC_EN						(unsigned char)(1 << 5)
#define	PKT_FEC_EN						(unsigned char)(1 << 6)
#define	PKT_MCH_EN						(unsigned char)(1 << 7)

#define	PKT_WHTSD_MASK					(unsigned char)(0x7F << 0)
#define	PKT_WHT_EN						(unsigned char)(1 << 7)

#define	PKT_DLYTXS_MASK					(unsigned char)(7 << 0)
#define	PKT_DLYRXS_MASK					(unsigned char)(7 << 3)
#define	PKT_RXPID_MASK					(unsigned char)(3 << 6)

#define	PKT_PMLP_EN						(unsigned char)(1 << 0)
#define	PKT_PMLPLEN						(unsigned char)(1 << 1)
#define	PKT_SYNCLENLB					(unsigned char)(1 << 2)
#define	PKT_CRCINV						(unsigned char)(1 << 3)
#define	PKT_CRCBITO						(unsigned char)(1 << 4)
#define	PKT_CRCBYTEO					(unsigned char)(1 << 5)
#define	PKT_WHTFMT1						(unsigned char)(1 << 6)

#define	PKT_OOKDT_EN					(unsigned char)(1 << 0)
#define	PKT_OOKDT_POR					(unsigned char)(1 << 1)
#define	PKT_OOKDT_MASK					(unsigned char)(0x0F << 2)
#define	PKT_WHTSD87_MASK				(unsigned char)(3 << 6)

/* Definitions of Modulator control register                                                   	*/
#define	MOD_DTR8						(unsigned char)(1 << 0)
#define	MOD_DITHER_MASK					(unsigned char)(3 << 2)
#define	MOD_RXIFOSHH_MASK				(unsigned char)(0x0F << 4)

/* Definitions of Demodulator Control register                                               	*/
#define	DM2_CFO_EN0						(unsigned char)(1 << 6)
#define	DM2_CFO_EN1						(unsigned char)(1 << 7)
#define	DM4_THOLD_MASK					(unsigned char)(0x0F << 4)

/* Definitions of Operation Mode control register                                              	*/
#define	OM_SX_EN						(unsigned char)(1 << 0)
#define	OM_RTX_SEL						(unsigned char)(1 << 1)
#define	OM_RTX_EN						(unsigned char)(1 << 2)
#define	OM_ACAL_EN						(unsigned char)(1 << 3)
#define	OM_BAND_MASK					(unsigned char)(3 << 5)

/* Definitions of Status control register                                              			*/
#define	STA_OMST_MASK					(unsigned char)(7 << 0)
#define	STA_CD_FLAG						(unsigned char)(1 << 4)

/* Definitions of Auto TX/RX control register                                            			*/
#define	ATR_ENABLE						(unsigned char)(1 << 0)
#define	ATR_MODE_MASK					(unsigned char)(3 << 1)
#define	ATR_CTMS						(unsigned char)(1 << 3)
#define	ATR_TUNIT						(unsigned char)(1 << 4)
#define	ATR_CLKSS						(unsigned char)(1 << 5)
#define	ATR_CLKDIV_MASK					(unsigned char)(3 << 6)
#define	ATR_COMPATIBLE					(unsigned char)(1 << 7)

/* Definitions of Auto packet repeat control register                                    			*/
#define	ARK_ENABLE						(unsigned char)(1 << 0)
#define	ATR_WDLY_MASK					(unsigned char)(3 << 1)
#define	ARK_RPNM_MASK					(unsigned char)(0x0F << 4)

/* Definitions of OOK duty tune,packet control register15                       						*/
#define	OOKDT_ENABLE					(unsigned char)(1 << 0)
#define	OOKDT_POLARITY					(unsigned char)(1 << 1)
#define	OOKDT_TUNE_MASK					(unsigned char)(0x0F << 2)

/* Definitions of XO control register                                    								*/
#define	XO_TRIM_MASK					(unsigned char)(0x1F << 0)
#define	XO_SHIFT_MASK					(unsigned char)(3 << 6)
#define	XO_DIV2							(unsigned char)(1 << 3)
#define	XO_EFUSE_TRIM              		(unsigned char)(1 << 4)

/* Definitions of LIRC control register                                   								*/
#define	LIRC_ENABLE						(unsigned char)(1 << 0)
#define	LIRC_TRIM_MASK					(unsigned char)(0x1F << 1)
#define	LIRC_OVERWRITE					(unsigned char)(1 << 6)
#define	LIRC_CAL_EN						(unsigned char)(1 << 7)

/* Definitions of eFuse control register                                 								*/
#define	EFC_EN							(unsigned char)(1 << 0)
#define	EFC_RW							(unsigned char)(1 << 1)
#define	EFC_CHECK						(unsigned char)(1 << 2)
#define	EFC_OW							(unsigned char)(1 << 4)
#define	EFC_XOTS						(unsigned char)(1 << 5)
#define	EFC_READY						(unsigned char)(1 << 6)
#define	EFC_PDCMP						(unsigned char)(1 << 7)

#endif




