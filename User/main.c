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
 1:表示振铃或摘机
 0:表示不振铃或挂机
*/

uint8 tmpbuf[100] = {0};  				//用来作为模拟串口接收数据的缓存   
uint8 buf[5] = {0};;
uint8 tmp = 0;
uint8 recv_num = 0;
uint8 ring_times = 0, ring_num = 0;
uint8 data_buf[100] = {0};

void init(void)
{
	system_init();							    	// 系统初始化
	gpio_init();								    	// GPIO初始化
	fsk_init();										// fsk缓冲区初始化
	time16b1_int_init(1000);				// 16位定时器1 1秒定时并产生中断
	uart_init(1200); 								// 串口，并设置波特率	
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
			//message_parese_process(tmpbuf);
#if 1
			//判断是来显还是主控发送
			if (tmpbuf[0] == '*') //主控发送 
			{					
				if (message_integrity(&tmpbuf[1])) //检测消息的完整性
				{
					hook_status = deal_message(tmpbuf, strlen(tmpbuf));
					if (hook_status == 1)
					{
						CLR_BIT(LPC_GPIO0,DATA,9); //摘机
						time16b1_disable();
						fsk_ucgetflag = 0; //我方摘机清零
						CLR_BIT(LPC_GPIO1,DATA,9);  	 	//ht9032 拉低PDWN进入休眠模式
					}
					else if (hook_status == 0)
					{
						SET_BIT(LPC_GPIO0,DATA,9); //挂机
					}
					uart_irq_disable();
					memset(tmpbuf, 0, sizeof(tmpbuf));
					recv_num = 0;	
					uart_irq_enable();
				}
				else  //不完整
				{
					
				}
			}
			else  //  fsk来显
			{				
				if (fsk_status = CheckFSKMessage(tmpbuf, strlen(tmpbuf)) > 0)
				{
					uart_send(tmpbuf, strlen(tmpbuf)); //发送给主控
					memset(data_buf, 0x0, sizeof(data_buf));
					sprintf(data_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 1, stFskMeg.ucTime, stFskMeg.ucFskNum, 0);
					uart_send(data_buf, strlen(data_buf)); //发送给主控
					uart_irq_disable();
					memset(tmpbuf, 0, sizeof(tmpbuf));
					recv_num = 0;	
					uart_irq_enable();
				}
				else if (fsk_status == -1) // fsk 消息错误
				{
					uart_irq_disable();
					memset(tmpbuf, 0, sizeof(tmpbuf));
					recv_num = 0;
					uart_irq_enable();
				}
				else 									// 消息不完整
				{
					
				}
				
			}			
			{ // dtmf 来显
			
			}
#endif
		}		
	}
}

void PIOINT0_IRQHandler(void)  	  
{
	if (GET_BIT(LPC_GPIO0, MIS, 8)!=0)	      // 检测P0.8引脚产生的中断 PSTN_RING_MCU 下降沿触发中断
	{
		ring_num++;
		time16b1_enable();
		uart_send("hello",5); 	//发送给主控
		ring_times = 0;
	}
	LPC_GPIO0->IC = 0xFFF;  						 	// 清除GPIO0上的中断
}

void UART_IRQHandler(void)
{
	uint32 IRQ_ID;		  				// 定义读取中断ID号变量
	uint8 redata;    						// 定义接收数据变量数组
	
	IRQ_ID = LPC_UART->IIR;   	// 读中断ID号
	IRQ_ID =((IRQ_ID>>1)&0x7);	// 检测bit4:bit1	
	if(IRQ_ID == 0x02 )		  			// 检测是不是接收数据引起的中断
	{
		while (LPC_UART->LSR & 0x1)
		{
			//tmpbuf[recv_num++] = LPC_UART->RBR;	  // 从RXFIFO中读取接收到的数据
		}
	}
	return;
}

void TIMER16_1_IRQHandler(void)
{
	//uint8 data_buf[100] = {0};
	if((LPC_TMR16B1->IR & 0x1)==1) 						// 检测是不是MR0引起的中断
	{	
		ring_times++;
		if (ring_num > 15)
		{
			SET_BIT(LPC_GPIO1,DATA,9);  	 				// 拉低 ht9032 PDWN进入工作模式		
			memset(data_buf, 0x0, sizeof(data_buf));
			sprintf(data_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 1, NULL, NULL, 0); 	//来电振铃通知主控振铃
			uart_send(data_buf, strlen(data_buf)); 	//发送给主控
		}
		if (ring_times > 5)
		{
			memset(data_buf, 0x0, sizeof(data_buf));
			sprintf(data_buf, "*RING:%d:CID:%s%s:HOOK:%d*", 0, NULL, NULL, 0);
			uart_send(data_buf, strlen(data_buf)); 	//发送给主控
			fsk_ucgetflag = 0;										//对方挂机清零
			CLR_BIT(LPC_GPIO1,DATA,9);  	 				//ht9032 拉低PDWN进入休眠模式
			time16b1_disable();
		}
		//CPL_BIT(LPC_GPIO0,DATA,7);  	 	
		//CPL_BIT(LPC_GPIO2,DATA,0);  	 	
		ring_num = 0;
	}
	LPC_TMR16B1->IR = 0x1F; 									// 清所有定时器/计数器中断标志	
}
