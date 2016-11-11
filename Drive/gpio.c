#include "config.h"
#include "gpio.h"
/**************************************************************************************
* FunctionName   : gpio_init()
* Description    : GPIO��ʼ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void gpio_init(void)
{
    //��һ�������Ź������ã�����IO��������
	SET_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16); //ʹ��IOCONʱ��(bit16)
	LPC_IOCON->PIO1_9 = 0XD0; //��P1.9����Ϊ����IO������   								PSTN_PDWN_CPU
	LPC_IOCON->JTAG_TMS_PIO1_0 = 0XD1; //��P1.0����Ϊ����IO�����				PSTN_DOUT_MCU
	LPC_IOCON->JTAG_TDO_PIO1_1 = 0XD1; //��P1.1����Ϊ����IO�ţ��ֽڲ�����	PSTN_CDET_MCU								
	LPC_IOCON->JTAG_nTRST_PIO1_2 = 0XD1; // ����Ϊgpio��								PSTN_RDET_MCU
	LPC_IOCON->JTAG_TDI_PIO0_11 = 0XD1;	 // 												PSTN_DOUTC_MCU
	LPC_IOCON->PIO1_8 = 0XD0;						//													DTMF_Q1_MCU
	//LPC_IOCON->PIO1_9 = 0XD0;						//													DTMF_Q2_MCU   ��ht9032 down��  �غ���
	LPC_IOCON->PIO1_10 = 0XD0; 					//��P1.10����Ϊ����IO�ţ��ֽڲ�����DTMF_Q3_MCU			PSTN_9032_OE
	LPC_IOCON->PIO1_11 = 0XD0; 					//��P1.11����Ϊ����IO�ţ��ֽڲ�����DTMF_Q4_MCU			ACOK_MCU
	LPC_IOCON->PIO0_8 = 0xD0;						//��p0.8����Ϊ����IO��					PSTN_RING_MCU
	LPC_IOCON->PIO0_9 = 0xD0;						//��p0.9����Ϊ����IO��					PSTN_OH_MCU
	LPC_IOCON->PIO0_7 = 0xD0;						//��p0.7����Ϊ����IO��		
	LPC_IOCON->PIO0_4 = 0x00;						//p0.4 ��©����	����Ϊ����IO��		DTMF_StD_MCU
	LPC_IOCON->PIO2_0 = 0xD0;						//��p2.0����Ϊ����IO��
	
	CLR_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16);//����IOCONʱ��(bit16)������������ɺ�رո�ʱ�ӣ�

	//�ڶ��������ŷ������ã�����������
	SET_BIT(LPC_GPIO1,DIR,0); //��P1.0��P1.1��������Ϊ���
	CLR_BIT(LPC_GPIO1,DIR,1); 	// ����p1.1Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,2);  //��p1.2����Ϊ����
	
	SET_BIT(LPC_GPIO1,DIR,9); //��P1.9����Ϊ���
	CLR_BIT(LPC_GPIO1,DIR,10);  //��p1.10����Ϊ����
	
	CLR_BIT(LPC_GPIO1,DIR,11);//��P1.11����Ϊ����
	CLR_BIT(LPC_GPIO0,DIR,11);//��P0.11����Ϊ����
	CLR_BIT(LPC_GPIO0, DIR, 8);//��p0.8����Ϊ����
	SET_BIT(LPC_GPIO0, DIR, 9);//��p0.9����Ϊ���
	SET_BIT(LPC_GPIO0, DIR, 7);//��p0.7����Ϊ���
	SET_BIT(LPC_GPIO2, DIR, 0);//��p2.0����Ϊ���

	//�������������ж�
	CLR_BIT(LPC_GPIO0,IS,11); //ѡ��P0.11Ϊ���ش���   PSTN_DOUTC_MCU
	CLR_BIT(LPC_GPIO0,IEV,11);//ѡ��P0.11Ϊ�½��ش��� 
	SET_BIT(LPC_GPIO0,IE,11); //����P0.11�жϲ�������
	CLR_BIT(LPC_GPIO0,IS,8); //ѡ��P0.8Ϊ���ش���   PSTN_RING_MCU
	CLR_BIT(LPC_GPIO0,IEV,8);//ѡ��P0.8Ϊ�½��ش��� 
	SET_BIT(LPC_GPIO0,IE,8); //����P0.8�жϲ�������
	CLR_BIT(LPC_GPIO1,IS,2); //ѡ��P1.2Ϊ���ش���   PSTN_RDET_MCU
	CLR_BIT(LPC_GPIO1,IEV,2);//ѡ��P1.2Ϊ�½��ش��� 
	SET_BIT(LPC_GPIO1,IE,2); //����P1.2�жϲ�������
	CLR_BIT(LPC_GPIO1,IS,1); //ѡ��P1.1Ϊ���ش���   PSTN_CDET_MCU
	SET_BIT(LPC_GPIO1,IEV,1);//ѡ��P1.1Ϊ�����ش��� 
	SET_BIT(LPC_GPIO1,IE,1); //����P1.1�жϲ�������
	
	//���Ĳ�����GPIO1�ж�
	NVIC_EnableIRQ(EINT1_IRQn);	// ʹ��GPIO1�ж�
	NVIC_EnableIRQ(EINT0_IRQn);	// ʹ��GPIO0�ж�
}

