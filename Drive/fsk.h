#ifndef _HT9032_H_
#define _HT9032_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

#define SINGLE_MODE		0x04		  // 单模式
#define COMPLEX_MODE	0x80		  // 复合模式
#define COMPLEX_M_TIME	0x01   // 时间
#define COMPLEX_M_DIAL	0x02   // 号码
#define COMPLEX_M_NAME	0x07   // 姓名
#define FSK_HEAD_ID		0x55
#define MAX_FSKBUF		100
#define FSK_COMFIRM		5

typedef unsigned int uint;

typedef struct
{
	uint8 ucFSK_Buf[100];			//FSK数据缓存
	uint8 ucRecCnt;						//收到的FSK数据数量
	uint8 ucRecHeadCnt;				//收到多少FSK数据头
	uint8 ucTime[10];
	uint8 ucFskNum[30];				//号码缓存
	uint8 ucNumLength;				//号码长度
	uint8 ucGetFlag;						//收到号码标志
}ST_FSK_MEG_STATE;

extern ST_FSK_MEG_STATE stFskMeg;
extern uint8 fsk_ucgetflag;
extern uint8 recv_num;
void fsk_init();
int CheckFSKMessage(uint8 * DataBuf, uint8 DataLength);

#endif
