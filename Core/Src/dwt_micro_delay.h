#ifndef LIBS_DWT_MICRO_DELAY_DWT_MICRO_DELAY_H_
#define LIBS_DWT_MICRO_DELAY_DWT_MICRO_DELAY_H_

#include <stdint.h>


#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC


void DWT_init(void);

void delay_micros(uint32_t us);

#endif /* LIBS_DWT_MICRO_DELAY_DWT_MICRO_DELAY_H_ */
