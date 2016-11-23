#include "dtmf.h"
#include "config.h"

ST_DTMF_RXBUFF dtmf_rx_buf;

void dtmf_rx_buf_init()
{
	memset(dtmf_rx_buf, 0x0, sizeof(dtmf_rx_buf));
}

void recv_dtmf()
{
	uint8 uc_dtmf = 0xff;
	if (READ_STD(user))
	{
		HT9370_TOE_SET(user); //使能			
		READ_DTMF_INPUT;  // 设置成输入 ，接收dtmf码
		//uc_dtmf = READ_DTMF();
		uc_dtmf = GET_BITS(LPC_GPIO0, DATA);
		READ_DTMF_OUTPUT; // 设置成输出，不接收dtmf码
		HT9370_TOE_CLR(user); //禁用
	}
	if (uc_dtmf != 0xFF)
	{
		if( (dtmf_rx_buf.rx_addr < MAX_DTMF_BUFF )
			&& ( uc_dtmf > 0x00 ) && ( uc_dtmf < 0x0D ) )
		{
			dtmf_rx_buf.dtmf_buff[dtmf_rx_buf.rx_addr++] = uc_dtmf;
		}
	}		
}