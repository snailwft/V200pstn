#include "config.h"
#include "uart.h"

/*
 *RING:1/0:CID:0704121313016054035:HOOK:1/0*
 1:表示振铃或摘机
 0:表示不振铃或挂机
*/

//接收一个字符 
uint8 recv_byte() 
{      
	uint8 Output = 0;
	uint8 i = 8;
	//SET_BIT(LPC_GPIO1,DATA,9);				  // 开LED2	
	LPC_TMR16B0->TCR = 0x01;  //启动Timer16 0
	wait(); //等过起始位	
	//CLR_BIT(LPC_GPIO1,DATA,9);  	 		  // 关LED2		
	while(i--) //接收8位数据
	{
		Output >>= 1; //先收低位
		if(GET_BIT(LPC_GPIO0,DATA,11) > 0) 		//高电平
			Output |= 0x80;			
		wait(); 			//位间延时		
	}
	LPC_TMR16B0->TCR = 0x00;  	//停止Timer0
	return Output;
}
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
	LPC_UART->IER = 0x00;	//只允许接收中断，关闭其他中断
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


