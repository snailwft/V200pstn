#ifndef _PSTN_H_
#define _PSTN_H_
#include "config.h"

typedef enum PSTN_MAINSTATE_TAG
{
	PSTN_INIT = 0,
	PSTN_ONHOOK,            //�һ�
	PSTN_OFFHOOK,           //ժ�� 
}PSTN_MAIN_STATE;

typedef enum PSTN_SUB_EVENT_TAG
{
	PSTN_EVENT_INIT = 0,
	PSTN_EVENT_POLA,		  //����
	PSTN_EVENT_RING,          //����
}PSTN_SUB_EVENT;

typedef enum CID_MODE
{
	PSTN_CID_IDL = 0,
	PSTN_FSK,
	PSTN_DTMF,
}PSTN_CID_MODE;

typedef struct
{
	PSTN_MAIN_STATE  state;     //��״̬
	PSTN_SUB_EVENT   event;     //��״̬
	PSTN_CID_MODE	   cid_mode;
	uint8            ringcount;		 //�������
	uint8            polacount;       //���Լ���
	uint8            delay_time;    //��⼫�Ժ�������ʱ
	uint8            last_pola;     	//�ϴμ���
	int              pstn_checktime;//���������ʱ
}ST_PSTN;

extern ST_PSTN   st_pstn;

extern void clear_pstn_event();
extern void pstn_init();
extern void set_pstn_state(PSTN_MAIN_STATE  state);
extern PSTN_MAIN_STATE get_pstn_state();
extern void check_pstn_hook();
#endif