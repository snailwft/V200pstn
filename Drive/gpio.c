#include "config.h"
#include "gpio.h"

extern uint8 ring_times, ring_num;

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

	LPC_IOCON->JTAG_TMS_PIO1_0 = 0XD1; 		//把P1.0设置为数字IO脚输出					DTMF_Q1_MCU
	LPC_IOCON->JTAG_TDO_PIO1_1 = 0XD1; 		//把P1.1设置为数字IO脚（字节操作）		DTMF_Q2_MCU	
	LPC_IOCON->JTAG_nTRST_PIO1_2 = 0XD1; 	//把P1.2设置为gpio脚							DTMF_Q3_MCU
	LPC_IOCON->PIO1_8 = 0XD0;							//	把p1.8设置为数字IO脚						DTMF_Q4_MCU
	LPC_IOCON->PIO1_10 = 0XD0; 						//把P1.10设置为数字IO脚（字节操作）	DTMF_OE_MCU	
	LPC_IOCON->PIO1_11 = 0XD0; 						//把P1.11设置为数字IO脚（字节操作）	DTMF_StD_MCU		
	
	LPC_IOCON->PIO0_7 = 0xD0;						//把p0.7设置为数字IO脚		
	LPC_IOCON->PIO2_0 = 0xD0;						//把p2.0设置为数字IO脚
	
	LPC_IOCON->PIO0_8 = 0xD0;						//把p0.8设置为数字IO脚					PSTN_RING_MCU
	LPC_IOCON->PIO0_9 = 0xD0;						//把p0.9设置为数字IO脚					PSTN_OH_MCU
	
	LPC_IOCON->PIO3_2 = 0xD0;						//把p3.2设置为数字IO脚					POLARITY_CPC5712   	反极检测
	LPC_IOCON->PIO3_4 = 0xD0;						//把p3.4设置为数字IO脚					LOOP_CPC5712				环路电流检测
	LPC_IOCON->PIO3_5 = 0xD0;						//把p3.5设置为数字IO脚					Line_In_Use_CPC5712	忙线检测   低电平表示设备正在摘机状态
		
	CLR_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16);//禁能IOCON时钟(bit16)（引脚配置完成后关闭该时钟）

	//第二步，引脚方向设置（输入或输出）
	CLR_BIT(LPC_GPIO1,DIR,0); 						//把P1.0设置为输入
	CLR_BIT(LPC_GPIO1,DIR,1); 						//设置p1.1为输入
	CLR_BIT(LPC_GPIO1,DIR,2); 						//把p1.2设置为输入
	CLR_BIT(LPC_GPIO1,DIR,8); 						//把p1.8设置为输入
	
	SET_BIT(LPC_GPIO1,DIR,9); 						//把P1.9设置为输出
	SET_BIT(LPC_GPIO1,DIR,10);						//把p1.10设置为输出
	
	CLR_BIT(LPC_GPIO1,DIR,11);						//把P1.11设置为输入
	SET_BIT(LPC_GPIO0,DIR,11);						//把P0.11设置为输出
	CLR_BIT(LPC_GPIO0, DIR, 8);						//把p0.8设置为输入
	SET_BIT(LPC_GPIO0, DIR, 9);						//把p0.9设置为输出
	SET_BIT(LPC_GPIO0, DIR, 7);						//把p0.7设置为输出
	SET_BIT(LPC_GPIO2, DIR, 0);						//把p2.0设置为输出

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

	//CLR_BIT(LPC_GPIO1,IS,2); //选择P1.2为边沿触发   PSTN_RDET_MCU
	//CLR_BIT(LPC_GPIO1,IEV,2);//选择P1.2为下降沿触发 
	//SET_BIT(LPC_GPIO1,IE,2); //设置P1.2中断不被屏蔽
	//CLR_BIT(LPC_GPIO1,IS,1); //选择P1.1为边沿触发   PSTN_CDET_MCU
	//SET_BIT(LPC_GPIO1,IEV,1);//选择P1.1为上升沿触发 
	//SET_BIT(LPC_GPIO1,IE,1); //设置P1.1中断不被屏蔽
	
	//第四步，开GPIO1中断
	//NVIC_EnableIRQ(EINT1_IRQn);	// 使能GPIO1中断
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
		time16b1_enable();
		ring_times = 0;
	}
	LPC_GPIO0->IC = 0xFFF;  						 	// 清除GPIO0上的中断
}

