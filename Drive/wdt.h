#ifndef __NXPLPC11XX_WDT_H__
#define __NXPLPC11XX_WDT_H__

#include "config.h"

extern void WDT_Enable(void);
extern void WDTFeed(void);
extern void WDT_IRQHandler(void);


#endif