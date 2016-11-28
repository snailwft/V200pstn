#ifndef   __NXPLPC11XX_UART_H__
#define   __NXPLPC11XX_UART_H__
#include "main.h"

typedef struct 
{
	uint8 uart_buf[BUF_MAX_SIZE];
	uint8 fsk_flag;
	int num;	
}ST_UART_BUF;

extern uint8 Recived_data; // �����ֽ�
extern void uart_init(uint32 baudrate);  // ��ʼ������
extern uint8 uart_recive(void);   // ���ڽ����ֽ�����
extern void uart_send_byte(uint8 byte); // ���ڷ����ֽ�����
extern void uart_send(uint8 *Buffer, uint32 Length); // ���ڷ�����������
extern void uart_irq_disable();
extern void uart_irq_enable();
extern void uart_recv_init();
extern int get_uart_recv_num();
uint8 recv_byte();
#endif

