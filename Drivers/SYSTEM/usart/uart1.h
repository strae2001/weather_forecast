#ifndef __UART1_H
#define __UART1_H

#include "sys.h"
#include <stdio.h>
#include "string.h"

#define TX1 GPIO_PIN_9
#define RX1 GPIO_PIN_10

#define TX1BUF_SIZE	128
#define RX1BUF_SIZE	128

//	接收数据标志
#define UART1_RECV_NONE 	0
#define UART1_RECV_OK   	1
#define UART1_RECV_ERROR 	4

void UART1_Init(void);

void UART1_sendString(char* str);

void UART1_test_recv(void);

#endif
