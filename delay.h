#ifndef __DELAY_H
#define __DELAY_H

static uint32_t time_keeper=0;

void clk_config_16MHz_hsi(void);
void delay_using_timer1_init(void);
void delay_isr(void);
void delay_ms(uint32_t time);
void delay_us(uint32_t time);
void AddToText(uint8_t add);
void AddDeviceToText(uint8_t add);
uint32_t GetMicroTick();
#endif