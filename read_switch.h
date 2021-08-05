#ifndef __READ_SWITCH_H
#define __READ_SWITCH_H

#define VREF 3300






void ReadSwitchInit();
uint8_t GetLocationMinValue(uint16_t adc_value);
uint8_t GetAddressBinaryFromADCValue(uint16_t adc_value);
uint8_t AddressBoard();
#endif