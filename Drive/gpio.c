#include "config.h"
#include "gpio.h"
#include "pstn.h"
#include "main.h"

extern uint8 ring_times, ring_num, uartsend_buf[BUF_MAX_SIZE];

/**************************************************************************************
* FunctionName   : gpio_init()
* Description    : GPIO��ʼ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void gpio_init(void)
{
    //��һ�������Ź������ã�����IO��������
	SET_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16); 	//ʹ��IOCONʱ��(bit16)
	LPC_IOCON->PIO1_9 = 0XD0;                        	//��P1.9����Ϊ����IO������   			PSTN_PDWN_CPU   DTMF_PDWN����	
	LPC_IOCON->JTAG_TDI_PIO0_11 = 0XD1;	 	//��p0.11����Ϊ����IO��					UART_SEL  mcu���ڽ��ն˿����л�		

	LPC_IOCON->JTAG_TMS_PIO1_0 = 0XD1; 		//��P1.0����Ϊ����IO�����					DTMF_Q1_MCU
	LPC_IOCON->JTAG_TDO_PIO1_1 = 0XD1; 		//��P1.1����Ϊ����IO�ţ��ֽڲ�����		DTMF_Q2_MCU	
	LPC_IOCON->JTAG_nTRST_PIO1_2 = 0XD1; 	//��P1.2����Ϊgpio��							DTMF_Q3_MCU
	LPC_IOCON->PIO1_8 = 0XD0;							//	��p1.8����Ϊ����IO��						DTMF_Q4_MCU
	LPC_IOCON->PIO1_10 = 0XD0; 						//��P1.10����Ϊ����IO�ţ��ֽڲ�����	DTMF_OE_MCU	
	LPC_IOCON->PIO1_11 = 0XD0; 						//��P1.11����Ϊ����IO�ţ��ֽڲ�����	DTMF_StD_MCU		
	
	LPC_IOCON->PIO0_7 = 0xD0;						//��p0.7����Ϊ����IO��		
	LPC_IOCON->PIO2_0 = 0xD0;						//��p2.0����Ϊ����IO��
	
	LPC_IOCON->PIO0_8 = 0xD0;						//��p0.8����Ϊ����IO��					PSTN_RING_MCU
	LPC_IOCON->PIO0_9 = 0xD0;						//��p0.9����Ϊ����IO��					PSTN_OH_MCU
	
	LPC_IOCON->PIO3_2 = 0xD0;						//��p3.2����Ϊ����IO��					POLARITY_CPC5712   	�������
	LPC_IOCON->PIO3_4 = 0xD0;						//��p3.4����Ϊ����IO��					LOOP_CPC5712				��·�������
	LPC_IOCON->PIO3_5 = 0xD0;						//��p3.5����Ϊ����IO��					Line_In_Use_CPC5712	æ�߼��   �͵�ƽ��ʾ�豸����ժ��״̬
		
	CLR_BIT(LPC_SYSCON,SYSAHBCLKCTRL,16);//����IOCONʱ��(bit16)������������ɺ�رո�ʱ�ӣ�

	//�ڶ��������ŷ������ã�����������
	CLR_BIT(LPC_GPIO1,DIR,0); 						//��P1.0����Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,1); 						//����p1.1Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,2); 						//��p1.2����Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,8); 						//��p1.8����Ϊ����
	
	SET_BIT(LPC_GPIO1,DIR,9); 						//��P1.9����Ϊ���
	SET_BIT(LPC_GPIO1,DIR,10);						//��p1.10����Ϊ���
	
	CLR_BIT(LPC_GPIO1,DIR,11);						//��P1.11����Ϊ����
	SET_BIT(LPC_GPIO0,DIR,11);						//��P0.11����Ϊ���
	CLR_BIT(LPC_GPIO0, DIR, 8);						//��p0.8����Ϊ����
	SET_BIT(LPC_GPIO0, DIR, 9);						//��p0.9����Ϊ���
	SET_BIT(LPC_GPIO0, DIR, 7);						//��p0.7����Ϊ���
	SET_BIT(LPC_GPIO2, DIR, 0);						//��p2.0����Ϊ���

	CLR_BIT(LPC_GPIO3, DIR, 2);						//��p3.2����Ϊ����
	CLR_BIT(LPC_GPIO3, DIR, 4);						//��p3.4����Ϊ����
	CLR_BIT(LPC_GPIO3, DIR, 5);						//��p3.5����Ϊ����


	//�������������ж�
	//CLR_BIT(LPC_GPIO0,IS,11); //ѡ��P0.11Ϊ���ش���   PSTN_DOUTC_MCU
	//CLR_BIT(LPC_GPIO0,IEV,11);//ѡ��P0.11Ϊ�½��ش��� 
	//SET_BIT(LPC_GPIO0,IE,11); //����P0.11�жϲ�������
	CLR_BIT(LPC_GPIO0,IS,8); //ѡ��P0.8Ϊ���ش���   PSTN_RING_MCU
	CLR_BIT(LPC_GPIO0,IEV,8);//ѡ��P0.8Ϊ�½��ش��� 
	SET_BIT(LPC_GPIO0,IE,8); //����P0.8�жϲ�������

	//CLR_BIT(LPC_GPIO1,IS,2); //ѡ��P1.2Ϊ���ش���   PSTN_RDET_MCU
	//CLR_BIT(LPC_GPIO1,IEV,2);//ѡ��P1.2Ϊ�½��ش��� 
	//SET_BIT(LPC_GPIO1,IE,2); //����P1.2�жϲ�������
	//CLR_BIT(LPC_GPIO1,IS,1); //ѡ��P1.1Ϊ���ش���   PSTN_CDET_MCU
	//SET_BIT(LPC_GPIO1,IEV,1);//ѡ��P1.1Ϊ�����ش��� 
	//SET_BIT(LPC_GPIO1,IE,1); //����P1.1�жϲ�������
	
	//���Ĳ�����GPIO1�ж�
	//NVIC_EnableIRQ(EINT1_IRQn);	// ʹ��GPIO1�ж�
	NVIC_EnableIRQ(EINT0_IRQn);	// ʹ��GPIO0�ж�
}

void set_dtmf_qn_dir()
{
	CLR_BIT(LPC_GPIO1,DIR,0); 						//��P1.0����Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,1); 						//����p1.1Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,2); 						//��p1.2����Ϊ����
	CLR_BIT(LPC_GPIO1,DIR,8); 						//��p1.8����Ϊ����
}

void clr_dtmf_qn_dir()
{
	SET_BIT(LPC_GPIO1,DIR,0); 						//��P1.0����Ϊ���
	SET_BIT(LPC_GPIO1,DIR,1); 						//����p1.1Ϊ���
	SET_BIT(LPC_GPIO1,DIR,2); 						//��p1.2����Ϊ���
	SET_BIT(LPC_GPIO1,DIR,8); 						//��p1.8����Ϊ���
}

void PIOINT0_IRQHandler(void)  	  
{
	if (GET_BIT(LPC_GPIO0, MIS, 8)!=0)	      // ���P0.8���Ų������ж� PSTN_RING_MCU �½��ش����ж�
	{
		ring_num++;
		if (ring_num > 15)
		{			
			if (get_pstn_cid_mode() != PSTN_DTMF)
			{
				SET_BIT(LPC_GPIO1, DATA,9);  	 				//���� ht9032 PDWN���빤��ģʽ		��Ϊ������˷������� 	
				SET_BIT(LPC_GPIO0, DATA, 11);					//���߽�ͨht9032����  ,�����dtmf���ԣ�����Ҫ����ht9032				
			}
			ring_num = 0;
			set_pstn_event(PSTN_EVENT_RING);
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID:%s%s:HOOK:%d*", 1, NULL, NULL, 0); 	//��������֪ͨ��������
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//���͸�����
		}
		//time16b1_enable();
		//ring_times = 0;
	}
	LPC_GPIO0->IC = 0xFFF;  						 	// ���GPIO0�ϵ��ж�
}

