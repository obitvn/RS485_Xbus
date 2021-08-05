#ifndef __UART_H
#define __UART_H

#define RS485_UART_BUFFER_MAX_SIZE      32

typedef enum {
  WAIT_FOR_FLAG_START,
  CHECK_X,
  CHECK_B,
  CHECK_U,
  CHECK_S,
  SAVE_ADD_SEND,
  CHECK_ADD_RECEVICE,
  READ_DATA_LEN_MSB,
  READ_DATA_LEN_LSB,
  READ_DATA,
  CHECK_FLAG_STOP_CR,
  CHECK_FLAG_STOP_LF,
} status_BUS;

extern status_BUS UartState;

void UART_Config(void);
void UART_Puts(uint8_t *data, uint16_t len_data);
void UART_CallBack();
void UART_PutChar(uint8_t c);
//void UartProcessInMainLoop(uint8_t* data, uint16_t len_data, uint8_t add_send);

uint8_t SendPackageToRS485(uint8_t add_recevice_pack);

#endif