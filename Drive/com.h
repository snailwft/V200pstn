#ifndef _COM_DEAL_
#define _COM_DEAL_
#include "fsk.h"
extern int deal_message(uint8 *databuf, uint8 datalength);
extern int message_integrity(uint8 *databuf);
extern int message_parese_process(uint8 *buf);
#endif
