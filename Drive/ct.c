#include "config.h"
#include "ct.h"
#include "main.h"
#include "fsk.h"
#include "pstn.h"
#include "uart.h"

extern uint8 uartrecv_buf[BUF_MAX_SIZE], uartsend_buf[BUF_MAX_SIZE];					//用来作为模拟串口接收数据的缓存  
extern uint8 recv_num, ring_times, ring_num, fsk_flag;

void wait()
{
	while((LPC_TMR16B0->IR & 0x1)!=1);
	LPC_TMR16B0->IR = 0x1F; // 清所有定时器/计数器中断标志	
}

/**************************************************************************************
* FunctionName   : time16b0_init()
* Description    : 使能TIM16B0时钟
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void time16b0_init(void) 
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	
}

/**************************************************************************************
* FunctionName   : time16b0_int_init()
* Description    : 初始化TIM16B0
* EntryParameter : ms-定时毫秒值    
* ReturnValue    : None
**************************************************************************************/
void time16b0_int_init(uint32 ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//使能TIM16B0时钟
	LPC_TMR16B0->TCR = 0x02;							//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = 1000;							//把预分频寄存器置0，使PC+1，TC+1
	//LPC_TMR16B0->PR  = 0x07ff;						//把预分频寄存器置0x7fff，即PC每计数2048,TC+1
	LPC_TMR16B0->MR0 = ms * 48;						//在48Mhz下工作的比较值，其它请修改
	LPC_TMR16B0->IR  = 0x01;								//MR0中断标志复位,即清中断（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x03;							//MR0与TC值匹配时产生中断，MR0与TC值匹配时使TC复位，TC重新计数
	LPC_TMR16B0->TCR = 0x00;							//启动定时器：TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // 使能TIM32B0中断	
}

void time16b0_int_1200()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//使能TIM16B0时钟
	LPC_TMR16B0->TCR = 0x02;			//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = 100;				//把预分频寄存器置0，使PC+1，TC+1
	LPC_TMR16B0->MR0 = 400;				//在48Mhz下工作的比较值，其它请修改
	//LPC_TMR16B0->MR0 = 433;	   //概率很低 ，有点快
	//LPC_TMR16B0->MR0 = 434;
	//LPC_TMR16B0->MR0 = 435;	   //读慢了
	LPC_TMR16B0->IR  = 0x01;				//MR0中断标志复位,即清中断（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x03;			//MR0与TC值匹配时产生中断，MR0与TC值匹配时使TC复位
	LPC_TMR16B0->TCR = 0x00;			//关闭定时器：TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // 使能TIM32B0中断	不注册中断函数，当中断来时防止进入中断函数
}

void time16b0_init_19200()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//使能TIM16B0时钟
	LPC_TMR16B0->TCR = 0x02;			//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = 100;				//把预分频寄存器置0，使PC+1，TC+1
	LPC_TMR16B0->MR0 = 25;				//在48Mhz下工作的比较值，其它请修改
	//LPC_TMR16B0->MR0 = 433;	   //概率很低 ，有点快
	//LPC_TMR16B0->MR0 = 434;
	//LPC_TMR16B0->MR0 = 435;	   //读慢了
	LPC_TMR16B0->IR  = 0x01;				//MR0中断标志复位,即清中断（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x03;			//MR0与TC值匹配时产生中断，MR0与TC值匹配时使TC复位
	LPC_TMR16B0->TCR = 0x00;			//关闭定时器：TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // 使能TIM32B0中断	不注册中断函数，当中断来时防止进入中断函数
}

void time16b0_init_2400()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//使能TIM16B0时钟
	LPC_TMR16B0->TCR = 0x02;			//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = 100;				//把预分频寄存器置0，使PC+1，TC+1
	LPC_TMR16B0->MR0 = 200;				//在48Mhz下工作的比较值，其它请修改
	//LPC_TMR16B0->MR0 = 433;	   //概率很低 ，有点快
	//LPC_TMR16B0->MR0 = 434;
	//LPC_TMR16B0->MR0 = 435;	   //读慢了
	LPC_TMR16B0->IR  = 0x01;				//MR0中断标志复位,即清中断（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x03;			//MR0与TC值匹配时产生中断，MR0与TC值匹配时使TC复位
	LPC_TMR16B0->TCR = 0x00;			//关闭定时器：TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // 使能TIM32B0中断	不注册中断函数，当中断来时防止进入中断函数
}

#if 0
void time16b0_int_1200()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//使能TIM16B0时钟
	LPC_TMR16B0->TCR = 0x02;			//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = 40;				//把预分频寄存器置0，使PC+1，TC+1
	//LPC_TMR16B0->MR0 = 400;				//在48Mhz下工作的比较值，其它请修改
	//LPC_TMR16B0->MR0 = 433;	   //概率很低 ，有点快
	LPC_TMR16B0->MR0 = 1072;
	//LPC_TMR16B0->MR0 = 435;	   //读慢了
	LPC_TMR16B0->IR  = 0x01;				//MR0中断标志复位,即清中断（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x03;			//MR0与TC值匹配时产生中断，MR0与TC值匹配时使TC复位
	LPC_TMR16B0->TCR = 0x00;			//关闭定时器：TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // 使能TIM32B0中断	不注册中断函数，当中断来时防止进入中断函数
}
#endif
/**************************************************************************************
* FunctionName   : tim16b0_delay_ms()
* Description    : TIM16B0毫秒延时
* EntryParameter : ms-延时毫秒值    
* ReturnValue    : None
**************************************************************************************/
void tim16b0_delay_ms(uint32 ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	
	LPC_TMR16B0->TCR = 0x02;			//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = ms;				//把预分频寄存器置1000，使PC+1，TC+1
	LPC_TMR16B0->MR0 = 1000 * 48;	//在48Mhz下工作的值，其它请修改
	LPC_TMR16B0->IR  = 0x01;		 		//MR0中断复位,即清中断（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x04;			//MR0邋禁止中断，MR0与TC值匹配时停止TC和PC，并使TCR[0]=0, 停止定时器工作
	LPC_TMR16B0->TCR = 0x01;			//启动定时器：TCR[0]=1;
	  
	while (LPC_TMR16B0->TCR & 0x01);//等待定时器计时时间到
}


