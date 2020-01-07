#include "uart.h"

//#define MCK_FREQ				(41625*1000)
#define MCK_FREQ				(37125*1000)

void Uart_Init(unsigned int Baud_Rate)
{
	volatile unsigned int *pUart = (unsigned int *)(UART_CSR);
	*pUart =  (unsigned int)(((MCK_FREQ / (Baud_Rate << 4)) - 1) << 20) | 0x00002040u;
}

void Uart_Tx(int ch)
{
	volatile unsigned int *tx = (unsigned int *)(UART_TX);
	volatile unsigned int *csr = (unsigned int *)(UART_CSR);

	while(*csr & 0x20);
	*tx = ((unsigned int)ch) & 0x000000ffu;
}
