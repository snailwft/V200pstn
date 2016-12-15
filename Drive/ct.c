#include "config.h"
#include "ct.h"
#include "main.h"
#include "fsk.h"
#include "pstn.h"
#include "uart.h"

extern uint8 uartrecv_buf[BUF_MAX_SIZE], uartsend_buf[BUF_MAX_SIZE];					//������Ϊģ�⴮�ڽ������ݵĻ���  
extern uint8 recv_num, ring_times, ring_num, fsk_flag;

void wait()
{
	while((LPC_TMR16B0->IR & 0x1)!=1);
	LPC_TMR16B0->IR = 0x1F; // �����ж�ʱ��/�������жϱ�־	
}

/**************************************************************************************
* FunctionName   : time16b0_init()
* Description    : ʹ��TIM16B0ʱ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void time16b0_init(void) 
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	
}

/**************************************************************************************
* FunctionName   : time16b0_int_init()
* Description    : ��ʼ��TIM16B0
* EntryParameter : ms-��ʱ����ֵ    
* ReturnValue    : None
**************************************************************************************/
void time16b0_int_init(uint32 ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//ʹ��TIM16B0ʱ��
	LPC_TMR16B0->TCR = 0x02;							//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = 1000;							//��Ԥ��Ƶ�Ĵ�����0��ʹPC+1��TC+1
	//LPC_TMR16B0->PR  = 0x07ff;						//��Ԥ��Ƶ�Ĵ�����0x7fff����PCÿ����2048,TC+1
	LPC_TMR16B0->MR0 = ms * 48;						//��48Mhz�¹����ıȽ�ֵ���������޸�
	LPC_TMR16B0->IR  = 0x01;								//MR0�жϱ�־��λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x03;							//MR0��TCֵƥ��ʱ�����жϣ�MR0��TCֵƥ��ʱʹTC��λ��TC���¼���
	LPC_TMR16B0->TCR = 0x00;							//������ʱ����TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // ʹ��TIM32B0�ж�	
}

void time16b0_int_1200()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//ʹ��TIM16B0ʱ��
	LPC_TMR16B0->TCR = 0x02;			//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = 100;				//��Ԥ��Ƶ�Ĵ�����0��ʹPC+1��TC+1
	LPC_TMR16B0->MR0 = 400;				//��48Mhz�¹����ıȽ�ֵ���������޸�
	//LPC_TMR16B0->MR0 = 433;	   //���ʺܵ� ���е��
	//LPC_TMR16B0->MR0 = 434;
	//LPC_TMR16B0->MR0 = 435;	   //������
	LPC_TMR16B0->IR  = 0x01;				//MR0�жϱ�־��λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x03;			//MR0��TCֵƥ��ʱ�����жϣ�MR0��TCֵƥ��ʱʹTC��λ
	LPC_TMR16B0->TCR = 0x00;			//�رն�ʱ����TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // ʹ��TIM32B0�ж�	��ע���жϺ��������ж���ʱ��ֹ�����жϺ���
}

void time16b0_init_19200()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//ʹ��TIM16B0ʱ��
	LPC_TMR16B0->TCR = 0x02;			//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = 100;				//��Ԥ��Ƶ�Ĵ�����0��ʹPC+1��TC+1
	LPC_TMR16B0->MR0 = 25;				//��48Mhz�¹����ıȽ�ֵ���������޸�
	//LPC_TMR16B0->MR0 = 433;	   //���ʺܵ� ���е��
	//LPC_TMR16B0->MR0 = 434;
	//LPC_TMR16B0->MR0 = 435;	   //������
	LPC_TMR16B0->IR  = 0x01;				//MR0�жϱ�־��λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x03;			//MR0��TCֵƥ��ʱ�����жϣ�MR0��TCֵƥ��ʱʹTC��λ
	LPC_TMR16B0->TCR = 0x00;			//�رն�ʱ����TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // ʹ��TIM32B0�ж�	��ע���жϺ��������ж���ʱ��ֹ�����жϺ���
}

void time16b0_init_2400()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//ʹ��TIM16B0ʱ��
	LPC_TMR16B0->TCR = 0x02;			//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = 100;				//��Ԥ��Ƶ�Ĵ�����0��ʹPC+1��TC+1
	LPC_TMR16B0->MR0 = 200;				//��48Mhz�¹����ıȽ�ֵ���������޸�
	//LPC_TMR16B0->MR0 = 433;	   //���ʺܵ� ���е��
	//LPC_TMR16B0->MR0 = 434;
	//LPC_TMR16B0->MR0 = 435;	   //������
	LPC_TMR16B0->IR  = 0x01;				//MR0�жϱ�־��λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x03;			//MR0��TCֵƥ��ʱ�����жϣ�MR0��TCֵƥ��ʱʹTC��λ
	LPC_TMR16B0->TCR = 0x00;			//�رն�ʱ����TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // ʹ��TIM32B0�ж�	��ע���жϺ��������ж���ʱ��ֹ�����жϺ���
}

