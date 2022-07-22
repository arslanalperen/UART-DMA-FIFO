#include "fifo.h"

uint8_t DMA_WRITE_BUF[DATA_SIZE]; //Buffer is filling by FIFO to write data to DMA
unsigned short fifoInit = 0; //Checking is fifo initialized
unsigned short STATUS_DMA;

//Extern this object
//Extern FIFO fifo;
//Use this object in fifo functions
FIFO fifo;

//FIFO variables initialized
//User does not need to use this function
void fifo_init(){
     fifo.head = 0;
     fifo.tail = 0;
     fifo.size = DATA_SIZE;
     fifo.buf[DATA_SIZE] = 0;
     fifo.FIFOindex = 0;
     fifo.UARTindex = 0;
}

//Checking is FIFO initialized at fifo_read, fifo_write and UART_fifo_write functions
//User does not need to use this function
void fifo_init_control(){
	if(fifoInit == 0){
		fifo_init();
		fifoInit = 1;
	}
}

//Read FIFO data and write to another buffer
//User does not need to use this function if just want to transmit data
unsigned short fifo_read(uint8_t * buf){
	unsigned short i;
    unsigned short bufIndex = 0;
    fifo_init_control();
    for(i = fifo.UARTindex*DATA_SIZE; i < (fifo.UARTindex+1)*DATA_SIZE; i++){
    	if( fifo.tail != fifo.head ){
    		buf[bufIndex] = fifo.buf[fifo.tail];
            fifo.tail++;
            bufIndex++;
            if( fifo.tail == DATA_SIZE*BUF_SIZE )
            	fifo.tail = 0;
        }
    	else
    		return i;
     }

	if(fifo.UARTindex + 1 == BUF_SIZE)
		fifo.UARTindex = 0;
//	else
//		fifo.UARTindex++;
	return DATA_SIZE;
}

//Write bytes to FIFO from given buffer
unsigned short fifo_write(uint8_t * buf){
	unsigned short i;
	unsigned short bufIndex = 0;
    fifo_init_control();
	for(i = fifo.FIFOindex*DATA_SIZE; i < (fifo.FIFOindex+1)*DATA_SIZE; i++){
		if( (fifo.head + 1 == fifo.tail) || ( (fifo.head + 1 == (fifo.size)*fifo.FIFOindex) && (fifo.tail == 0) ))
			return i;
		else {
			fifo.buf[fifo.head] = buf[bufIndex];
			fifo.head++;
			bufIndex++;
			if( fifo.head == DATA_SIZE*BUF_SIZE )
				fifo.head = 0;
		}
	}
	if(fifo.FIFOindex + 1 == BUF_SIZE)
		fifo.FIFOindex = 0;
	else
		fifo.FIFOindex++;
	return DATA_SIZE;
}

//Write FIFO data to UART with DMA
unsigned short UART_fifo_write(UART_HandleTypeDef *huart){
	unsigned short STATUS_DMA = 0x00U;
	fifo_init_control();
	if((fifo.UARTindex != 0 || fifo.UARTindex != fifo.FIFOindex) && (fifo.head != fifo.tail)){
		if(STATUS_DMA == 0x02U){
			STATUS_DMA = HAL_UART_Transmit_DMA(huart,DMA_WRITE_BUF,DATA_SIZE);
		}
		else if(STATUS_DMA == 0x00U){
			fifo_read(DMA_WRITE_BUF);
			STATUS_DMA = HAL_UART_Transmit_DMA(huart,DMA_WRITE_BUF,DATA_SIZE);
		}
	}
	return STATUS_DMA;
}
