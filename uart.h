#ifndef UART_H_
#define UART_H_

#define	UART_CSR  0x44c00000
#define	UART_TX   0x44c00010

extern void Uart_Init(unsigned int Baud_Rate);
extern void Uart_Tx(int ch);

#endif /* UART_H_ */
