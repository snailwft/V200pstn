#include "config.h"
#include "gpio.h"
#include "ct.h"
#include "uart.h"
#include "fsk.h"
#include "com.h"
#include "wdt.h"
#include "dtmf.h"
#include "main.h"
#include <stdio.h>

/*
 &RING:1/0:FSK:0704121313016054035:HOOK:1/0*
 &RING:1/0:DTMF:0704121313016054035:HOOK:1/0*
 1:��ʾ�����ժ��
 0:��ʾ�������һ�
*/

uint8 uartrecv_buf[BUF_MAX_SIZE] = {0}, uartsend_buf[BUF_MAX_SIZE] = {0};	//������Ϊģ�⴮�ڽ������ݵĻ���    
uint8 ring_times = 0, ring_num = 0, recv_num = 0, fsk_flag = 0, fsk_clear = 0, dtmf_flag = 0, dtmf_time = 0;

void init(void)
{
	system_init();							// ϵͳ��ʼ��
	gpio_init();								// GPIO��ʼ��
	//gpio_init2();
	//uart_init(1200); 	
#if 1
	fsk_init();								// fsk��������ʼ��
	dtmf_rx_buf_init();
	time16b1_int_init(1000);		// 16λ��ʱ��1 1�붨ʱ�������ж�
	time16b0_init_2400();
	uart_init(1200); 						// ���ڣ������ò�����	
	pstn_init();
	wdt_enable();
	uart_recv_init();
	fsk_buf_int();
#endif
}

void delay(int ms)
{
	int i = 0, j = 0;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 4000; j++)
			__NOP();			  // 0.25us * 4000 = 1ms
	}
}

int main(void)
{		
	init();
	uart_send("system init\n", strlen("system init\n"));
	while (1)
	{		
		//delay(500);
		wdt_feed();     						// ι��
		//check_pstn_hook();		
		message_handler();
		dtmf_data_handler(); 
	}
	return 0;
}
