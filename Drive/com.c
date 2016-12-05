#include "com.h"
#include "config.h"
#include "main.h"
#include "pstn.h"
#include "uart.h"

extern ST_UART_BUF uartrecv;
extern uint8 uartrecv_buf[BUF_MAX_SIZE], uartsend_buf[BUF_MAX_SIZE], fsk_flag;					//用来作为模拟串口接收数据的缓存  

/*
 *RING:1/0:CID:0704121313016054035:HOOK:1/0*
 1:表示振铃或摘机
 0:表示不振铃或挂机
*/

int deal_message(uint8 *databuf, uint8 datalength)
{
	uint8 *ptr = NULL;
	uint8 *ch = NULL;
	//获取HOOK,返回值
	if (datalength == 0)
		return -1;
	ptr = strstr(databuf, "HOOK:");
	if (ptr != NULL)
	{
		ch = (ptr + 5);
		if (atoi(ch) == 0)
			return 0;
		else if (atoi(ch) == 1)
			return 1;
	}
	return -1;
}

int message_integrity(uint8 *databuf)
{
	uint8 *ptr = NULL;
	ptr = strstr(databuf, "*");
	if (ptr == NULL)
	{
		return 0;
	}
	return 1;
}

int message_parese(uint8 *buf)
{
	int hook_status;
	int fsk_status;
	uint8 uartsend_buf[100] = {0};
	
	if (buf[0] == '&') //主控发送 
	{					
		if (message_integrity(&buf[1])) //检测消息的完整性
		{
			hook_status = deal_message(buf, strlen(buf));
			if (hook_status == 1)
			{
				CLR_BIT(LPC_GPIO0,DATA,9); //摘机			
				clear_pstn_event();	
				set_pstn_state(PSTN_OFFHOOK);
				time16b1_disable();
				CLR_BIT(LPC_GPIO1,DATA,9);  	 	//ht9032 拉低PDWN进入休眠模式
			}
			else if (hook_status == 0)
			{
				SET_BIT(LPC_GPIO0,DATA,9); //挂机
				clear_pstn_event();
				set_pstn_state(PSTN_INIT);
			}
			return 1;
		}
	}
	else //  fsk来显
	{				
		if (fsk_status = CheckFSKMessage(buf, strlen(buf)) > 0)
		{
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID:%s%s:HOOK:%d*", 1, stFskMeg.ucTime, stFskMeg.ucFskNum, 0);
			uart_send(uartsend_buf, strlen(uartsend_buf)); //发送给主控
			set_pstn_cid_mode(PSTN_CID_IDL);
			return 1;
		}
	}			
	return 0;
}

void message_handler()
{
	int stat = 0;
	if (get_uart_recv_num() > 0)
	{		
		stat = message_parese(uartrecv.uart_buf);
		if (stat == 1)
		{
			uart_irq_disable();
			uart_recv_init();
			uart_irq_enable();
		}
	}		
}