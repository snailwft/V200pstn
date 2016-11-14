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

#define BUF_MAX_SIZE				100

uint8 uartrecv_buf[BUF_MAX_SIZE] = {0};  					//������Ϊģ�⴮�ڽ������ݵĻ���    
uint8 ring_times = 0, ring_num = 0, recv_num = 0;
uint8 uartsend_buf[BUF_MAX_SIZE] = {0};

void init(void)
{
	system_init();							// ϵͳ��ʼ��
	gpio_init();							// GPIO��ʼ��
	fsk_init();								// fsk��������ʼ��
	time16b1_int_init(1000);				// 16λ��ʱ��1 1�붨ʱ�������ж�
	uart_init(9600); 						// ���ڣ������ò�����	
}

int main(void)
{	
	init();
	while (1)
	{		
		tim16b0_delay_ms(1000);
		//uart_send("hello", 5);
		if (recv_num > 0)
		{		
			message_parese_process(uartrecv_buf);
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
			if (recv_num >= BUF_MAX_SIZE)
			{
				recv_num = 0;
			}
			uartrecv_buf[recv_num++] = LPC_UART->RBR;	  // ��RXFIFO�ж�ȡ���յ�������
		}
	}
}

void TIMER16_1_IRQHandler(void)
{
	if((LPC_TMR16B1->IR & 0x1)==1) 						// ����ǲ���MR0������ж�
	{	
		ring_times++;
		if (ring_num > 15)
		{
			SET_BIT(LPC_GPIO1,DATA,9);  	 				// ���� ht9032 PDWN���빤��ģʽ		
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 1, NULL, NULL, 0); 	//��������֪ͨ��������
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//���͸�����
		}
		if (ring_times > 6)
		{
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 0, NULL, NULL, 0);
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//���͸�����
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
