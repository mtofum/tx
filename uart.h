#ifndef _USART_H_
#define _USART_H_
#define u8 unsigned char

#define UART_DATA_LEN 8
struct Received_Data_struct
{
  u8 head[3];
  u8 cmd;
  u8 data[UART_DATA_LEN];
  u8 check;
  u8 data_ready;
};

//public function
void UART_Init(void);
//public variable
extern struct Received_Data_struct uart_data;
#endif
