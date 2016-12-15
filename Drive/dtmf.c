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
	
	if (!GET_BIT(LPC_GPIO1, DATA, 11)) //判断有没有数据
	{
		SET_BIT(LPC_GPIO1, DATA, 10); //使能			
		set_dtmf_qn_dir();  //设置成输入 ，接收dtmf码
		uc_dtmf = GET_BITS(LPC_GPIO1, DATA);
		if (GET_BIT(LPC_GPIO1, DATA, 8))
		{
			uc_dtmf |= (1<<3);
		}
		
		clr_dtmf_qn_dir(); //设置成输出，不接收dtmf码
		while (!GET_BIT(LPC_GPIO1, DATA, 11));
		CLR_BIT(LPC_GPIO1, DATA, 10); // 这个地方把开关关闭了导致没有数据输出
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
}

void dtmf_data_handler()
{
	recv_dtmf();  								//怎么检测dtmf来显的完整性
	if (dtmf_rx_buf.rx_addr > 0)
	{
		// 获取到数据之后解析数据，判断数据的完整性，解析出时间和号码
		//有时间和号码就发送给主控
	}
}