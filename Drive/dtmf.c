#include "dtmf.h"
#include "config.h"

ST_DTMF_RXBUFF dtmf_rx_buf;

void dtmf_rx_buf_init()
{
	memset(&dtmf_rx_buf, 0x0, sizeof(dtmf_rx_buf));
}

void recv_dtmf()
{
	uint8 uc_dtmf = 0xff;
	
	if (GET_BIT(LPC_GPIO1, DATA, 11)) //�ж���û������
	{
		SET_BIT(LPC_GPIO1, DATA, 10); //ʹ��			
		set_dtmf_qn_dir();  //���ó����� ������dtmf��
		uc_dtmf = GET_BITS(LPC_GPIO1, DATA);
		if (GET_BIT(LPC_GPIO1, DATA, 8))
		{
			uc_dtmf |= (1<<3);
		}
		clr_dtmf_qn_dir(); //���ó������������dtmf��
		CLR_BIT(LPC_GPIO1, DATA, 10); //����
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

void dtmf_data_handler()
{
	recv_dtmf();  								//��ô���dtmf���Ե�������
	if (dtmf_rx_buf.rx_addr > 0)
	{
		
	}
}