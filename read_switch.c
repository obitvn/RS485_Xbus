#include "stm8s.h"
#include "read_switch.h"
#include "stm8s_adc1.h"

typedef struct
{
  uint16_t ValueReadFromADC;
  uint8_t HexRaw;

}decdoder_value_type;

 decdoder_value_type encoder_table [16] ={
 {0,0x00},
 {510,0x01},
 {363,0x02},
 {620,0x03},
 {225,0x04},
 {574,0x05},
 {465,0x06},
 {661,0x07},
 {130,0x08},
 {546,0x09},
 {420,0x0A},
 {643,0x0B},
 {307,0x0C},
 {602,0x0D},
 {495,0x0E},
 {680,0x0F},
};



void ReadSwitchInit()
{
  ADC1_DeInit();
  ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D4); //Fcpu/4=D4 
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3|ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
  //ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS,((ADC1_Channel_TypeDef)(ADC1_CHANNEL_3 | ADC1_CHANNEL_4)),ADC1_ALIGN_RIGHT);
  ADC1_DataBufferCmd(ENABLE);
  ADC1_Cmd(ENABLE);
  ADC1_ScanModeCmd (ENABLE);
  
    
//  ADC1->CR1 |= (uint8_t)(ADC1_PRESSEL_FCPU_D4);
//  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_ALIGN);
//  ADC1->CR2 |= (uint8_t)(ADC1_ALIGN_RIGHT);
//    /* Set the continuous conversion mode */
//  ADC1->CR1 |= ADC1_CR1_CONT;
//  /* Clear the ADC1 channels */
//  ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);
//  /* Select the ADC1 channel */
//  ADC1->CSR |= (uint8_t)((ADC1_Channel_TypeDef)(ADC1_CHANNEL_3 | ADC1_CHANNEL_4));
//  ADC1->CR3 |= ADC1_CR3_DBUF;
//  ADC1->CR2 |= ADC1_CR2_SCAN;
  
}

uint8_t GetLocationMinValue(uint16_t adc_value)
{
  uint16_t min_value = adc_value;
  uint8_t location_value_in_table=0;
  uint16_t scratch_paper;
  for(int i=0; i<16; i++)
  {
    scratch_paper = abs(adc_value - encoder_table[i].ValueReadFromADC);
    if(min_value > scratch_paper) 
      {
       min_value = scratch_paper;
       location_value_in_table = i;
      }
  }
  return location_value_in_table;
}


uint8_t GetAddressBinaryFromADCValue(uint16_t adc_value)
{
  uint8_t location_value_in_table;
  location_value_in_table = GetLocationMinValue(adc_value);
  return encoder_table[location_value_in_table].HexRaw;
}



uint8_t AddressBoard()  
{
  uint8_t msb,lsb, get;
  uint16_t value_msb, value_lsb;
  
  ADC1_StartConversion();
  while ( ADC1_GetFlagStatus (ADC1_FLAG_EOC) == 0);
  ADC1_ClearITPendingBit (ADC1_IT_EOC); 
  ADC1_ClearFlag (ADC1_FLAG_EOC);

  value_msb = ADC1_GetBufferValue (ADC1_CHANNEL_4);
  value_lsb = ADC1_GetBufferValue (ADC1_CHANNEL_3);
  msb = GetAddressBinaryFromADCValue(value_msb);
  lsb = GetAddressBinaryFromADCValue(value_lsb);
  nop();
  msb = msb<<4;
  msb= msb&0xf0;
  get= msb|lsb;
  return get;
}
