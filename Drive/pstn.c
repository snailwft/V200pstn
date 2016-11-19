#include "pstn.h"
#include "config.h"

void clear_pstn_event()
{
	st_pstn.event = PSTN_EVENT_INIT;
	st_pstn.polacount = 0;
	st_pstn.ringcount = 0;
}

void pstn_init()
{
	memset(&st_pstn, 0x0, sizeof(st_pstn));
	clear_pstn_event();
}

void set_pstn_state(PSTN_MAIN_STATE  state)
{
	st_pstn.state = state;
}

PSTN_MAIN_STATE get_pstn_state()
{
	return st_pstn.state;
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
			set_pstn_state(PSTN_ONHOOK);
			break;

		case PSTN_ONHOOK:
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
					st_pstn.event = PSTN_EVENT_POLA; // 表示有dtmf来显
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
				}
			}
			else 
				st_pstn.polacount = 0;
			break;
	 }
}
