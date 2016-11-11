#ifndef _COM_DEAL_
#define _COM_DEAL_
#include "fsk.h"
int deal_message(uchar *databuf, uchar datalength);
int message_integrity(uchar *databuf);
#endif
