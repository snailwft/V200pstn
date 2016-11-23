#ifndef _DTMF_
#define _DTMF_

#include "config.h"
#include <stdio.h>
#include <string.h>
#define        MAX_DTMF_BUFF                100

typedef struct
{
	uchar  rx_addr;                        //接收指针地址
	char   dtmf_buff[MAX_DTMF_BUFF];       //接收缓冲
}ST_DTMF_RXBUFF;

extern ST_DTMF_RXBUFF dtmf_rx_buf;

#endif