#include "dtmf.h"
#include "config.h"

ST_DTMF_RXBUFF dtmf_rx_buf;

void dtmf_rx_buf_init()
{
	memset(&dtmf_rx_buf, 0x0, sizeof(dtmf_rx_buf));
}

void recv_dtmf()
{
	uint8 uc_dtmf = 0xff, ch;
	//uart_send("hello", 5);
	if (!GET_BIT(LPC_GPIO1, DATA, 11)) //�ж���û������
	{
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
		if( (dtmf_rx_buf.rx_addr < MAX_DTMF_BUFF )
			&& ( uc_dtmf > 0x00 ) && ( uc_dtmf < 0x0D ) )
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
	recv_dtmf();  								//��ô���dtmf���Ե�������
	if (dtmf_rx_buf.rx_addr > 0)
	{
		// ��ȡ������֮��������ݣ��ж����ݵ������ԣ�������ʱ��ͺ���
		//��ʱ��ͺ���ͷ��͸�����
	}
}