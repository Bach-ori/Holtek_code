#ifndef BC3603_CONFIG_TABLE_H
#define BC3603_CONFIG_TABLE_H

/*------------------------------------------------------------------*/
/*							Header									*/
/*------------------------------------------------------------------*/
#include "BC3603_CMD_REG.h"
#include "rf_parameter.h"
/*------------------------------------------------------------------*/
/*						  Calculate									*/
/*------------------------------------------------------------------*/
#define		ATR_Cycle			(((RF_ATR_Cycle*1024L)/125)-1)
#define		ATR_Cycle_low		(ATR_Cycle&0xff)
#define		ATR_Cycle_high		((ATR_Cycle>>8)&0xff)
#define		ATR_RX_Window		(RF_ATR_RX_Window/250L)
#define		ATR_RX_Window_low	(ATR_RX_Window&0xff)
#define		ATR_RX_Window_high	((ATR_RX_Window>>8)&0x07)
#define		ATR_RX_extra		(RF_ATR_RX_extra*4L)
#define		ATR_RX_extra_low	(ATR_RX_extra&0xff)
#define		ATR_RX_extra_high	((ATR_RX_extra>>8)&0xff)
#define		ARK_RX_Window		(RF_ARK_RX_Window/250L)

/*------------------------------------------------------------------*/
/*							Define									*/
/*------------------------------------------------------------------*/
//	BC3601 IRQ/IO Configure	//
const unsigned short IRQIO_REGS_TABLE[]=
{
	((	IO2_REGS	<< 8)	+	0x00	),	//b[3:0]=GIO3S[3:0]
#if (RF_SPI_line!=3)
	((	IO1_REGS	<< 8)	+	0x69	),	//b[7:6]=PADDS[1:0],b[5:3]=GIO2S[2:0],b[2:0]=GIO1S[2:0]
	((	IO3_REGS	<< 8)	+	0xF9	),	//b7=SDO_TEN,b6=SPIPU,b[5:4]=reserved,b[3:1]=GIOPU[3:1],b0=reserved
#else
	((	IO1_REGS	<< 8)	+	0x45	),	//b[7:6]=PADDS[1:0],b[5:3]=GIO2S[2:0],b[2:0]=GIO1S[2:0]
#endif
	((	IRQ1_REGS	<< 8)	+	0x0E	),	//b7=RXTO, b6=RXFFOW, b4=RXCRCF, b[3:2]=RXDETS[1:0], b1=IRQCPOR, b0=IRQPOR
	((	IRQ2_REGS	<< 8)	+	0x03	),	//b7=ARKTFIE, b6=ARTCIE, b5=FIFOLTIE, b4=RXERRIE, b3=RXDETIE, b2=CALCMPIE, b1=RXCMPIE, b0=TXCMPIE
	((	IRQ3_REGS	<< 8)	+	0xFF	),	//b7=ARKTFIF, b6=ARTCIF, b5=FIFOLTIF, b4=RXERRIF, b3=RXDETIF, b2=CALCMPIF, b1=RXCMPIF, b0=TXCMPIF
};

#define	RF_SYNCLEN_					((RF_SYNC_Length-1)>>1)
#define	RF_SYNCLENLB_				((RF_SYNC_Length-1)&0x01)
#define	RF_WHTFMT1_					((RF_whitening_Format)>>1)
#define	RF_WHTFMT0_					((RF_whitening_Format)&0x01)

#define	RF_TX_PREAMBLE_PATTERN_H	((RF_TX_PREAMBLE_PATTERN>>8)&0xFF)
#define	RF_TX_PREAMBLE_PATTERN_L	((RF_TX_PREAMBLE_PATTERN)&0xFF)

#define	RF_CRC_SEED_H				((RF_CRC_SEED>>8)&0xFF)
#define	RF_CRC_SEED_L				((RF_CRC_SEED)&0xFF)


#define	RF_PLHEA					((RF_PLHA_Address>>8)&0xFF)
#define	RF_PLHA						((RF_PLHA_Address)&0x3F)


