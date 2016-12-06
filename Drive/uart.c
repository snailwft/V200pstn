#include "config.h"
#include "uart.h"
#include "pstn.h"

ST_UART_BUF uartrecv, uartsend;
extern uint8 uartrecv_buf[BUF_MAX_SIZE], uartsend_buf[BUF_MAX_SIZE];					//用来作为模拟串口接收数据的缓存  
extern uint8 recv_num, fsk_flag, fsk_clear;
/*
 *RING:1/0:CID:0704121313016054035:HOOK:1/0*
 1:表示振铃或摘机
 0:表示不振铃或挂机
*/

/********************************************************
* FunctionName   : UART_Init()
* Description    : 串口初始化
* EntryParameter : baudrate-波特率
* ReturnValue    : None
*********************************************************/
void uart_init(uint32 baudrate)
{
	uint32 DL_value,Clear=Clear;   // (用这种方式定义变量解决编译器的Warning)                       
	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // 使能IOCON时钟 
	LPC_IOCON->PIO1_6 &= ~0x07;    
	LPC_IOCON->PIO1_6 |= 0x01; //把P1.6脚设置为RXD
	LPC_IOCON->PIO1_7 &= ~0x07;	
	LPC_IOCON->PIO1_7 |= 0x01; //把P1.7脚设置为TXD
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // 禁能IOCON时钟

	LPC_SYSCON->UARTCLKDIV = 0x1;	//时钟分频值为1
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);//允许UART时钟
	LPC_UART->LCR = 0x83;   						//8位传输，1个停止位，无几偶校验，允许访问除数锁存器
	DL_value = 48000000/16/baudrate ;  		//计算该波特率要求的除数锁存寄存器值
	LPC_UART->DLM = DL_value / 256;	   	//写除数锁存器高位值			
	LPC_UART->DLL = DL_value % 256;	  	//写除数锁存器低位值
	LPC_UART->LCR = 0x03;    						//DLAB置0	  禁止访问除数锁存器
	LPC_UART->FCR = 0x07;    						//允许FIFO，清空RxFIFO 和 TxFIFO
	Clear = LPC_UART->LSR;   						//读UART状态寄存器将清空残留状态

	LPC_UART->IER = 0x01;							//只允许接收中断，关闭其他中断
	NVIC_EnableIRQ(UART_IRQn); 				//开启串口中断
}

void uart_irq_disable()
{
	uint32 Clear=Clear; 
	LPC_UART->FCR = 0x03;    //允许FIFO，清空RxFIFO 
	Clear = LPC_UART->LSR;   //读UART状态寄存器将清空残留状态
	LPC_UART->IER = 0x00;	//关闭中断
}

void uart_irq_enable()
{
	uint32 Clear=Clear; 
	LPC_UART->FCR = 0x03;    //允许FIFO，清空RxFIFO 
	Clear = LPC_UART->LSR;   //读UART状态寄存器将清空残留状态
	LPC_UART->IER = 0x01;	//只允许接收中断，关闭其他中断
}

/********************************************************
* FunctionName   : UART_Recive()
* Description    : 串口接收一个字节
* EntryParameter : None
* ReturnValue    : 收到的字节
*********************************************************/
uint8 uart_recive(void)
{	
	while(!(LPC_UART->LSR & (1<<0)));//等待接收到数据
	return(LPC_UART->RBR);			 //读出数据
}

/********************************************************
* FunctionName   : uart_send_byte()
* Description    : 串口发送一个字节
* EntryParameter : byte-需要发送的字节
* ReturnValue    : None
*********************************************************/
void uart_send_byte(uint8 byte)
{
	while ( !(LPC_UART->LSR & (1<<5)) );//等待发送完
	LPC_UART->THR = byte;
}

/********************************************************
* FunctionName   : UART_Send()
* Description    : 串口发送数组
* EntryParameter : *Buffer-数组 ; Length-长度
* ReturnValue    : None
*********************************************************/
void uart_send(uint8 *Buffer, uint32 Length)
{
	while(Length != 0)
	{
		while ( !(LPC_UART->LSR & (1<<5)) );//等待发送完
		LPC_UART->THR = *Buffer;
		Buffer++;
		Length--;
	}
}

void uart_recv_init()
{
	memset(&uartrecv, 0, sizeof(uartrecv));
}

int get_uart_recv_num()
{
	return uartrecv.num;
}
#if 1
void UART_IRQHandler(void)
{
	uint32 IRQ_ID;		  				// 定义读取中断ID号变量
	uint8 redata;    						// 定义接收数据变量数组
	
	IRQ_ID = LPC_UART->IIR;   	// 读中断ID号
	IRQ_ID =((IRQ_ID>>1)&0x7);	// 检测bit4:bit1	
	if(IRQ_ID == 0x02 )		  			// 检测是不是接收数据引起的中断
	{
		while (LPC_UART->LSR & 0x1)
		{
			redata = LPC_UART->RBR;
			if (get_pstn_cid_mode() == PSTN_FSK)
			{
				if (redata == 0x55) 			//来显数据头
				{
					uartrecv.fsk_flag = 1;
					if (uartrecv.num > 30) // 0x55数量最多不会超过20，如果大于30表示uartrecv_buf填充了很多垃圾数据
					{
						uartrecv.num = 0;
					}
				}
				if (uartrecv.num < BUF_MAX_SIZE && uartrecv.fsk_flag == 1) 	//存在风险，万一recv_num没有清0
				{
					uartrecv.uart_buf[uartrecv.num++] = redata;	  				//从RXFIFO中读取接收到的数据 ，控制数据量
				}
			}
			else 
			{
				if (uartrecv.num >= BUF_MAX_SIZE2)
				{
					uartrecv.num = 0;
				}
				uartrecv.uart_buf[uartrecv.num++] = redata;	
			}
		}
	}
}
#endif

