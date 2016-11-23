#include "wdt.h"

/******************************************
* FunctionName   : WDT_CLK_Setup()
* Description    : �������Ź�ʱ�ӣ�
                   clksrc=0 ѡ��IRC������clksrc=1 ѡ����ʱ�ӣclksrc=2 ѡ���Ź�ʱ��
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDT_CLK_Setup(void)
{
  	CLR_BIT(LPC_SYSCON,PDRUNCFG,6);    		// ���Ź�����ʱ���ϵ磨bit6��
	LPC_SYSCON->WDTOSCCTRL = (0x1<<5); // DIVSEL=0 FREQSEL=1  WDT_OSC_CLK=250KHz
  	LPC_SYSCON->WDTCLKSEL = 0x2;        		// ѡ���Ź�ʱ��Դ ѡ���Ź�����
  	LPC_SYSCON->WDTCLKUEN = 0x01;          // ����ʱ��Դ
  	LPC_SYSCON->WDTCLKUEN = 0x00;          // ��д0����д1�ﵽ����Ŀ��
  	LPC_SYSCON->WDTCLKUEN = 0x01;
  	while ( !GET_BIT(LPC_SYSCON,WDTCLKUEN,0) );  // �ȴ����³ɹ�
  	LPC_SYSCON->WDTCLKDIV = 1;            // ���ÿ��Ź���ƵֵΪ1  
  	return;
}

/******************************************
* FunctionName   : WDT_Enable()
* Description    : ʹ�ܿ��Ź�  
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDT_Enable(void)
{
  	
	WDT_CLK_Setup();
	SET_BIT(LPC_SYSCON,SYSAHBCLKCTRL,15);// ����WDTʱ�ӣ����ʱ�������üĴ����õ�
  	LPC_WDT->TC = 80000;		// �����Ź���ʱ����ֵ����ʱʱ���Լ1��(������wdt_clk=250KHzʱ)8000*4*1/f
  	LPC_WDT->MOD |= 0x03;		// дֵ0x03����ι��������λ     дֵ0x01����ι�������ж�
 	LPC_WDT->FEED = 0xAA;		// ι���Ź�������
  	LPC_WDT->FEED = 0x55;    

	//NVIC_EnableIRQ(WDT_IRQn);	// ʹ�ܿ��Ź��ж�
  	return;
}

/******************************************
* FunctionName   : WDT_IRQHandler()
* Description    : ���Ź��жϺ��� 
                   ��MODֵ����Ϊ0x01ʱ�����û�м�ʱι���������������жϺ�����   
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDT_IRQHandler(void)
{
	if (GET_BIT(LPC_WDT, MOD, 3))
	{
		uart_send("wdt irq", 7);
	}
 	CLR_BIT(LPC_WDT,MOD,2);		// �忴�Ź���ʱ��־λWDTOF
	// ���������д�뵱���Ź��жϷ���ʱ����Ҫ��������
	
}

/******************************************
* FunctionName   : WDTFeed()
* Description    : ���Ź�ι��   
* EntryParameter : None
* ReturnValue    : None
*******************************************/
void WDTFeed(void)
{
  	LPC_WDT->FEED = 0xAA;		// ι��
  	LPC_WDT->FEED = 0x55;
  	return;
}