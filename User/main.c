#include "config.h"
#include "gpio.h"
#include "ct.h"
#include "uart.h"
#include "fsk.h"
#include "com.h"
#include <stdio.h>

/*
 *RING:1/0:FSK:0704121313016054035:HOOK:1/0*
 *RING:1/0:DTMF:0704121313016054035:HOOK:1/0*
 1:��ʾ�����ժ��
 0:��ʾ�������һ�
*/

uint8 tmpbuf[100] = {0};  				//������Ϊģ�⴮�ڽ������ݵĻ���   
uint8 buf[5] = {0};;
uint8 tmp = 0;
uint8 recv_num = 0;
uint8 ring_times = 0, ring_num = 0;
uint8 data_buf[100] = {0};

void init(void)
{
	system_init();							    	// ϵͳ��ʼ��
	gpio_init();								    	// GPIO��ʼ��
	fsk_init();										// fsk��������ʼ��
	time16b1_int_init(1000);				// 16λ��ʱ��1 1�붨ʱ�������ж�
	uart_init(1200); 								// ���ڣ������ò�����	
}

int main(void)
{
	
	int fsk_status = 0;
	uint8 hook_status = 0;
	init();
	while (1)
	{		
		tim16b0_delay_ms(100);
		if (recv_num > 0)
		{		
			message_parese_process(tmpbuf);
		}		
	}
}

void PIOINT0_IRQHandler(void)  	  
{
	if (GET_BIT(LPC_GPIO0, MIS, 8)!=0)	      // ���P0.8���Ų������ж� PSTN_RING_MCU �½��ش����ж�
	{
		ring_num++;
		time16b1_enable();
		ring_times = 0;
	}
	LPC_GPIO0->IC = 0xFFF;  						 	// ���GPIO0�ϵ��ж�
}

void UART_IRQHandler(void)
{
	uint32 IRQ_ID;		  				// �����ȡ�ж�ID�ű���
	uint8 redata;    						// ����������ݱ�������
	
	IRQ_ID = LPC_UART->IIR;   	// ���ж�ID��
	IRQ_ID =((IRQ_ID>>1)&0x7);	// ���bit4:bit1	
	if(IRQ_ID == 0x02 )		  			// ����ǲ��ǽ�������������ж�
	{
		while (LPC_UART->LSR & 0x1)
		{
			tmpbuf[recv_num++] = LPC_UART->RBR;	  // ��RXFIFO�ж�ȡ���յ�������
		}
	}
	return;
}

void TIMER16_1_IRQHandler(void)
{
	//uint8 data_buf[100] = {0};
	if((LPC_TMR16B1->IR & 0x1)==1) 						// ����ǲ���MR0������ж�
	{	
		ring_times++;
		if (ring_num > 15)
		{
			SET_BIT(LPC_GPIO1,DATA,9);  	 				// ���� ht9032 PDWN���빤��ģʽ		
			memset(data_buf, 0x0, sizeof(data_buf));
			sprintf(data_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 1, NULL, NULL, 0); 	//��������֪ͨ��������
			uart_send(data_buf, strlen(data_buf)); 	//���͸�����
		}
		if (ring_times > 6)
		{
			memset(data_buf, 0x0, sizeof(data_buf));
			sprintf(data_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 0, NULL, NULL, 0);
			uart_send(data_buf, strlen(data_buf)); 	//���͸�����
			fsk_ucgetflag = 0;										//�Է��һ�����
			CLR_BIT(LPC_GPIO1,DATA,9);  	 				//ht9032 ����PDWN��������ģʽ
			time16b1_disable();
		}
		CPL_BIT(LPC_GPIO0,DATA,7);  	 	
		CPL_BIT(LPC_GPIO2,DATA,0);  	 	
		ring_num = 0;
	}
	LPC_TMR16B1->IR = 0x1F; 									// �����ж�ʱ��/�������жϱ�־	
}