//	BC3603 packet format Configure	//
/* CRC_EN=1, PLLEN_EN=0, PAYLOAD = 16-byte */
const unsigned short PACKET_REGS_TABLE[]=
{
	((	FIFO1_REGS	<< 8)	+	0x00	),	//b[5:0]=TXFFSA[5:0]
	((	FIFO2_REGS	<< 8)	+	0x01	),	//b4=RXPL2F_EN,b3=FFINF_EN,b2=FFMG_EN,b[1:0]=FFMG[1:0]
	((	PKT1_REGS	<< 8)	+	RF_TxPreamble_Length	),	//TXPMLEN[7:0] (+1)
	((	PKT2_REGS	<< 8)	+	((RF_Trailer_EN<<5)|(RF_WHTFMT0_<<4)|(RF_SYNCLEN_<<2))),	//b[7:6]=PID[1:0],b[5]=TRAILER_EN,b[4]=WHTFMT[0],b[3:2]=SYNCLEN[1:0],b[1:0]=reserved
//	((	PKT2_REGS	<< 8)	+	(0x08|(RF_Trailer_EN<<5))	),
	#if	RF_ARK_Enable == 1
	((	PKT3_REGS	<< 8)	+	(0x00|(RF_Manchester_EN<<7)|(RF_FEC_EN<<6)|(RF_ARK_Enable<<5)|(RF_CRC_Format<<4)|(RF_ARK_Enable<<3)|(RF_PLHAC_EN<<2)|(RF_Header_Length<<1)|RF_ARK_Enable)	),	//b7=MCH_EN, b6=FEC_EN, b5=CRC_EN, b4=CRCFMT, b3=PLLEN_EN, b2=PLHAC_EN, b1=PLHLEN, b0=PLH_EN
	#else
	((	PKT3_REGS	<< 8)	+	(0x00|(RF_Manchester_EN<<7)|(RF_FEC_EN<<6)|(RF_CRC_EN<<5)|(RF_CRC_Format<<4)|(RF_PLLEN_EN<<3)|(RF_PLHAC_EN<<2)|(RF_Header_Length<<1)|RF_Header_EN)	),	//b7=MCH_EN, b6=FEC_EN, b5=CRC_EN, b4=CRCFMT, b3=PLLEN_EN, b2=PLHAC_EN, b1=PLHLEN, b0=PLH_EN
	#endif
	((	PKT4_REGS	<< 8)	+	(0x00|(RF_whitening_EN<<7)|RF_whitening_Seed)	),	//b7=WHT_EN, b[6:0]=WHTSD[6:0]
	((	PKT5_REGS	<< 8)	+	RF_Payload_Length	),	//TXDLEN
	#if RF_ARK_Enable==1
	((	PKT6_REGS	<< 8)	+	0x00	),				//RXDLEN
	#else
	((	PKT6_REGS	<< 8)	+	RF_Payload_Length	),	//RXDLEN
	#endif
	((	PKT7_REGS	<< 8)	+	0x27	),	//b[7:6]=RXPID[1:0], b[5:3]=DLY_RXS[2:0], b[2:0]=DLY_TXS[2:0]
	((	PKT8_REGS	<< 8)	+	RF_PLHA	),	//b[5:0]=PLHA[5:0]
	((	PKT9_REGS	<< 8)	+	RF_PLHEA	),	//b[7:0]=PLHEA[7:0]
	((	PKT10_REGS	<< 8)	+	((RF_WHTFMT1_<<6)|(RF_CRC_BYTE_ORDER<<5)|(RF_CRC_BIT_ORDER<<4)|(RF_CRC_INVERTED<<3)|(RF_SYNCLENLB_<<2)|(RF_TX_PREAMBLE_CLASS<<1)|(RF_Preamble_Pattern_EN))),//b[7:]=reserved,b[6]=WHTFMT[1],b[5]=CRCBYTEO,b[4]=CRCBITO,b[3]=CRCINV,b[2]=SYNCLENLB,b[1]=PMLPLEN,b[0]=PMLP_EN
	((	PKT11_REGS	<< 8)	+	RF_TX_PREAMBLE_PATTERN_L	),	//b[7:0]=PMLPAT[7:0]
	((	PKT12_REGS	<< 8)	+	RF_TX_PREAMBLE_PATTERN_H	),	//b[7:0]=PMLPAT[15:8]
	((	PKT13_REGS	<< 8)	+	RF_CRC_SEED_L	),	//b[7:0]=CRCSD[7:0]
	((	PKT14_REGS	<< 8)	+	RF_CRC_SEED_H	),	//b[7:0]=CRCSD[15:8]
	((	PKT15_REGS	<< 8)	+	0x15	),	// RF_CRC_SEED_H	b[7:6]=WHTSD[8:7] 
	((	XO1_REGS	<< 8)	+	(RF_CYL_COARSE_TUNE_<<6|RF_CYL_FINE_TUNE_)),	//b[7:6]=XSHIFT[1:0] b[4:0]= XO_TRIM[4:0]
};

