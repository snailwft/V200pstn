#include "wdt.h"

/******************************************
* FunctionName   : WDT_CLK_Setup()
* Description    : 启动看门狗时钟：
                   clksrc=0 选择IRC振荡器；clksrc=1 选择主时钟lksrc=2 选择看门狗时钟
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDT_CLK_Setup(void)
{
  	CLR_BIT(LPC_SYSCON,PDRUNCFG,6);    		// 看门狗振荡器时钟上电（bit6）
	LPC_SYSCON->WDTOSCCTRL = (0x1<<5); // DIVSEL=0 FREQSEL=1  WDT_OSC_CLK=250KHz
  	LPC_SYSCON->WDTCLKSEL = 0x2;        		// 选择看门狗时钟源 选择看门狗振荡器
  	LPC_SYSCON->WDTCLKUEN = 0x01;          // 更新时钟源
  	LPC_SYSCON->WDTCLKUEN = 0x00;          // 先写0，再写1达到更新目的
  	LPC_SYSCON->WDTCLKUEN = 0x01;
  	while ( !GET_BIT(LPC_SYSCON,WDTCLKUEN,0) );  // 等待更新成功
  	LPC_SYSCON->WDTCLKDIV = 1;            // 设置看门狗分频值为1  
  	return;
}

/******************************************
* FunctionName   : WDT_Enable()
* Description    : 使能看门狗  
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDT_Enable(void)
{
  	
	WDT_CLK_Setup();
	SET_BIT(LPC_SYSCON,SYSAHBCLKCTRL,15);// 允许WDT时钟，这个时钟是配置寄存器用的
  	LPC_WDT->TC = 80000;		// 给看门狗定时器赋值，定时时间大约1秒(这是在wdt_clk=250KHz时)8000*4*1/f
  	LPC_WDT->MOD |= 0x03;		// 写值0x03：不喂狗产生复位     写值0x01：不喂狗发生中断
 	LPC_WDT->FEED = 0xAA;		// 喂看门狗，开启
  	LPC_WDT->FEED = 0x55;    

	//NVIC_EnableIRQ(WDT_IRQn);	// 使能看门狗中断
  	return;
}

/******************************************
* FunctionName   : WDT_IRQHandler()
* Description    : 看门狗中断函数 
                   当MOD值设置为0x01时，如果没有及时喂狗，将会进入这个中断函数。   
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDT_IRQHandler(void)
{
	if (GET_BIT(LPC_WDT, MOD, 3))
	{
		uart_send("wdt irq", 7);
	}
 	CLR_BIT(LPC_WDT,MOD,2);		// 清看门狗超时标志位WDTOF
	// 在下面可以写入当看门狗中断发生时你想要做的事情
	
}

/******************************************
* FunctionName   : WDTFeed()
* Description    : 看门狗喂狗   
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDTFeed(void)
{
  	LPC_WDT->FEED = 0xAA;		// 喂狗
  	LPC_WDT->FEED = 0x55;
  	return;
}