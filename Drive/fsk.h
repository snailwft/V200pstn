#ifndef _HT9032_H_
#define _HT9032_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SINGLE_MODE		0x04		  // 单模式
#define COMPLEX_MODE	0x80		  // 复合模式
#define COMPLEX_M_TIME	0x01   // 时间
#define COMPLEX_M_DIAL	0x02   // 号码
#define COMPLEX_M_NAME	0x07   // 姓名
#define FSK_HEAD_ID		0x55
#define MAX_FSKBUF		512
#define FSK_COMFIRM		5

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct
{
	uchar ucFSK_Buf[512];			//FSK数据缓存
	uchar ucRecCnt;						//收到的FSK数据数量
	uchar ucRecHeadCnt;				//收到多少FSK数据头
	uchar ucTime[10];
	uchar ucFskNum[30];				//号码缓存
	uchar ucNumLength;				//号码长度
	uchar ucGetFlag;						//收到号码标志
}ST_FSK_MEG_STATE;

extern ST_FSK_MEG_STATE stFskMeg;
extern uchar fsk_ucgetflag;
void fsk_init();
int CheckFSKMessage(uchar * DataBuf, uchar DataLength);

#endif
