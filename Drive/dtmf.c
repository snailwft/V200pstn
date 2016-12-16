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

	if (GET_BIT(LPC_GPIO1, DATA, 11)) //判断有没有数据
	{
		dtmf_flag = 1;
		time16b1_enable();
		SET_BIT(LPC_GPIO1, DATA, 10); //使能			
		set_dtmf_qn_dir();  //设置成输入 ，接收dtmf码
		uc_dtmf = GET_BITS(LPC_GPIO1, DATA);
		if (GET_BIT(LPC_GPIO1, DATA, 8))
		{
			uc_dtmf |= (1<<3);
		}	
		clr_dtmf_qn_dir(); //设置成输出，不接收dtmf码
		while (GET_BIT(LPC_GPIO1, DATA, 11));
		CLR_BIT(LPC_GPIO1, DATA, 10); // 这个地方把开关关闭了导致没有数据输出
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
	recv_dtmf();  								//怎么检测dtmf来显的完整性
}