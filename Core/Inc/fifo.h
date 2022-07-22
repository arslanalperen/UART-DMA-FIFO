#ifndef FIFO_H_
#define FIFO_H_

#include "main.h"

#define BUF_SIZE 10 //FIFO Number
#define DATA_SIZE 32 //Data Size

//FIFO Struct
typedef struct {
     uint8_t buf[DATA_SIZE*BUF_SIZE]; //FIFO Buffer
     unsigned short head; //FIFO Write Index
     unsigned short tail; //FIFO Read Index
     unsigned short size; //FIFO Size (Byte)
     unsigned short FIFOindex; //FIFO Buffer's Write Index
     unsigned short UARTindex; //FIFO Buffer's Read  Index
}FIFO;

void fifo_init();
void fifo_init_control();
unsigned short fifo_read(uint8_t * buf);
unsigned short fifo_write(uint8_t * buf);
unsigned short UART_fifo_write(UART_HandleTypeDef *huart);

#endif //FIFO_H_