//	BC3603 common Configure	//
const unsigned short COMMON_REGS_TABLE[]=
{
	((	CFG1_REGS	<< 8)	+	0x40	),	//b[5:0]=TXFFSA[5:0]
};

//	BC3603 Bank0 Configure	//
const unsigned short BANK0_REGS_TABLE[]=
{
	((	ATR1_REGS	<< 8)	+	0x42	),	//_ATR1_		ATRCLK = 8192Hz / WOR
	((	ATR2_REGS	<< 8)	+	ATR_Cycle_low	),	//_ATR2_
	((	ATR3_REGS	<< 8)	+	ATR_Cycle_high	),	//_ATR3_	ATR Cycle = 0x8000 * 0.125 = 4s
	((	ATR4_REGS	<< 8)	+	ATR_RX_Window_low	),	//_ATR4_	RX cycle = (0x25+1) * 250us = 9.5ms
	((	ATR5_REGS	<< 8)	+	ATR_RX_extra_low	),	//_ATR5_	
	((	ATR6_REGS	<< 8)	+	ATR_RX_extra_high	),	//_ATR6_	RX extra time = (0x3E7+1) * 250us = 250ms
	((	ATR7_REGS	<< 8)	+	(((0x02|((RF_ARK_Resend_Count)<<4)))|RF_ARK_Enable	)),	//_ATR7_
	((	ATR8_REGS	<< 8)	+	(ARK_RX_Window&0xFF)		),	//_ATR8_	RX cycle = (ARK_RX_Window+1) * 250us = ms
	((	ATR11_REGS	<< 8)	+	ATR_RX_Window_high		),
//	((	XO1_REGS	<< 8)	+	0x40	),	//
	((	XO2_REGS	<< 8)	+	0x03	),	//
	((	XO3_REGS	<< 8)	+	0x20	),	//
	((	TX2_REGS	<< 8)	+	0x2A	),	//
};

//	BC3603 Bank1 Configure	//
const unsigned short BANK1_REGS_TABLE[]=
{
	((	AGC2_REGS	<< 8)	+	0x14	),	//
};

