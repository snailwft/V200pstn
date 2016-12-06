#include "pstn.h"
#include "config.h"

ST_PSTN   st_pstn;

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
			if (GET_BIT(LPC_GPIO3, DATA, 2)) // ��ȡ���Ե�ƽ		
				temp = 1;
			else 
				temp = 0;
			st_pstn.last_pola = temp;
			clear_pstn_event();
			set_pstn_cid_mode(PSTN_CID_IDL);
			set_pstn_state(PSTN_ONHOOK);
			break;

		case PSTN_ONHOOK:
			if (GET_BIT(LPC_GPIO3, DATA, 2)) // ��ȡ���Ե�ƽ		��ƽһֱ�ڸߵͱ仯
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
					if (st_pstn.cid_mode == PSTN_CID_IDL) // ��һ��û�յ����Ծ͹��ˣ�ͬʱҲû������  
					{
						//st_pstn.cid_mode = PSTN_DTMF;
						//uart_send("PSTN_CID_IDL", 12);
						st_pstn.event = PSTN_EVENT_POLA; // ��ʾ��dtmf����,���žͻ�������
					}
					else
					{
						st_pstn.event = PSTN_EVENT_POLA;
						//����uart���ش�������
						//֪ͨ���ضԷ��ѹһ�
					}
				}
			}
			else 
				st_pstn.polacount = 0;
			break;

		case PSTN_OFFHOOK:
			if (GET_BIT(LPC_GPIO3, DATA, 2)) // ��ȡ���Ե�ƽ		
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
					st_pstn.event = PSTN_EVENT_POLA; //��ʾ�Է��һ���
					//����uart���ش�������
					//֪ͨ���ضԷ��ѹһ�
				}
			}
			else 
				st_pstn.polacount = 0;
			break;
	 }
}
