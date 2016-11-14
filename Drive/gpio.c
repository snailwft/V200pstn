#include "config.h"
#include "gpio.h"
/**************************************************************************************
* FunctionName   : gpio_init()
* Description    : GPIO初始化
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void gpio_init(void)
{
    //第一步，引脚功能设置（数字IO或其它）
	SET_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16); //使能IOCON时钟(bit16)
	LPC_IOCON->PIO1_9 = 0XD0; //把P1.9设置为数字IO脚输入   								PSTN_PDWN_CPU
	LPC_IOCON->JTAG_TMS_PIO1_0 = 0XD1; //把P1.0设置为数字IO脚输出				PSTN_DOUT_MCU
	LPC_IOCON->JTAG_TDO_PIO1_1 = 0XD1; //把P1.1设置为数字IO脚（字节操作）	PSTN_CDET_MCU								
	LPC_IOCON->JTAG_nTRST_PIO1_2 = 0XD1; // 设置为gpio脚								PSTN_RDET_MCU
	LPC_IOCON->JTAG_TDI_PIO0_11 = 0XD1;	 // 												PSTN_DOUTC_MCU
	LPC_IOCON->PIO1_8 = 0XD0;						//													DTMF_Q1_MCU
	//LPC_IOCON->PIO1_9 = 0XD0;						//													DTMF_Q2_MCU   与ht9032 down脚  重合了
	LPC_IOCON->PIO1_10 = 0XD0; 					//把P1.10设置为数字IO脚（字节操作）DTMF_Q3_MCU			PSTN_9032_OE
	LPC_IOCON->PIO1_11 = 0XD0; 					//把P1.11设置为数字IO脚（字节操作）DTMF_Q4_MCU			ACOK_MCU
	LPC_IOCON->PIO0_8 = 0xD0;						//把p0.8设置为数字IO脚					PSTN_RING_MCU
	LPC_IOCON->PIO0_9 = 0xD0;						//把p0.9设置为数字IO脚					PSTN_OH_MCU
	LPC_IOCON->PIO0_7 = 0xD0;						//把p0.7设置为数字IO脚		
	LPC_IOCON->PIO0_4 = 0x00;						//p0.4 开漏引脚	设置为数字IO脚		DTMF_StD_MCU
	LPC_IOCON->PIO2_0 = 0xD0;						//把p2.0设置为数字IO脚
	
	CLR_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16);//禁能IOCON时钟(bit16)（引脚配置完成后关闭该时钟）

	//第二步，引脚方向设置（输入或输出）
	SET_BIT(LPC_GPIO1,DIR,0); //把P1.0和P1.1引脚设置为输出
	CLR_BIT(LPC_GPIO1,DIR,1); 	// 设置p1.1为输入
	CLR_BIT(LPC_GPIO1,DIR,2);  //把p1.2设置为输入
	
	SET_BIT(LPC_GPIO1,DIR,9); //把P1.9设置为输出
	CLR_BIT(LPC_GPIO1,DIR,10);  //把p1.10设置为输入
	
	CLR_BIT(LPC_GPIO1,DIR,11);//把P1.11设置为输入
	CLR_BIT(LPC_GPIO0,DIR,11);//把P0.11设置为输入
	CLR_BIT(LPC_GPIO0, DIR, 8);//把p0.8设置为输入
	SET_BIT(LPC_GPIO0, DIR, 9);//把p0.9设置为输出
	SET_BIT(LPC_GPIO0, DIR, 7);//把p0.7设置为输出
	SET_BIT(LPC_GPIO2, DIR, 0);//把p2.0设置为输出

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