//	BC3603 Bank2 Configure	//
const unsigned short BANK2_REGS_TABLE[]=
{
#if	RF_BAND == _315MHz_	
	((	RXG_REGS	<< 8)	+	0X97	),
	((	RX1_REGS	<< 8)	+	0x68	),	
#if	(RF_Datarate==_125kbps_	||RF_Datarate==_250kbps_)
	((	RX2_REGS	<< 8)	+	0x16	),
#else
	((	RX2_REGS	<< 8)	+	0x06	),
#endif	
	((	TX3_REGS	<< 8)	+	0x41	),
	((	CA1_REGS	<< 8)	+	0x90	),
	((	LDC1_REGS	<< 8)	+	0x9C	),
#endif	
#if	RF_BAND==_433MHz_	
	((	RXG_REGS	<< 8)	+	0X97	),
	((	RX1_REGS	<< 8)	+	0x68	),	
#if	(RF_Datarate==_125kbps_	||RF_Datarate==_250kbps_)
	((	RX2_REGS	<< 8)	+	0x16	),
#else
	((	RX2_REGS	<< 8)	+	0x06	),
#endif	
	((	TX3_REGS	<< 8)	+	0x41	),
	((	CA1_REGS	<< 8)	+	0x90	),
	((	LDC1_REGS	<< 8)	+	0x9C	),
#endif	
#if	RF_BAND==_470MHz_	
	((	RXG_REGS	<< 8)	+	0X97	),
	((	RX1_REGS	<< 8)	+	0x68	),	
#if	(RF_Datarate==_125kbps_	||RF_Datarate==_250kbps_)
	((	RX2_REGS	<< 8)	+	0x16	),
#else
	((	RX2_REGS	<< 8)	+	0x06	),
#endif	
	((	TX3_REGS	<< 8)	+	0x41	),
	((	CA1_REGS	<< 8)	+	0x90	),
	((	LDC1_REGS	<< 8)	+	0x9C	),	
#endif	
#if	RF_BAND==_868MHz_	
	((	RXG_REGS	<< 8)	+	0X97	),
	((	RX1_REGS	<< 8)	+	0x68	),	
#if	(RF_Datarate==_125kbps_	||RF_Datarate==_250kbps_)
	((	RX2_REGS	<< 8)	+	0x96	),
#else
	((	RX2_REGS	<< 8)	+	0x86	),
#endif	
	((	TX3_REGS	<< 8)	+	0x41	),
	((	CA1_REGS	<< 8)	+	0x90	),
	((	LDC1_REGS	<< 8)	+	0x9C	),	
#endif	
#if	RF_BAND==_915MHz_	
	((	RXG_REGS	<< 8)	+	0X97	),
	((	RX1_REGS	<< 8)	+	0x68	),	
#if	(RF_Datarate==_125kbps_	||RF_Datarate==_250kbps_)
	((	RX2_REGS	<< 8)	+	0x96	),
#else
	((	RX2_REGS	<< 8)	+	0x86	),
#endif
	((	TX3_REGS	<< 8)	+	0x41	),
	((	CA1_REGS	<< 8)	+	0x90	),
	((	LDC1_REGS	<< 8)	+	0x9C	),	
#endif	
	((	0x3B	   << 8)	+	0x5D	),

};

//	BC3603 SYNCWORD	Configure	//
const unsigned char BC3603_SYNCWORD[8]=
{
		(unsigned char)RF_SYNC_L,
		(unsigned char)(RF_SYNC_L>>8),
		(unsigned char)(RF_SYNC_L>>16),
		(unsigned char)(RF_SYNC_L>>24),
		(unsigned char)RF_SYNC_H,
		(unsigned char)(RF_SYNC_H>>8),
		(unsigned char)(RF_SYNC_H>>16),
		(unsigned char)(RF_SYNC_H>>24)		
};



//	BC3603 TX Power value	//
const unsigned char TxPowerValue[5][5]=
{
	/*0dBm,10dBm,13dBm,17dBm,19dBm */
	{	0x18,0x29,0x32,0x38, 0x3B	},
	{	0x1A,0x2a,0x31,0x37, 0x3C	},//433
	{	0x16,0x2B,0x33,0x39, 0x3C	},
	{	0x15,0x29,0x32,0x39, 0x3C	},//868
	{	0x1C,0x30,0x35,0x3A, 0x3D	},	//915
};


	
#if	(RF_FREQ <= 35000) 
	#define	_RF_BAND_	0
	#define _ODDIV_		4
#elif (RF_FREQ <= 45000) 
	#define	_RF_BAND_	1
	#define _ODDIV_		2
#elif (RF_FREQ <= 51000) 
	#define	_RF_BAND_	2
	#define _ODDIV_		2
#elif (RF_FREQ <= 93000) 
	#define	_RF_BAND_	3
	#define _ODDIV_		1
#endif

#define	_OM_	(_RF_BAND_ << 5)

