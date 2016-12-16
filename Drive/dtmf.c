#include "dtmf.h"
#include "config.h"

ST_DTMF_RXBUFF dtmf_rx_buf;
extern uint8 dtmf_flag, dtmf_time;

void dtmf_rx_buf_init()
{
	memset(&dtmf_rx_buf, 0x0, sizeof(dtmf_rx_buf));
}

int get_dtmf_rx_buf_length()
{
	return dtmf_rx_buf.rx_addr;
}


void recv_dtmf()
{
	uint8 uc_dtmf = 0xff, ch;

	if (!GET_BIT(LPC_GPIO1, DATA, 11)) //�ж���û������
	{
		dtmf_flag = 1;
		time16b1_enable();
		SET_BIT(LPC_GPIO1, DATA, 10); //ʹ��			
		set_dtmf_qn_dir();  //���ó����� ������dtmf��
		uc_dtmf = GET_BITS(LPC_GPIO1, DATA);
		if (GET_BIT(LPC_GPIO1, DATA, 8))
		{
			uc_dtmf |= (1<<3);
		}	
		clr_dtmf_qn_dir(); //���ó������������dtmf��
		while (!GET_BIT(LPC_GPIO1, DATA, 11));
		CLR_BIT(LPC_GPIO1, DATA, 10); // ����ط��ѿ��عر��˵���û���������
	}
	if (uc_dtmf != 0xFF)
	{
		if (dtmf_rx_buf.rx_addr > MAX_DTMF_BUFF )
		{
			dtmf_rx_buf_init();
		}
		if(( uc_dtmf > 0x00 ) && ( uc_dtmf < 0x0D ))
		{
			if (uc_dtmf <= 9)
			{
				ch = uc_dtmf + '0';
			}
			else if (uc_dtmf == 0x0a)
			{
				ch = '0';
			}
			else if (uc_dtmf == 0x0b)
			{
				ch = '*';
			}
			else if (uc_dtmf == 0x0c)
			{
				ch = '#';
			}
			dtmf_rx_buf.dtmf_buff[dtmf_rx_buf.rx_addr++] = ch;
			uart_send(&ch, 1);
		}
	}		
}

void dtmf_data_handler()
{
	uint8 uartsend_buf[100] = {0};
	recv_dtmf();  								//��ô���dtmf���Ե�������
}