#if 0
void time16b0_int_1200()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	//ʹ��TIM16B0ʱ��
	LPC_TMR16B0->TCR = 0x02;			//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = 40;				//��Ԥ��Ƶ�Ĵ�����0��ʹPC+1��TC+1
	//LPC_TMR16B0->MR0 = 400;				//��48Mhz�¹����ıȽ�ֵ���������޸�
	//LPC_TMR16B0->MR0 = 433;	   //���ʺܵ� ���е��
	LPC_TMR16B0->MR0 = 1072;
	//LPC_TMR16B0->MR0 = 435;	   //������
	LPC_TMR16B0->IR  = 0x01;				//MR0�жϱ�־��λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x03;			//MR0��TCֵƥ��ʱ�����жϣ�MR0��TCֵƥ��ʱʹTC��λ
	LPC_TMR16B0->TCR = 0x00;			//�رն�ʱ����TCR[0]=1;
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);   // ʹ��TIM32B0�ж�	��ע���жϺ��������ж���ʱ��ֹ�����жϺ���
}
#endif
/**************************************************************************************
* FunctionName   : tim16b0_delay_ms()
* Description    : TIM16B0������ʱ
* EntryParameter : ms-��ʱ����ֵ    
* ReturnValue    : None
**************************************************************************************/
void tim16b0_delay_ms(uint32 ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);	
	LPC_TMR16B0->TCR = 0x02;			//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = ms;				//��Ԥ��Ƶ�Ĵ�����1000��ʹPC+1��TC+1
	LPC_TMR16B0->MR0 = 1000 * 48;	//��48Mhz�¹�����ֵ���������޸�
	LPC_TMR16B0->IR  = 0x01;		 		//MR0�жϸ�λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x04;			//MR0���ֹ�жϣ�MR0��TCֵƥ��ʱֹͣTC��PC����ʹTCR[0]=0, ֹͣ��ʱ������
	LPC_TMR16B0->TCR = 0x01;			//������ʱ����TCR[0]=1;
	  
	while (LPC_TMR16B0->TCR & 0x01);//�ȴ���ʱ����ʱʱ�䵽
}


/**************************************************************************************
* FunctionName   : tim16b0_delay_us()
* Description    : TIM16B0΢����ʱ
* EntryParameter : us-��ʱ΢��ֵ    
* ReturnValue    : None
**************************************************************************************/
void tim16b0_delay_us(uint32 us)
{
	LPC_TMR16B0->TCR = 0x02;		//��λ��ʱ����bit1��д1��λ��
	LPC_TMR16B0->PR  = 0x00;		//��Ԥ��Ƶ�Ĵ�����0��ʹPC+1��TC+1
	LPC_TMR16B0->MR0 = us * 48;	    //��48Mhz�¹�����ֵ���������޸�
	LPC_TMR16B0->IR  = 0x01;		//MR0�жϸ�λ��bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
	LPC_TMR16B0->MCR = 0x04;		//MR0�жϲ���ʱֹͣTC��PC����ʹTCR[0]=0, ֹͣ��ʱ������
	LPC_TMR16B0->TCR = 0x01;		//������ʱ����TCR[0]=1;
	  
	while (LPC_TMR16B0->TCR & 0x01);//�ȴ���ʱ����ʱʱ�䵽
}

void time16b1_int_init(uint32 ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);    //��CT16B1��ʱ��ʱ��
    LPC_TMR16B1->TCR = 0x02;        					//��λ��ʱ��
    LPC_TMR16B1->PR  = ms;        					//����Ԥ��Ƶ����ʹ��1us TC+1
    LPC_TMR16B1->MR0 = 48 * 1000;						//��48Mhz�¹����ıȽ�ֵ���������޸�
    LPC_TMR16B1->IR  = 0x01;        						//�жϸ�λ
    LPC_TMR16B1->MCR = 0x03;							//MR0��TCֵƥ��ʱ�����жϣ�MR0��TCֵƥ��ʱʹTC��λ
    LPC_TMR16B1->TCR = 0x00; 							// �رն�ʱ��
    NVIC_EnableIRQ(TIMER_16_1_IRQn);    			// ����NVCI�ж����
}

void time16b1_enable()
{
	 LPC_TMR16B1->TCR = 0x01; 							// ��16λ��ʱ��1
}

void time16b1_disable()
{
	LPC_TMR16B1->TCR = 0x00; 							// �ر�16λ��ʱ��1
}

void TIMER16_1_IRQHandler(void)
{
	if((LPC_TMR16B1->IR & 0x1) == 1) 						// ����ǲ���MR0������ж�
	{	
		ring_times++;
		if (ring_num > 15)
		{			
			set_pstn_event(PSTN_EVENT_RING);
#if 1		//��Ҫ������֪ͨ���壬ֱ���յ�����֮����֪ͨ
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID::HOOK:%d*", 1, 0); 	//��������֪ͨ��������
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//���͸�����
#endif
		}
		if (ring_times > 6)
		{
#if 1		//��Ҫ����֪ͨ��
			memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
			sprintf(uartsend_buf, "&RING:%d:CID::HOOK:%d*", 0, 0);
			uart_send(uartsend_buf, strlen(uartsend_buf)); 	//���͸�����
#endif

			//CLR_BIT(LPC_GPIO1,DATA,9);  	 								//ht9032 ����PDWN��������ģʽ
			CLR_BIT(LPC_GPIO2, DATA, 0);									//uart��������
			set_pstn_cid_mode(PSTN_CID_IDL);
			fsk_buf_int();
			time16b1_disable();
		}
		ring_num = 0;
	}
	LPC_TMR16B1->IR = 0x1F; 									// �����ж�ʱ��/�������жϱ�־	
}


