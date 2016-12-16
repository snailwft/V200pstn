#include "pstn.h"
#include "config.h"
#include "main.h"

ST_PSTN   st_pstn;
extern uint8 uartsend_buf[BUF_MAX_SIZE];

void clear_pstn_event()
{
	st_pstn.event = PSTN_EVENT_INIT;
	st_pstn.polacount = 0;
	st_pstn.ringcount = 0;
}

void pstn_init()
{
	memset(&st_pstn, 0x0, sizeof(st_pstn));
}

void set_pstn_state(PSTN_MAIN_STATE  state)
{
	st_pstn.state = state;
}

PSTN_MAIN_STATE get_pstn_state()
{
	return st_pstn.state;
}

PSTN_SUB_EVENT  get_pstn_event()
{
	return st_pstn.event;
}

void set_pstn_event(PSTN_SUB_EVENT event)
{
	st_pstn.event = event;
}

void set_pstn_cid_mode(PSTN_CID_MODE cid_mode)
{
	st_pstn.cid_mode = cid_mode;
}

PSTN_CID_MODE get_pstn_cid_mode()
{
	return st_pstn.cid_mode;
}

void check_pstn_hook()
{
	uint8 temp;

	 switch(st_pstn.state)
	 {
		case PSTN_INIT:
			if (GET_BIT(LPC_GPIO3, DATA, 2)) // 获取极性电平		
				temp = 1;
			else 
				temp = 0;
			st_pstn.last_pola = temp;
			clear_pstn_event();
			set_pstn_cid_mode(PSTN_CID_IDL);
			set_pstn_state(PSTN_ONHOOK);
			break;

		case PSTN_ONHOOK:
			if (GET_BIT(LPC_GPIO3, DATA, 2)) // 获取极性电平		电平一直在高低变化
				temp = 1;
			else 
				temp = 0;
			if (st_pstn.last_pola != temp)
			{
				st_pstn.polacount++;
				if (st_pstn.polacount > 5)
				{
					st_pstn.last_pola = temp;
					st_pstn.polacount = 0;
#if 1
					if (st_pstn.cid_mode == PSTN_CID_IDL) // 万一还没收到来显就挂了，同时也没有振铃,对方挂机pstn网络应该会发忙音来通知，是不需要协处理器来做了  
					{
						st_pstn.cid_mode = PSTN_DTMF;
						st_pstn.event = PSTN_EVENT_POLA; // 表示有dtmf来显,接着就会有振铃,让接收dtmf来显的芯片一直使能
					}
#endif
					else
					{
						st_pstn.event = PSTN_EVENT_POLA;
#if 0
						memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
						sprintf(uartsend_buf, "&RING:%d:CID::HOOK:%d:HANG:%d*", 1, 0, 1); 	//来电振铃通知主控振铃
						uart_send(uartsend_buf, strlen(uartsend_buf)); 	//发送给主控
#endif
					}
				}
			}
			else 
				st_pstn.polacount = 0;
			break;

		case PSTN_OFFHOOK:
			if (GET_BIT(LPC_GPIO3, DATA, 2)) // 获取极性电平		
				temp = 1;
			else 
				temp = 0;
			if (st_pstn.last_pola != temp)
			{
				st_pstn.polacount++;
				if (st_pstn.polacount > 5)
				{
					st_pstn.last_pola = temp;
					st_pstn.polacount = 0;
					st_pstn.event = PSTN_EVENT_POLA; //表示对方挂机了
#if 0
					memset(uartsend_buf, 0x0, sizeof(uartsend_buf));
					sprintf(uartsend_buf, "&RING:%d:CID::HOOK:%d:HANG:%d*", 1, 0, 1); 	//来电振铃通知主控振铃
					uart_send(uartsend_buf, strlen(uartsend_buf)); 	//发送给主控
#endif
				}
			}
			else 
				st_pstn.polacount = 0;
			break;
	 }
#if 0
	CPL_BIT(LPC_GPIO0,DATA,9); //摘挂机	
	//CLR_BIT(LPC_GPIO0,DATA,9); // 摘机
	//delay(2000);
	if (GET_BIT(LPC_GPIO3, DATA, 4))
	{
		uart_send("loop = 1\n", strlen("loop = 1\n"));
	}
	else 
	{
		uart_send("loop = 0\n", strlen("loop = 0\n"));
	}

	if (GET_BIT(LPC_GPIO3, DATA, 5))
	{
		uart_send("line not in use\n", strlen("line not in use\n"));		
	}
	else 
	{
		uart_send("line in use\n", strlen("line in use\n"));
	}
#endif

}
