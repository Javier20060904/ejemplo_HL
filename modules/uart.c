/*******************************************************************************
* Title                 :   uart  
* Filename              :   uart.c
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "uart.h"

/******************************************************************************
* Definicion de variables
*******************************************************************************/
int uart = -1;

/******************************************************************************
* Definicion de funciones
*******************************************************************************/
ExitCode initUart(){
    Log_Debug("Abriendo UART \n");
    UART_Config uartConfig; //Estructura que carectriza al puerto UART
    UART_InitConfig(&uartConfig); //Inicializacion de la estructura
    uartConfig.baudRate = 115200; //Asignacion del baudrate
    uartConfig.flowControl = UART_FlowControl_None; //Asignacion del control de flujo
    uart = UART_Open(MT3620_RDB_HEADER2_ISU0_UART, &uartConfig); //Se abre la UART
    if (uart == -1) {
        Log_Debug("ERROR: No se puede abrir UART: %s (%d).\n", strerror(errno), errno);
        return ExitCode_Init_UartOpen;
    }
    return ExitCode_Success;
}

//Envia una cadena de texto por medio de la uart
//Funcion obtenida por medio de los ejemplos proveidos por Microsoft
void SendUartMessage(int uartFd, const char *dataToSend)
{
    size_t totalBytesSent = 0;
    size_t totalBytesToSend = strlen(dataToSend);
    int sendIterations = 0;
    while (totalBytesSent < totalBytesToSend) {
        sendIterations++;

        size_t bytesLeftToSend = totalBytesToSend - totalBytesSent;
        const char *remainingMessageToSend = dataToSend + totalBytesSent;
        ssize_t bytesSent = write(uartFd, remainingMessageToSend, bytesLeftToSend);
        if (bytesSent == -1) {
            Log_Debug("ERROR: Could not write to UART: %s (%d).\n", strerror(errno), errno);
            return;
        }

        totalBytesSent += (size_t)bytesSent;
    }

    Log_Debug("Sent %zu bytes over UART in %d calls.\n", totalBytesSent, sendIterations);
}