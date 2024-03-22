/*
 * uart.c
 *
 *  Created on: Mar 11, 2024
 *      Author: kccistc
 */

#include "uart.h"
#include <stdio.h>

UART_HandleTypeDef *myHuart;

#define rxBufferMax 255

int rxBufferGp;			// get pointer (read)
int rxBufferPp;			// put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar;

// init device
void initUart(UART_HandleTypeDef *inHuart) {
	myHuart = inHuart;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);	//	문자 하나 수신할 때마다 HAL_UART_RxCpltCallback호출
}

// process received charactor
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	rxBuffer[rxBufferPp++] = rxChar;						//buffer가 꽉 찰 때까지 반복 됨
	rxBufferPp %= rxBufferMax;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);
}

// get charactor from buffer
uint8_t getChar() {
	uint8_t result;
	if (rxBufferGp == rxBufferPp)	return 0;	// Gp == Pp : 버퍼가 비어있다는 뜻
	result = rxBuffer[rxBufferGp++];				//
	rxBufferGp %= rxBufferMax;
	return result;
}

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(myHuart, p, len, 10); // lcd, uart, ...
	return len;
}