#define	_DN_	((RF_FREQ*_ODDIV_)/XTAL_FREQ/100)		//D_N[6:0}

#define _FDKH_	(((RF_FREQ*_ODDIV_)-(_DN_*XTAL_FREQ*100))*16)
#define _DKH_	(_FDKH_/XTAL_FREQ/100)

#define _FDKM_	((_FDKH_-(_DKH_*XTAL_FREQ*100))*256)
#define _DKM_	(_FDKM_/XTAL_FREQ/100)

#define _FDKL_	((_FDKM_-(_DKM_*XTAL_FREQ*100))*256)
#define _DKL_	(_FDKL_/XTAL_FREQ/100)

#define	_D_N_	_DN_
#define	_D_KL_	_DKL_
#define	_D_KM_	_DKM_
#define	_D_KH_	_DKH_

const unsigned short Frequency_REGS_TABLE[]=
{
	(OM_REGS << 8) 		+ _OM_,		// OM,Band 
	(SX1_REGS << 8) 	+ _D_N_,	// SX1,D_N 
	(SX2_REGS << 8) 	+ _D_KL_,	// SX2,D_K low 
	(SX3_REGS << 8) 	+ _D_KM_,	// SX3,D_K middle 
	(SX4_REGS << 8) 	+ _D_KH_	// SX4,D_K high 	
};

