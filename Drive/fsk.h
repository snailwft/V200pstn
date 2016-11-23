#ifndef _HT9032_H_
#define _HT9032_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SINGLE_MODE		0x04		  // ��ģʽ
#define COMPLEX_MODE	0x80		  // ����ģʽ
#define COMPLEX_M_TIME	0x01   // ʱ��
#define COMPLEX_M_DIAL	0x02   // ����
#define COMPLEX_M_NAME	0x07   // ����
#define FSK_HEAD_ID		0x55
#define MAX_FSKBUF		512
#define FSK_COMFIRM		5

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct
{
	uchar ucFSK_Buf[512];			//FSK���ݻ���
	uchar ucRecCnt;						//�յ���FSK��������
	uchar ucRecHeadCnt;				//�յ�����FSK����ͷ
	uchar ucTime[10];
	uchar ucFskNum[30];				//���뻺��
	uchar ucNumLength;				//���볤��
	uchar ucGetFlag;						//�յ������־
}ST_FSK_MEG_STATE;

extern ST_FSK_MEG_STATE stFskMeg;
extern uchar fsk_ucgetflag;
void fsk_init();
int CheckFSKMessage(uchar * DataBuf, uchar DataLength);

#endif
