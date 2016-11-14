#include "com.h"
#include "config.h"
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

int message_parese_process(uint8 *buf)
{
	int hook_status;
	int fsk_status;
	uint8 uartsend_buf[100] = {0};
	
	if (buf[0] == '*') //���ط��� 
	{					
		if (message_integrity(&buf[1])) //�����Ϣ��������
		{
			hook_status = deal_message(buf, strlen(buf));
			if (hook_status == 1)
			{
				CLR_BIT(LPC_GPIO0,DATA,9); //ժ��
				time16b1_disable();
				fsk_ucgetflag = 0; //�ҷ�ժ������
				CLR_BIT(LPC_GPIO1,DATA,9);  	 	//ht9032 ����PDWN��������ģʽ
			}
			else if (hook_status == 0)
			{
				SET_BIT(LPC_GPIO0,DATA,9); //�һ�
			}
			return 1;
		}
	}
	else  //  fsk����
	{				
		if (fsk_status = CheckFSKMessage(buf, strlen(buf)) > 0)
		{
			uart_send(buf, strlen(buf)); //���͸�����
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 1, stFskMeg.ucTime, stFskMeg.ucFskNum, 0);
			uart_send(uartsend_buf, strlen(uartsend_buf)); //���͸�����
			return 1;
		}
		else if (fsk_status == -1) // fsk ��Ϣ����
		{
			return 1;
		}
	}			
	{ // dtmf ����

	}

	return 0;
}