//	BC3603 RF_Datarate	Configure//
/**********************************************/
#if RF_Datarate==_2kbps_
/**********************************************/
const unsigned short DM_REGS_TABLE[]=
{
	((	MOD1_REGS	<< 8)	+	0xF9		),	//
	((	MOD2_REGS	<< 8)	+	0x60	),	//
	((	MOD3_REGS	<< 8)	+	0x67	),	//
	((	DM1_REGS	<< 8)	+	0x31	),	//
	((	DM2_REGS	<< 8)	+	0x09	),	//
	((	DM3_REGS	<< 8)	+	0xE6	),	//
	((	DM5_REGS	<< 8)	+	0x1F	),	//
	((	DM8_REGS	<< 8)	+	0x05	),	//
};
const unsigned short FCF_REGS_TABLE[]=
{
	((	AGC1_REGS	<< 8)	+	0x00	),	//	
	((	AGC3_REGS	<< 8)	+	0x04	),	//		
	((	AGC7_REGS	<< 8)	+	0x30	),	//		
	((	FCF1_REGS	<< 8)	+	0x30	),	//
	((	FCF2_REGS	<< 8)	+	0x20	),	//
	((	FCF3_REGS	<< 8)	+	0x00	),	//
	((	FCF4_REGS	<< 8)	+	0x00	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x00	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x02	),	//
	((	FCF9_REGS	<< 8)	+	0x03	),	//
	((	FCF10_REGS	<< 8)	+	0x00	),	//
	((	FCF11_REGS	<< 8)	+	0x00	),	//
	((	FCF12_REGS	<< 8)	+	0x00	),	//
	((	FCF13_REGS	<< 8)	+	0x00	),	//
	((	FCF14_REGS	<< 8)	+	0x00	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x00	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x00	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TABLE[]=
{
#if	(RF_BAND==_315MHz_ || RF_BAND==_433MHz_ || RF_BAND==_470MHz_)
	((	RX2_REGS	<< 8)	+	0x06	),	//	
#else
	((	RX2_REGS	<< 8)	+	0x86	),	//	
#endif	
};
#endif
/**********************************************/
#if RF_Datarate==_10kbps_
/**********************************************/
const unsigned short DM_REGS_TABLE[]=
{
	((	MOD1_REGS	<< 8)	+	0x31		),	//
	((	MOD2_REGS	<< 8)	+	0x60	),	//
	((	MOD3_REGS	<< 8)	+	0x66	),	//
	((	DM1_REGS	<< 8)	+	0x09	),	//
	((	DM2_REGS	<< 8)	+	0xC9	),	//
	((	DM3_REGS	<< 8)	+	0xE6	),	//
	((	DM5_REGS	<< 8)	+	0x1F	),	//
	((	DM8_REGS	<< 8)	+	0x1A	),	//
};
const unsigned short FCF_REGS_TABLE[]=
{
	((	AGC1_REGS	<< 8)	+	0x00	),	//	
	((	AGC3_REGS	<< 8)	+	0x04	),	//		
	((	AGC7_REGS	<< 8)	+	0x30	),	//
	((	FCF1_REGS	<< 8)	+	0x10	),	//
	((	FCF2_REGS	<< 8)	+	0xA4	),	//
	((	FCF3_REGS	<< 8)	+	0x00	),	//
	((	FCF4_REGS	<< 8)	+	0x00	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x00	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x10	),	//
	((	FCF9_REGS	<< 8)	+	0x03	),	//
	((	FCF10_REGS	<< 8)	+	0x00	),	//
	((	FCF11_REGS	<< 8)	+	0x00	),	//
	((	FCF12_REGS	<< 8)	+	0x00	),	//
	((	FCF13_REGS	<< 8)	+	0x00	),	//
	((	FCF14_REGS	<< 8)	+	0x00	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x00	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x00	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TABLE[]=
{
#if	(RF_BAND==_315MHz_ || RF_BAND==_433MHz_ || RF_BAND==_470MHz_)
	((	RX2_REGS	<< 8)	+	0x06	),	//	
#else
	((	RX2_REGS	<< 8)	+	0x86	),	//	
#endif
};
#endif
/**********************************************/
#if RF_Datarate==_50kbps_
/**********************************************/
const unsigned short DM_REGS_TABLE[]=
{
	((	MOD1_REGS	<< 8)	+	0x09	),	//
	((	MOD2_REGS	<< 8)	+	0x60	),	//
	((	MOD3_REGS	<< 8)	+	0x67	),	//
	((	DM1_REGS	<< 8)	+	0x13	),	//
	((	DM2_REGS	<< 8)	+	0xC0	),	//
	((	DM3_REGS	<< 8)	+	0xE0	),	//
	((	DM5_REGS	<< 8)	+	0x30	),	//
	((	DM8_REGS	<< 8)	+	0x0D	),	//
};
const unsigned short FCF_REGS_TABLE[]=
{
//	((	AGC1_REGS	<< 8)	+	0x00	),	//	
	((	AGC3_REGS	<< 8)	+	0x04	),	//		
	((	AGC7_REGS	<< 8)	+	0x30	),	//	
	((	FCF1_REGS	<< 8)	+	0x00	),	//
	((	FCF2_REGS	<< 8)	+	0x4C	),	//
	((	FCF3_REGS	<< 8)	+	0x00	),	//
	((	FCF4_REGS	<< 8)	+	0x00	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x00	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x00	),	//
	((	FCF9_REGS	<< 8)	+	0x00	),	//
	((	FCF10_REGS	<< 8)	+	0x00	),	//
	((	FCF11_REGS	<< 8)	+	0x00	),	//
	((	FCF12_REGS	<< 8)	+	0x00	),	//
	((	FCF13_REGS	<< 8)	+	0x00	),	//
	((	FCF14_REGS	<< 8)	+	0x00	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x00	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x00	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TABLE[]=
{
#if	(RF_BAND==_315MHz_ || RF_BAND==_433MHz_ || RF_BAND==_470MHz_)
	((	RX2_REGS	<< 8)	+	0x06	),	//	
#else
	((	RX2_REGS	<< 8)	+	0x86	),	//	
#endif	
};
#endif
/**********************************************/
#if RF_Datarate==_125kbps_
/**********************************************/
const unsigned short DM_REGS_TABLE[]=
{
	((	MOD1_REGS	<< 8)	+	0x03	),	//
	((	MOD2_REGS	<< 8)	+	0x90	),	//
	((	MOD3_REGS	<< 8)	+	0x9A	),	//
	((	DM1_REGS	<< 8)	+	0x07	),	//
	((	DM2_REGS	<< 8)	+	0xC0	),	//
	((	DM3_REGS	<< 8)	+	0xE0	),	//
	((	DM5_REGS	<< 8)	+	0x30	),	//
	((	DM8_REGS	<< 8)	+	0x20	),	//
};
const unsigned short FCF_REGS_TABLE[]=
{
	((	AGC1_REGS	<< 8)	+	0x1C	),	//	
	((	AGC3_REGS	<< 8)	+	0x07	),	//		
	((	AGC7_REGS	<< 8)	+	0x50	),	//	
	((	FCF1_REGS	<< 8)	+	0x00	),	//
	((	FCF2_REGS	<< 8)	+	0x19	),	//
	((	FCF3_REGS	<< 8)	+	0x01	),	//
	((	FCF4_REGS	<< 8)	+	0x1D	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x46	),	//
	((	FCF7_REGS	<< 8)	+	0x03	),	//
	((	FCF8_REGS	<< 8)	+	0x22	),	//
	((	FCF9_REGS	<< 8)	+	0x00	),	//
	((	FCF10_REGS	<< 8)	+	0x31	),	//
	((	FCF11_REGS	<< 8)	+	0x03	),	//
	((	FCF12_REGS	<< 8)	+	0x86	),	//
	((	FCF13_REGS	<< 8)	+	0x03	),	//
	((	FCF14_REGS	<< 8)	+	0x12	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x08	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x08	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TABLE[]=
{
#if	(RF_BAND==_315MHz_ || RF_BAND==_433MHz_ || RF_BAND==_470MHz_)
	((	RX2_REGS	<< 8)	+	0x16	),	//	
#else
	((	RX2_REGS	<< 8)	+	0x96	),	//	
#endif	
};
#endif
/**********************************************/
#if RF_Datarate==_250kbps_
/**********************************************/
const unsigned short DM_REGS_TABLE[]=
{
	((	MOD1_REGS	<< 8)	+	0x01	),	//
	((	MOD2_REGS	<< 8)	+	0x90	),	//
	((	MOD3_REGS	<< 8)	+	0x9A	),	//
	((	DM1_REGS	<< 8)	+	0x03	),	//
	((	DM2_REGS	<< 8)	+	0x40	),	//
	((	DM3_REGS	<< 8)	+	0x40	),	//
	((	DM5_REGS	<< 8)	+	0x30	),	//
	((	DM8_REGS	<< 8)	+	0x40	),	//
};
const unsigned short FCF_REGS_TABLE[]=
{
	((	AGC1_REGS	<< 8)	+	0x18	),	//	
	((	AGC3_REGS	<< 8)	+	0x04	),	//		
	((	AGC7_REGS	<< 8)	+	0x50	),	//	
	((	FCF1_REGS	<< 8)	+	0x06	),	//
	((	FCF2_REGS	<< 8)	+	0x94	),	//
	((	FCF3_REGS	<< 8)	+	0x02	),	//
	((	FCF4_REGS	<< 8)	+	0xCA	),	//
	((	FCF5_REGS	<< 8)	+	0x02	),	//
	((	FCF6_REGS	<< 8)	+	0x62	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x58	),	//
	((	FCF9_REGS	<< 8)	+	0x03	),	//
	((	FCF10_REGS	<< 8)	+	0xE9	),	//
	((	FCF11_REGS	<< 8)	+	0x03	),	//
	((	FCF12_REGS	<< 8)	+	0xB3	),	//
	((	FCF13_REGS	<< 8)	+	0x03	),	//
	((	FCF14_REGS	<< 8)	+	0x3E	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0xE9	),	//
	((	FCF17_REGS	<< 8)	+	0x03	),	//
	((	FCF18_REGS	<< 8)	+	0x39	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TABLE[]=
{
#if	(RF_BAND==_315MHz_ || RF_BAND==_433MHz_ || RF_BAND==_470MHz_)
	((	RX2_REGS	<< 8)	+	0x16	),	//	
#else
	((	RX2_REGS	<< 8)	+	0x96	),	//	
#endif	
};
#endif

#endif