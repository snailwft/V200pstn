#ifndef _DTMF_
#define _DTMF_

#include "config.h"
#include <stdio.h>
#include <string.h>
#define        MAX_DTMF_BUFF                100

typedef struct
{
	uchar  rx_addr;                        //����ָ���ַ
	char   dtmf_buff[MAX_DTMF_BUFF];       //���ջ���
}ST_DTMF_RXBUFF;

extern ST_DTMF_RXBUFF dtmf_rx_buf;

#endif