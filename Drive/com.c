#include "com.h"
#include "config.h"
#include "main.h"
#include "pstn.h"

extern uint8 uartrecv_buf[BUF_MAX_SIZE], uartsend_buf[BUF_MAX_SIZE], fsk_flag;					//������Ϊģ�⴮�ڽ������ݵĻ���  

/*
 *RING:1/0:CID:0704121313016054035:HOOK:1/0*
 1:��ʾ�����ժ��
 0:��ʾ�������һ�
*/

int deal_message(uint8 *databuf, uint8 datalength)
{
	uint8 *ptr = NULL;
	uint8 *ch = NULL;
	//��ȡHOOK,����ֵ
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
	
	if (buf[0] == '&') //���ط��� 
	{					
		if (message_integrity(&buf[1])) //�����Ϣ��������
		{
			hook_status = deal_message(buf, strlen(buf));
			if (hook_status == 1)
			{
				CLR_BIT(LPC_GPIO0,DATA,9); //ժ��			
				clear_pstn_event();	
				set_pstn_state(PSTN_OFFHOOK);
				time16b1_disable();
				CLR_BIT(LPC_GPIO1,DATA,9);  	 	//ht9032 ����PDWN��������ģʽ
			}
			else if (hook_status == 0)
			{
				SET_BIT(LPC_GPIO0,DATA,9); //�һ�
				clear_pstn_event();
				set_pstn_state(PSTN_INIT);
			}
			return 1;
		}
	}
	else  //  fsk����
	{				
		if (fsk_status = CheckFSKMessage(buf, strlen(buf)) > 0)
		{
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID:%s%s:HOOK:%d*", 1, stFskMeg.ucTime, stFskMeg.ucFskNum, 0);
			uart_send(uartsend_buf, strlen(uartsend_buf)); //���͸�����
			return 1;
		}
	}			
	return 0;
}

void message_handler()
{
	int stat = 0;
	if (recv_num > 0)
	{		
		stat = message_parese(uartrecv_buf);
		if (stat == 1)
		{
			uart_irq_disable();
			memset(uartrecv_buf, 0, sizeof(uartrecv_buf));
			recv_num = 0;	
			fsk_flag = 0;
			uart_irq_enable();
		}
	}		
}