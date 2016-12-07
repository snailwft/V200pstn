#ifndef __GPIO_H 
#define __GPIO_H

void gpio_init(void);
void Delay_1ms(void);
void Delay_Nms(uint32 N);
uint8 recv_byte();
void fsk_buf_int();
int get_fsk_buf_num();

#endif
