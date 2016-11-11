#ifndef _HT9032_H_
#define _HT9032_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

#define SINGLE_MODE		0x04		  // ��ģʽ
#define COMPLEX_MODE	0x80		  // ����ģʽ
#define COMPLEX_M_TIME	0x01   // ʱ��
#define COMPLEX_M_DIAL	0x02   // ����
#define COMPLEX_M_NAME	0x07   // ����
#define FSK_HEAD_ID		0x55
#define MAX_FSKBUF		100
#define FSK_COMFIRM		5

typedef unsigned int uint;

typedef struct
{
	uint8 ucFSK_Buf[100];			//FSK���ݻ���
	uint8 ucRecCnt;						//�յ���FSK��������
	uint8 ucRecHeadCnt;				//�յ�����FSK����ͷ
	uint8 ucTime[10];
	uint8 ucFskNum[30];				//���뻺��
	uint8 ucNumLength;				//���볤��
	uint8 ucGetFlag;						//�յ������־
}ST_FSK_MEG_STATE;

extern ST_FSK_MEG_STATE stFskMeg;
extern uint8 fsk_ucgetflag;
extern uint8 recv_num;
void fsk_init();
int CheckFSKMessage(uint8 * DataBuf, uint8 DataLength);

#endif
