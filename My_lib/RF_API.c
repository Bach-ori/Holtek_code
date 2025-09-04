/*------------------------------------------------------------------*/
/*							Header									*/
/*------------------------------------------------------------------*/
#include "RF_API.h"
#include "rf_parameter.h"  
#include "SYS.h"
#include "Spi.h"
#include "BC3603.h"
#include "BC3603_CMD_REG.h"
/*------------------------------------------------------------------*/
/*							 Define									*/
/*------------------------------------------------------------------*/      
_BC3603_device_ BC3603_T;

volatile unsigned int Resend_Count;
unsigned char syncword[RF_SYNC_Length];
const unsigned char	margin_len[] = {4, 8, 16, 32};
extern u8 Ngat,j;
/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	RF_Parameter_loading
// input	:	BC3603_T
// output	:	none
//*******************************//
void RF_Parameter_loading(void)
{
	unsigned char mode_sts;
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	mode_sts = BC3603_T.mode;
	BC3603_T.mode = RF_LightSleep;	
	BC3603_T.sync_word_len = RF_SYNC_Length;
	
	BC3603_ReadSyncWord(syncword,RF_SYNC_Length);
	BC3603_T.sync_word = (unsigned char*)syncword;
	
	BC3603_T.data_rate = RF_Datarate;
	BC3603_T.tx_power = RF_TxPower;
	
	BC3603_T.irq_status = BC3603_ReadRegister(IRQ3_REGS);
	
	BC3603_T.tx_preamble_len = BC3603_ReadRegister(PKT1_REGS);
	BC3603_T.tx_packet_len = BC3603_ReadRegister(PKT5_REGS);
	BC3603_T.tx_payload_buffer = RF_TXFIFO;

	BC3603_T.rx_preamble_len = BC3603_ReadRegister(PKT2_REGS) & 0x03;
	BC3603_T.rx_packet_len = BC3603_T.tx_packet_len;
	BC3603_T.rec_data_len = 0;
	BC3603_T.rx_payload_buffer = RF_RXFIFO;
	
	BC3603_T.mode = mode_sts;
}

#if RF_Payload_Length<65
void SimpleFIFO_TX_Process(_BC3603_device_ *BC3603_T)
{
	switch((*BC3603_T).step)
	{
		case 0:
			(*BC3603_T).irq_status = 0;	
			BC3603_StrobeCommand(REST_TX_POS_CMD);													
			BC3603_WriteTxPayload((*BC3603_T).tx_payload_buffer,(*BC3603_T).tx_packet_len);		
			BC3603_StrobeCommand(TX_MODE_CMD);	
			(*BC3603_T).mode = RF_TX;
			(*BC3603_T).step++;
			break;
			
		case 1:
			if(Ngat==1)
			{	
				(*BC3603_T).irq_status = BC3603_GetIRQState();	
				Ngat=0;
			}
			if((*BC3603_T).irq_status & _TXFSHI_)			
			{	
				(*BC3603_T).tx_irq_f = 1;
				BC3603_ClearIRQFlag(_TXFSHI_);
				(*BC3603_T).irq_status &= ~_TXFSHI_;
				(*BC3603_T).step = 0;	

			}
			
			
			if((*BC3603_T).irq_status)
			{
				BC3603_ClearIRQFlag((*BC3603_T).irq_status);
				(*BC3603_T).irq_status = 0;
				(*BC3603_T).step = 0;	
			}			
			break;	
		default: break;	
	}			
}
#endif

//*******************************//
// name		:	ATR_WOR_Process
// input	:	BC3603_T
// output	:	RF_WOR_Mode
//*******************************//
#if RF_ATR_Enable == 1
void ATR_WOR_Process(_BC3603_device_ *BC3603_T)
{
	switch((*BC3603_T).step)
	{
		case 0:	
			BC3603_StrobeCommand(LIGHT_SLEEP_CMD);
			(*BC3603_T).mode = RF_LightSleep;
			RFXtalReady();
			RFEnATR();																		// Enable ATR	
			BC3603_StrobeCommand(IDLE_MODE_CMD);											// Enable Idel mode	
			(*BC3603_T).step++;
			break;
		case 1:
			if(Ngat==1)
			{	
				(*BC3603_T).irq_status = BC3603_GetIRQState();	
				Ngat=0;
			}
			if ((*BC3603_T).irq_status & _RXFSHI_)
			{
			    // Gói đúng CRC
			    (*BC3603_T).rx_irq_f = 1;
			    (*BC3603_T).rec_data_len = BC3603_GetRxPayloadWidth();
			    BC3603_ReadRxPayload((*BC3603_T).rx_payload_buffer, (*BC3603_T).rec_data_len);
			
			    // Reset sau khi xử lý
			    BC3603_ClearIRQFlag(_RXFSHI_);
			    (*BC3603_T).irq_status &= ~_RXFSHI_;
			    BC3603_StrobeCommand(IDLE_MODE_CMD);
			    (*BC3603_T).step = 0;
			}
			
			if ((*BC3603_T).irq_status & _RXFAILI_)
			{
			    // Gói lỗi -> chỉ reset
			    BC3603_ClearIRQFlag(_RXFAILI_);
			    (*BC3603_T).irq_status &= ~_RXFAILI_;
			    BC3603_StrobeCommand(IDLE_MODE_CMD);
			    (*BC3603_T).step = 0;
			}
			
			if ((*BC3603_T).irq_status)
			{
			    BC3603_ClearIRQFlag((*BC3603_T).irq_status);
			    (*BC3603_T).irq_status = 0;
			}
			break;
			
		default: break;	
	}		
}
#endif
