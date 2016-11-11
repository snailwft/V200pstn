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
		HT9370_TOE_SET(user); //ʹ��			
		READ_DTMF_INPUT;  // ���ó����� ������dtmf��
		//uc_dtmf = READ_DTMF();
		uc_dtmf = GET_BITS(LPC_GPIO0, DATA);
		READ_DTMF_OUTPUT; // ���ó������������dtmf��
		HT9370_TOE_CLR(user); //����
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