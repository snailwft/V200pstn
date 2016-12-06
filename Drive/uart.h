#ifndef   __NXPLPC11XX_UART_H__
#define   __NXPLPC11XX_UART_H__
#include "main.h"

typedef struct 
{
	uint8 uart_buf[BUF_MAX_SIZE];
	uint8 fsk_flag;
	int num;	
}ST_UART_BUF;

extern uint8 Recived_data; // 接收字节
extern void uart_init(uint32 baudrate);  // 初始化串口
extern uint8 uart_recive(void);   // 串口接收字节数据
extern void uart_send_byte(uint8 byte); // 串口发送字节数据
extern void uart_send(uint8 *Buffer, uint32 Length); // 串口发送数组数据
extern void uart_irq_disable();
extern void uart_irq_enable();
extern void uart_recv_init();
extern int get_uart_recv_num();
#endif

