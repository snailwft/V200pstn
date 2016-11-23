#ifndef __NXPLPC11XX_WDT_H__
#define __NXPLPC11XX_WDT_H__

#include "config.h"

extern void wdt_enable(void);
extern void wdt_feed(void);
extern void WDT_IRQHandler(void);


#endif