#ifndef __NXPLPC11xx_CT_H__
#define __NXPLPC11xx_CT_H__

void time16b0_init(void);
void time16b0_int_init(uint32 ms);
void tim16b0_delay_ms(uint32 ms);
void tim16b0_delay_us(uint32 us);
void wait();
void time16b1_int_init(uint32 ms);
void time16b0_init_1200();
void time16b0_init_19200();
void time16b1_enable();
void time16b1_disable();
#endif

