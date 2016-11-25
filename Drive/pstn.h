#ifndef _PSTN_H_
#define _PSTN_H_
#include "config.h"

typedef enum PSTN_MAINSTATE_TAG
{
	PSTN_INIT = 0,
	PSTN_ONHOOK,            //挂机
	PSTN_OFFHOOK,           //摘机 
}PSTN_MAIN_STATE;

typedef enum PSTN_SUB_EVENT_TAG
{
	PSTN_EVENT_INIT = 0,
	PSTN_EVENT_POLA,		  //反极
	PSTN_EVENT_RING,          //振铃
}PSTN_SUB_EVENT;

typedef enum CID_MODE
{
	PSTN_CID_IDL = 0,
	PSTN_FSK,
	PSTN_DTMF,
}PSTN_CID_MODE;

typedef struct
{
	PSTN_MAIN_STATE  state;     //主状态
	PSTN_SUB_EVENT   event;     //次状态
	PSTN_CID_MODE	   cid_mode;
	uint8            ringcount;		 //振铃计数
	uint8            polacount;       //极性计数
	uint8            delay_time;    //检测极性和振铃延时
	uint8            last_pola;     	//上次极性
	int              pstn_checktime;//检测外线延时
}ST_PSTN;

extern ST_PSTN   st_pstn;

extern void clear_pstn_event();
extern void pstn_init();
extern void set_pstn_state(PSTN_MAIN_STATE  state);
extern PSTN_MAIN_STATE get_pstn_state();
extern void check_pstn_hook();
#endif