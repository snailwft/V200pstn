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
 1:表示振铃或摘机
 0:表示不振铃或挂机
*/

uint8 uartrecv_buf[BUF_MAX_SIZE] = {0}, uartsend_buf[BUF_MAX_SIZE] = {0};					//用来作为模拟串口接收数据的缓存    
uint8 ring_times = 0, ring_num = 0, recv_num = 0, fsk_flag = 0, fsk_clear = 0;

void init(void)
{
	system_init();							// 系统初始化
	gpio_init();								// GPIO初始化
	fsk_init();								// fsk缓冲区初始化
	dtmf_rx_buf_init();
	time16b1_int_init(1000);		// 16位定时器1 1秒定时并产生中断
	uart_init(1200); 						// 串口，并设置波特率	
	pstn_init();
	wdt_enable();
}

int main(void)
{	
	init();
	while (1)
	{		
		tim16b0_delay_ms(100);
		wdt_feed();     					// 喂狗
		check_pstn_hook();		
		message_handler();
		dtmf_data_handler(); 
	}
	return 0;
}

