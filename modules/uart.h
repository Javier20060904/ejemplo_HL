/*******************************************************************************
* Title                 :   uart  
* Filename              :   uart.h
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

#ifndef UART_H
#define UART_H

/******************************************************************************
* Includes
*******************************************************************************/

#include "librerias.h"
#include <applibs/uart.h>

/******************************************************************************
* Declaracion de variables
*******************************************************************************/

extern int uart; //Variable extern para guardar la descricion del puerto

/******************************************************************************
* Prototipo de funciones
*******************************************************************************/
void SendUartMessage(int uartFd, const char *dataToSend);
ExitCode initUart(void);

#endif