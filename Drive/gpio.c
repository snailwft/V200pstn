#include "config.h"
#include "gpio.h"
#include "pstn.h"
#include "main.h"
#include "uart.h"

extern uint8 ring_times, ring_num, uartsend_buf[BUF_MAX_SIZE];
extern ST_UART_BUF uartrecv, uartsend;
ST_FSK_BUF fsk_buf;
/**************************************************************************************
* FunctionName   : gpio_init()
* Description    : GPIO初始化
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void gpio_init(void)
{
    //第一步，引脚功能设置（数字IO或其它）
	SET_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16); 	//使能IOCON时钟(bit16)
	LPC_IOCON->PIO1_9 = 0XD0;                        	//把P1.9设置为数字IO脚输入   			PSTN_PDWN_CPU   DTMF_PDWN共用	
	LPC_IOCON->JTAG_TDI_PIO0_11 = 0XD1;	 	//把p0.11设置为数字IO脚					UART_SEL  mcu串口接收端开关切换		

	LPC_IOCON->JTAG_TMS_PIO1_0 = 0XD1; 		//把P1.0设置为数字IO脚输出					DTMF_Q1_MCU   HT9032D_DOUT
	LPC_IOCON->JTAG_TDO_PIO1_1 = 0XD1; 		//把P1.1设置为数字IO脚（字节操作）		DTMF_Q2_MCU	
	LPC_IOCON->JTAG_nTRST_PIO1_2 = 0XD1; 	//把P1.2设置为gpio脚							DTMF_Q3_MCU
	LPC_IOCON->PIO1_8 = 0XD0;							//	把p1.8设置为数字IO脚						DTMF_Q4_MCU
	LPC_IOCON->PIO1_10 = 0XD0; 						//把P1.10设置为数字IO脚（字节操作）	DTMF_OE_MCU	
	LPC_IOCON->PIO1_11 = 0XD0; 						//把P1.11设置为数字IO脚（字节操作）	DTMF_StD_MCU		
	
	LPC_IOCON->PIO0_7 = 0xD0;							//把p0.7设置为数字IO脚		
	LPC_IOCON->PIO2_0 = 0xD0;							//把p2.0设置为数字IO脚
	
	LPC_IOCON->PIO0_8 = 0xD0;							//把p0.8设置为数字IO脚					PSTN_RING_MCU
	LPC_IOCON->PIO0_9 = 0xD0;							//把p0.9设置为数字IO脚					PSTN_OH_MCU
	
	LPC_IOCON->PIO3_2 = 0xD0;							//把p3.2设置为数字IO脚					POLARITY_CPC5712   	反极检测
	LPC_IOCON->PIO3_4 = 0xD0;							//把p3.4设置为数字IO脚					LOOP_CPC5712				环路电流检测
	LPC_IOCON->PIO3_5 = 0xD0;							//把p3.5设置为数字IO脚					Line_In_Use_CPC5712	忙线检测   低电平表示设备正在摘机状态
		
	CLR_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16);//禁能IOCON时钟(bit16)（引脚配置完成后关闭该时钟）

	//第二步，引脚方向设置（输入或输出）
	CLR_BIT(LPC_GPIO1,DIR,0); 						//把P1.0设置为输入
	CLR_BIT(LPC_GPIO1,DIR,1); 						//设置p1.1为输入
	CLR_BIT(LPC_GPIO1,DIR,2); 						//把p1.2设置为输入
	CLR_BIT(LPC_GPIO1,DIR,8); 						//把p1.8设置为输入
	
	SET_BIT(LPC_GPIO1,DIR,9); 						//把P1.9设置为输出
	SET_BIT(LPC_GPIO1, DATA,9); 
	SET_BIT(LPC_GPIO1,DIR,10);						//把p1.10设置为输出
	
	CLR_BIT(LPC_GPIO1,DIR,11);						//把P1.11设置为输入
	SET_BIT(LPC_GPIO0,DIR,11);						//把P0.11设置为输出
	CLR_BIT(LPC_GPIO0, DIR, 8);						//把p0.8设置为输入
	SET_BIT(LPC_GPIO0, DIR, 9);						//把p0.9设置为输出
	SET_BIT(LPC_GPIO0, DIR, 7);						//把p0.7设置为输出
	SET_BIT(LPC_GPIO2, DIR, 0);						//把p2.0设置为输出
	CLR_BIT(LPC_GPIO2, DATA, 0);					//默认配置成连接主控uart

	CLR_BIT(LPC_GPIO3, DIR, 2);						//把p3.2设置为输入
	CLR_BIT(LPC_GPIO3, DIR, 4);						//把p3.4设置为输入
	CLR_BIT(LPC_GPIO3, DIR, 5);						//把p3.5设置为输入


	//第三步，设置中断
	//CLR_BIT(LPC_GPIO0,IS,11); //选择P0.11为边沿触发   PSTN_DOUTC_MCU
	//CLR_BIT(LPC_GPIO0,IEV,11);//选择P0.11为下降沿触发 
	//SET_BIT(LPC_GPIO0,IE,11); //设置P0.11中断不被屏蔽
	CLR_BIT(LPC_GPIO0,IS,8); //选择P0.8为边沿触发   PSTN_RING_MCU
	CLR_BIT(LPC_GPIO0,IEV,8);//选择P0.8为下降沿触发 
	SET_BIT(LPC_GPIO0,IE,8); //设置P0.8中断不被屏蔽

	CLR_BIT(LPC_GPIO1,IS,0); //选择P1.0为边沿触发   PSTN_DOUT_MCU
	CLR_BIT(LPC_GPIO1,IEV,0);	//选择P1.0为下降沿触发		PSTN_DOUT_MCU
	SET_BIT(LPC_GPIO1,IE,0); //设置P1.0中断不被屏蔽
	//CLR_BIT(LPC_GPIO1,IS,2); //选择P1.2为边沿触发   PSTN_RDET_MCU
	//CLR_BIT(LPC_GPIO1,IEV,2);//选择P1.2为下降沿触发 
	//SET_BIT(LPC_GPIO1,IE,2); //设置P1.2中断不被屏蔽
	//CLR_BIT(LPC_GPIO1,IS,1); //选择P1.1为边沿触发   PSTN_CDET_MCU
	//SET_BIT(LPC_GPIO1,IEV,1);//选择P1.1为上升沿触发 
	//SET_BIT(LPC_GPIO1,IE,1); //设置P1.1中断不被屏蔽
	
	//第四步，开GPIO1中断
	NVIC_EnableIRQ(EINT1_IRQn);	// 使能GPIO1中断
	NVIC_EnableIRQ(EINT0_IRQn);	// 使能GPIO0中断
}

void set_dtmf_qn_dir()
{
	CLR_BIT(LPC_GPIO1,DIR,0); 						//把P1.0设置为输入
	CLR_BIT(LPC_GPIO1,DIR,1); 						//设置p1.1为输入
	CLR_BIT(LPC_GPIO1,DIR,2); 						//把p1.2设置为输入
	CLR_BIT(LPC_GPIO1,DIR,8); 						//把p1.8设置为输入
}

void clr_dtmf_qn_dir()
{
	SET_BIT(LPC_GPIO1,DIR,0); 						//把P1.0设置为输出
	SET_BIT(LPC_GPIO1,DIR,1); 						//设置p1.1为输出
	SET_BIT(LPC_GPIO1,DIR,2); 						//把p1.2设置为输出
	SET_BIT(LPC_GPIO1,DIR,8); 						//把p1.8设置为输出
}

void PIOINT0_IRQHandler(void)  	  
{
	if (GET_BIT(LPC_GPIO0, MIS, 8)!=0)	      // 检测P0.8引脚产生的中断 PSTN_RING_MCU 下降沿触发中断
	{
		ring_num++;		
		SET_BIT(LPC_GPIO1, DATA,9);  	 				//拉低 ht9032 PDWN进入工作模式		因为这里接了反极开关 	
		time16b1_enable();
		ring_times = 0;
	}
	LPC_GPIO0->IC = 0xFFF;  						 	// 清除GPIO0上的中断
}

#if 1
void fsk_buf_int()
{
	memset(&fsk_buf, 0, sizeof(fsk_buf));
}

int get_fsk_buf_num()
{
	return fsk_buf.num;
}

//接收一个字符 
uint8 recv_byte() 
{      
	uint8 Output = 0;
	uint8 i = 8;
	LPC_TMR16B0->TCR = 0x01;  //启动Timer16 0
	wait(); //等过起始位	
	while(i--) //接收8位数据
	{
		Output >>= 1; //先收低位
		if(GET_BIT(LPC_GPIO1,DATA,0) > 0) 		//高电平
			Output |= 0x80;			
		wait(); 			//位间延时		
	}
	LPC_TMR16B0->TCR = 0x00;  	//停止Timer0
	return Output;
}

//接收一个字符 
uint8 recv_byte2() 
{      
	uint8 Output = 0;
	uint8 i = 8, j =0;
	wait(); // 定时器要是16倍发送波特率
	wait();	
	while(i--) //接收8位数据
	{
		Output >>= 1; //先收低位
		if(GET_BIT(LPC_GPIO1, DATA, 0) > 0) 		//高电平
			Output |= 0x80;				
		wait(); 			//位间延时	
		wait();		
	}
	LPC_TMR16B0->TCR = 0x00;  	//停止Timer0
	return Output;
}

void gpio_irq_disable()
{
	CLR_BIT(LPC_GPIO1,IE,0); //设置P1.0中断屏蔽
}

void gpio_irq_enable()
{
	SET_BIT(LPC_GPIO1,IE,0); 						//设置P1.0中断不屏蔽
}
#if 1
void PIOINT1_IRQHandler(void)
{
	int i = 0; 
	uint8 redata;
	//检测是否是该中断产生
	if (GET_BIT(LPC_GPIO1, MIS, 0)!=0)	      // 检测P1.0引脚产生的中断 PSTN_DOUT_MCU 下降沿触发中断
	{		
		//屏蔽中断
		CLR_BIT(LPC_GPIO1,IE,0); //设置P1.0中断屏蔽
		LPC_TMR16B0->TCR = 0x01;  //启动Timer16 0
		wait();// 定时器要是16倍发送波特率
		// 读取该gpio脚电平是否还是低(低电平表示start位)
		if (GET_BIT(LPC_GPIO1, DATA, 0) == 0)		
		{		
			redata = recv_byte2();
			//uart_send(&redata, 1);
#if 1
			if (redata == 0x55) 			//来显数据头
			{
				fsk_buf.fsk_flag = 1;
				if (fsk_buf.num > 40) // 0x55数量最多不会超过20，如果大于30表示uartrecv_buf填充了很多垃圾数据
				{
					fsk_buf.num = 0;
					memset(fsk_buf.fsk_buf, 0, sizeof(fsk_buf.fsk_buf));
				}
			}
			if (fsk_buf.num < BUF_MAX_SIZE && fsk_buf.fsk_flag == 1) 	//存在风险，万一recv_num没有清0
			{
				fsk_buf.fsk_buf[fsk_buf.num++] = redata;	  						//从RXFIFO中读取接收到的数据 ，控制数据量
				uart_send(&redata, 1);
			}
#endif
		}
	}
	SET_BIT(LPC_GPIO1,IE,0); 						//设置P1.0中断不屏蔽
	LPC_GPIO1->IC = 0xFFF;  						 	// 清除GPIO1上的中断
}

#endif

#endif

