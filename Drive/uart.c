#include "config.h"
#include "uart.h"

/*
 *RING:1/0:CID:0704121313016054035:HOOK:1/0*
 1:��ʾ�����ժ��
 0:��ʾ�������һ�
*/

//����һ���ַ� 
uint8 recv_byte() 
{      
	uint8 Output = 0;
	uint8 i = 8;
	//SET_BIT(LPC_GPIO1,DATA,9);				  // ��LED2	
	LPC_TMR16B0->TCR = 0x01;  //����Timer16 0
	wait(); //�ȹ���ʼλ	
	//CLR_BIT(LPC_GPIO1,DATA,9);  	 		  // ��LED2		
	while(i--) //����8λ����
	{
		Output >>= 1; //���յ�λ
		if(GET_BIT(LPC_GPIO0,DATA,11) > 0) 		//�ߵ�ƽ
			Output |= 0x80;			
		wait(); 			//λ����ʱ		
	}
	LPC_TMR16B0->TCR = 0x00;  	//ֹͣTimer0
	return Output;
}
/********************************************************
* FunctionName   : UART_Init()
* Description    : ���ڳ�ʼ��
* EntryParameter : baudrate-������
* ReturnValue    : None
*********************************************************/
void uart_init(uint32 baudrate)
{
	uint32 DL_value,Clear=Clear;   // (�����ַ�ʽ������������������Warning)                       
	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // ʹ��IOCONʱ�� 
	LPC_IOCON->PIO1_6 &= ~0x07;    
	LPC_IOCON->PIO1_6 |= 0x01; //��P1.6������ΪRXD
	LPC_IOCON->PIO1_7 &= ~0x07;	
	LPC_IOCON->PIO1_7 |= 0x01; //��P1.7������ΪTXD
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // ����IOCONʱ��

	LPC_SYSCON->UARTCLKDIV = 0x1;	//ʱ�ӷ�ƵֵΪ1
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);//����UARTʱ��
	LPC_UART->LCR = 0x83;   						//8λ���䣬1��ֹͣλ���޼�żУ�飬������ʳ���������
	DL_value = 48000000/16/baudrate ;  		//����ò�����Ҫ��ĳ�������Ĵ���ֵ
	LPC_UART->DLM = DL_value / 256;	   	//д������������λֵ			
	LPC_UART->DLL = DL_value % 256;	  	//д������������λֵ
	LPC_UART->LCR = 0x03;    						//DLAB��0	  ��ֹ���ʳ���������
	LPC_UART->FCR = 0x07;    						//����FIFO�����RxFIFO �� TxFIFO
	Clear = LPC_UART->LSR;   						//��UART״̬�Ĵ�������ղ���״̬

	LPC_UART->IER = 0x01;							//ֻ��������жϣ��ر������ж�
	NVIC_EnableIRQ(UART_IRQn); 				//���������ж�
}

void uart_irq_disable()
{
	uint32 Clear=Clear; 
	LPC_UART->FCR = 0x03;    //����FIFO�����RxFIFO 
	Clear = LPC_UART->LSR;   //��UART״̬�Ĵ�������ղ���״̬
	LPC_UART->IER = 0x00;	//ֻ��������жϣ��ر������ж�
}

void uart_irq_enable()
{
	uint32 Clear=Clear; 
	LPC_UART->FCR = 0x03;    //����FIFO�����RxFIFO 
	Clear = LPC_UART->LSR;   //��UART״̬�Ĵ�������ղ���״̬
	LPC_UART->IER = 0x01;	//ֻ��������жϣ��ر������ж�
}

/********************************************************
* FunctionName   : UART_Recive()
* Description    : ���ڽ���һ���ֽ�
* EntryParameter : None
* ReturnValue    : �յ����ֽ�
*********************************************************/
uint8 uart_recive(void)
{	
	while(!(LPC_UART->LSR & (1<<0)));//�ȴ����յ�����
	return(LPC_UART->RBR);			 //��������
}

/********************************************************
* FunctionName   : uart_send_byte()
* Description    : ���ڷ���һ���ֽ�
* EntryParameter : byte-��Ҫ���͵��ֽ�
* ReturnValue    : None
*********************************************************/
void uart_send_byte(uint8 byte)
{
	while ( !(LPC_UART->LSR & (1<<5)) );//�ȴ�������
	LPC_UART->THR = byte;
}

/********************************************************
* FunctionName   : UART_Send()
* Description    : ���ڷ�������
* EntryParameter : *Buffer-���� ; Length-����
* ReturnValue    : None
*********************************************************/
void uart_send(uint8 *Buffer, uint32 Length)
{
	while(Length != 0)
	{
		while ( !(LPC_UART->LSR & (1<<5)) );//�ȴ�������
		LPC_UART->THR = *Buffer;
		Buffer++;
		Length--;
	}
}


