/***********************************************************************************************************
   ___  _    _ _     _                  
  / _ \| |__(_) |_  | |_ ___ __ _ _ __  
 | (_) | '_ \ |  _| |  _/ -_) _` | '  \ 
  \___/|_.__/_|\__|  \__\___\__,_|_|_|_|

Hanoi, Creartive by UP, 20/7/2019
Xbus Rs485 
Using STM8S003F3P6 + Slock AKS
-------------------------------------------------------------------------------------------------------------------------------------------
Code by Chu Tien Thinh  
--------------------------------------------------------------------------------------------------------------------------------------------
tienthinh.obit@gmail.com
chutienthinh.bk@gmail.com
+84981762826
https://www.facebook.com/thinh.chutien                                   
https://www.facebook.com/obit.team/
https://obitvn.wordpress.com/
************************************************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "delay.h"
#include "uart.h"
#include "read_switch.h"

uint8_t data_re[32],add_send_485;
extern uint8_t AddDevice;
uint8_t alarm_on=0;

static void IWDG_Config(void)
{
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512
   */
  IWDG_SetReload(255);
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}


/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint16_t value_msb, value_lsb;
void main(void)
{
  AddDevice =0xff;
  clk_config_16MHz_hsi();
  delay_using_timer1_init();
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_WriteLow(GPIOD,GPIO_PIN_6);
  GPIO_WriteLow(GPIOD,GPIO_PIN_5);
  GPIO_WriteLow(GPIOA,GPIO_PIN_2);
  delay_ms(5000);
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_FL_IT);    // SIGNAL INPUT
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY); // INIT FOR INTERRUPT SIGN
  UART_Config();
  delay_ms(10000);
  ReadSwitchInit();
  delay_ms(10000);
  AddDevice = AddressBoard();
  if(AddDevice == 0) AddDevice =0xff;
  IWDG_Config();
  enableInterrupts();
  
  /* Infinite loop */
  
  while (1)
  {
    
    IWDG_ReloadCounter(); 
    if(alarm_on)
        {
          if(SendPackageToRS485(0x00)) alarm_on=0;
          else alarm_on=1;
        }
    GPIO_WriteReverse(GPIOA,GPIO_PIN_2);
    delay_ms(5);

    /*
    SendPackageToRS485("12345a54321", sizeof("12345a54321"), 0x76);
    delay_ms(100);
    SendPackageToRS485("right data", sizeof("right data"), 0x75);
    delay_ms(100);
    SendPackageToRS485("test1314", sizeof("test1314"), 0x79);
    delay_ms(100);
    SendPackageToRS485("35@35", sizeof("35@35"), 0x1E);
    delay_ms(100);
    SendPackageToRS485("1312@332e", sizeof("1312@332e"), 0x79);
    delay_ms(100);
    */
  }
  
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