/**************************************************************************************
* FunctionName   : tim16b0_delay_us()
* Description    : TIM16B0微秒延时
* EntryParameter : us-延时微秒值    
* ReturnValue    : None
**************************************************************************************/
void tim16b0_delay_us(uint32 us)
{
	LPC_TMR16B0->TCR = 0x02;		//复位定时器（bit1：写1复位）
	LPC_TMR16B0->PR  = 0x00;		//把预分频寄存器置0，使PC+1，TC+1
	LPC_TMR16B0->MR0 = us * 48;	    //在48Mhz下工作的值，其它请修改
	LPC_TMR16B0->IR  = 0x01;		//MR0中断复位（bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0）
	LPC_TMR16B0->MCR = 0x04;		//MR0中断产生时停止TC和PC，并使TCR[0]=0, 停止定时器工作
	LPC_TMR16B0->TCR = 0x01;		//启动定时器：TCR[0]=1;
	  
	while (LPC_TMR16B0->TCR & 0x01);//等待定时器计时时间到
}

void time16b1_int_init(uint32 ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);    //打开CT16B1定时器时钟
    LPC_TMR16B1->TCR = 0x02;        					//复位定时器
    LPC_TMR16B1->PR  = ms;        					//配置预分频器，使得1us TC+1
    LPC_TMR16B1->MR0 = 48 * 1000;						//在48Mhz下工作的比较值，其它请修改
    LPC_TMR16B1->IR  = 0x01;        						//中断复位
    LPC_TMR16B1->MCR = 0x03;							//MR0与TC值匹配时产生中断，MR0与TC值匹配时使TC复位
    LPC_TMR16B1->TCR = 0x00; 							// 关闭定时器
    NVIC_EnableIRQ(TIMER_16_1_IRQn);    			// 开启NVCI中断入口
}

void time16b1_enable()
{
	 LPC_TMR16B1->TCR = 0x01; 							// 打开16位定时器1
}

void time16b1_disable()
{
	LPC_TMR16B1->TCR = 0x00; 							// 关闭16位定时器1
}

void TIMER16_1_IRQHandler(void)
{
	//uint8 uartsend_buf[BUF_MAX_SIZE];
	if((LPC_TMR16B1->IR & 0x1) == 1) 						// 检测是不是MR0引起的中断
	{	
#if 1
		ring_times++;
#if 0
		if (ring_num > 15)
		{			
			if (get_pstn_cid_mode() != PSTN_DTMF)
			{
				SET_BIT(LPC_GPIO1, DATA,9);  	 				//拉低 ht9032 PDWN进入工作模式		因为这里接了反极开关 	
				SET_BIT(LPC_GPIO0, DATA, 11);					//拉高接通ht9032串口
				set_pstn_event(PSTN_EVENT_RING);
			}
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID:%s%s:HOOK:%d*", 1, NULL, NULL, 0); 	//来电振铃通知主控振铃
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//发送给主控
		}
#endif
		if (ring_num > 15)
		{			
			if (get_pstn_cid_mode() == PSTN_CID_IDL)
			{
				SET_BIT(LPC_GPIO1, DATA,9);  	 				//拉低 ht9032 PDWN进入工作模式		因为这里接了反极开关 	
				//SET_BIT(LPC_GPIO0, DATA, 11);					//拉高接通ht9032串口  ,如果是dtmf来显，不需要操作ht9032		
				//SET_BIT(LPC_GPIO2, DATA, 0);					//拉高接通ht9032串口  ,如果是dtmf来显，不需要操作ht9032	
				set_pstn_cid_mode(PSTN_FSK);
			}
			set_pstn_event(PSTN_EVENT_RING);
#if 1		//不要在这里通知振铃，直接收到来显之后在通知
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID:%s%s:HOOK:%d*", 1, NULL, NULL, 0); 	//来电振铃通知主控振铃
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//发送给主控
#endif
		}
		if (ring_times > 6)
		{
#if 0			//不要这里通知了
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID::HOOK:%d*", 0, 0);
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//发送给主控
#endif
			if (get_pstn_cid_mode() == PSTN_FSK)
			{
				CLR_BIT(LPC_GPIO1,DATA,9);  	 								//ht9032 拉低PDWN进入休眠模式
				CLR_BIT(LPC_GPIO2, DATA, 0);									//uart接向主控
				set_pstn_cid_mode(PSTN_CID_IDL);
				uart_recv_init();
				time16b1_disable();
			}
		}
		ring_num = 0;
#endif
	}
	LPC_TMR16B1->IR = 0x1F; 									// 清所有定时器/计数器中断标志	
}


