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
 *RING:1/0:FSK:0704121313016054035:HOOK:1/0*
 *RING:1/0:DTMF:0704121313016054035:HOOK:1/0*
 1:��ʾ�����ժ��
 0:��ʾ�������һ�
*/


uint8 uartrecv_buf[BUF_MAX_SIZE] = {0}, uartsend_buf[BUF_MAX_SIZE] = {0};					//������Ϊģ�⴮�ڽ������ݵĻ���    
uint8 ring_times = 0, ring_num = 0, recv_num = 0;

void init(void)
{
	system_init();							// ϵͳ��ʼ��
	gpio_init();								// GPIO��ʼ��
	fsk_init();								// fsk��������ʼ��
	dtmf_rx_buf_init();
	time16b1_int_init(1000);		// 16λ��ʱ��1 1�붨ʱ�������ж�
	uart_init(9600); 						// ���ڣ������ò�����	
	WDT_Enable();
}

int main(void)
{	
	init();
	while (1)
	{		
		WDTFeed();     // ι��
		check_pstn_hook();
		tim16b0_delay_ms(1000);
		message_handler();
		dtmf_data_handler();
	}
	return 0;
}

