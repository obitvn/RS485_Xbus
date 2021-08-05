#include "stm8s.h"
#include "uart.h"
#include "delay.h"



status_BUS UartState;

uint8_t is_device, data_correct;

uint32_t timeout_uart, time_last_uart;
uint8_t UartData, AddSendPack;
uint8_t AddDevice;
uint16_t DataLength;
uint8_t DataBuff[RS485_UART_BUFFER_MAX_SIZE], data_index;

void UART_CallBack()
{
  UartData=0;
  UartData = UART1_ReceiveData8();
  //timeout_uart = GetMicroTick() - time_last_uart;
  switch (UartState)
  {

    
    case WAIT_FOR_FLAG_START:
      if((UartData=='X')) UartState = CHECK_B;
      else UartState = WAIT_FOR_FLAG_START;
      break;
      
    case CHECK_B:
      if((UartData=='B')) UartState = CHECK_U;
      else UartState = WAIT_FOR_FLAG_START;
      break;
      
    case CHECK_U:
      if((UartData=='U')) UartState = CHECK_S;
      else UartState = WAIT_FOR_FLAG_START;
      break;
      
    case CHECK_S:
      if((UartData=='S')) UartState = CHECK_ADD_RECEVICE;
      else UartState = WAIT_FOR_FLAG_START;
      break;;
      
    case CHECK_ADD_RECEVICE:
      if(AddDevice == UartData) is_device=1;
      else is_device=0;
      UartState = SAVE_ADD_SEND;
      break;
      
    case SAVE_ADD_SEND:
      AddSendPack = UartData;
      UartState = READ_DATA_LEN_MSB;
      break;
      
   case READ_DATA_LEN_MSB:
      DataLength = UartData;
      UartState = READ_DATA_LEN_LSB;
      break;
      
   case READ_DATA_LEN_LSB:
      DataLength = (DataLength<<8)|UartData;
      UartState = READ_DATA;
      data_index=0;
      break;
      
   case READ_DATA:
      if((data_index < DataLength)&&(DataLength < RS485_UART_BUFFER_MAX_SIZE))
      {
        DataBuff[data_index++] = UartData;
        
        if(data_index == DataLength) UartState = CHECK_FLAG_STOP_CR;
        else UartState = READ_DATA;
      }
      else UartState = WAIT_FOR_FLAG_START;
      break;
      
   case CHECK_FLAG_STOP_CR:
      if(UartData == 13) // \r
      {
        UartState = CHECK_FLAG_STOP_LF;
      }
      else UartState = WAIT_FOR_FLAG_START;
      break;
      
   case CHECK_FLAG_STOP_LF:
      if(UartData == 10) // \n
      {
        data_correct=1;
      }
      else data_correct=0;
      UartState = WAIT_FOR_FLAG_START;
      break;
  }
}

//void UartProcessInMainLoop(uint8_t* data, uint16_t len_data, uint8_t add_send)
//{
  /*
   switch (UartState)
   {
     case WAIT_TIME_FOR_STOP:
        if((GetMicroTick() - time_last_uart)>100) 
        {
          UartState = DATA_CORRECT;
        }
        else UartState = WAIT_FOR_FLAG_START;
        break;
     case DATA_CORRECT:
        for(int i=0; i<len_data; i++)
        {
          data[i] = DataBuff[i];
        }
        UartState = ENABLE_SEND_DATA;
        break;
   }
*/
//}

void UART_Config(void)
{
  
    UART1_DeInit();
  
    UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
    UART1_Cmd(ENABLE);
    
  /*
    UART1->CR1 |= (uint8_t)UART1_WORDLENGTH_8D;
    UART1->CR3 |= (uint8_t)UART1_STOPBITS_1;
    UART1->CR1 |= (uint8_t)UART1_PARITY_NO;
    UART1->BRR2 |= 0x0a;
    UART1->BRR1 |= 0x08;
    UART1->CR2 |= (uint8_t)UART1_CR2_TEN; 
    UART1->CR2 |= (uint8_t)UART1_CR2_REN;  
    UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN); 
    */
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
    UART1_Cmd(ENABLE);
    
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_WriteLow(GPIOD,GPIO_PIN_4);
    UartState = WAIT_FOR_FLAG_START;
    time_last_uart = GetMicroTick();

}

void UART_PutChar(uint8_t c)
{
  
  UART1_SendData8(c);
  while(UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);
  
}


void UART_Puts(uint8_t *data, uint16_t len_data)
{
 for(int i=0; i<len_data; i++)
  {
    UART_PutChar(*data);
    data++;
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  }
}

void AddDeviceToText(uint8_t add)
{
  uint8_t add_lsb, add_msb;
  add_msb = add >> 4;
  if(add_msb < 10) UART_PutChar(add_msb +48);
    else UART_PutChar(add_msb + 55); 
  add_lsb =add<<4;
  add_lsb = add_lsb >> 4;  
  if(add_lsb < 10) UART_PutChar(add_lsb +48);
    else UART_PutChar(add_lsb + 55); 
}

uint8_t SendPackageToRS485(uint8_t add_recevice_pack)
{
  if( UartState == WAIT_FOR_FLAG_START)
  {
    GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    delay_ms(8);
    UART_Puts("XBUS",4);
    UART_PutChar(add_recevice_pack);
    UART_PutChar(AddDevice);
    UART_PutChar(0x00);
    UART_PutChar(0x0A);
    UART_Puts("\r\nID:",5); 
    AddDeviceToText(AddDevice);
    UART_PutChar('@');
    AddDeviceToText(AddDevice);
    UART_PutChar(13);
    UART_PutChar(10);
    delay_ms(8);
    GPIO_WriteLow(GPIOD,GPIO_PIN_4);
    UartState = WAIT_FOR_FLAG_START;
    return 1;
  }
  else return 0;
}
