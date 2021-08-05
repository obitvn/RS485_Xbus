#include "stm8s.h"
#include "delay.h"
#include "uart.h"




void clk_config_16MHz_hsi(void)
{       
        /*
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); 
        CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
        CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
        while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)!=SET);
          */
        CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void delay_using_timer1_init(void)
{
       /*
        TIM1_DeInit();
	TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP,59999, 1);
        TIM1_ARRPreloadConfig(ENABLE);
	TIM1_ITConfig(TIM1_IT_UPDATE,DISABLE);
        TIM1_ClearFlag(TIM1_FLAG_UPDATE);
        TIM1_SetCounter(0);
	TIM1_Cmd(ENABLE);  
        enableInterrupts();
        */
}

void delay_isr(void)
{         
          nop();
          //time_keeper = time_keeper + 60000;	
	  //TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}

uint32_t GetMicroTick()
{
	//return time_keeper + TIM1_GetCounter();
          return 1;
}

void delay_us(uint32_t time)
{
  /*
	uint32_t time_start;
	time_start=GetMicroTick();
	if(4294967295UL-time_start>time)
	{
	  while(GetMicroTick()-time_start<time){};
	}
	else
	{
          //uint32_t time_rest = time + time_start - 4294967295UL;
          uint32_t time_rest = 4294967295UL - time_start;
          time_rest = time - time_rest ;
          while(time_start<4294967295UL){};
          while(GetMicroTick()<time_rest){};
	}
*/
}

void delay_ms(uint32_t time)
{
  for(int i=0; i<1000; i++)
  {
    //delay_us(time);
    for(int a=0; a<50; a++)
    {
       nop();
        nop();
         nop();
          nop();
           nop();
            nop(); 
    }
  }
}