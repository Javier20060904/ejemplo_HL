/*******************************************************************************
* Title                 :   gpio  
* Filename              :   gpio.c
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/

#include "gpio.h"
#include "uart.h"

/******************************************************************************
* Definicion de variables
*******************************************************************************/

//Salidas
int LedRedFd = -1;
int LedGreenFd = -1;
int LedBlueFd = -1;
//Entradas
int buttonA = -1;
int buttonB = -1;

static GPIO_Value buttonState = GPIO_Value_High;
/******************************************************************************
* Definicion de funciones
*******************************************************************************/

//Cambia el color de LED 1
void change_color(GPIO_Value_Type R, GPIO_Value_Type G, GPIO_Value_Type B){
    //Value_high = Apagado
    //Value_low = Prendido
    GPIO_SetValue(LedRedFd, R);
    GPIO_SetValue(LedGreenFd, G);
    GPIO_SetValue(LedBlueFd, B);
    return;
}

ExitCode initGpio(void){
    Log_Debug("Abriendo GPIO \n");

    //Se abre el LED 1
    LedRedFd =
        GPIO_OpenAsOutput(MT3620_RDB_LED1_RED, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (LedRedFd == -1) {
        Log_Debug("ERROR: No se puede abrir MT3620_RDB_LED1_RED: %s (%d).\n", strerror(errno),
                  errno);
        return ExitCode_Init_OpenRedLed;
    }

    LedGreenFd =
        GPIO_OpenAsOutput(MT3620_RDB_LED1_GREEN, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (LedGreenFd == -1) {
        Log_Debug("ERROR: No se puede abrir MT3620_RDB_LED1_GREEN: %s (%d).\n", strerror(errno),
                  errno);
        return ExitCode_Init_OpenGreenLed;
    }

    LedBlueFd =
        GPIO_OpenAsOutput(MT3620_RDB_LED1_BLUE, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (LedBlueFd == -1) {
        Log_Debug("ERROR: No se puede abrir MT3620_RDB_LED1_BLUE: %s (%d).\n", strerror(errno),
                  errno);
        return ExitCode_Init_OpenBlueLed;
    }

    // Se abre el boton A
    buttonA = GPIO_OpenAsInput(MT3620_RDB_BUTTON_A);
    if (buttonA == -1) {
        Log_Debug("ERROR: No se puede abrir BUTTON_A: %s (%d).\n", strerror(errno), errno);
        return ExitCode_Init_OpenButton;
    }

    // Se abre el boton B
    buttonB = GPIO_OpenAsInput(MT3620_RDB_BUTTON_B);
    if (buttonB == -1) {
        Log_Debug("ERROR: No se puede abrir BUTTON_B: %s (%d).\n", strerror(errno), errno);
        return ExitCode_Init_OpenButton;
    }

    return ExitCode_Success;
}


//Lee los botones A y B, al estar presionado uno, envia un mensaje por la uart y prende el led
void ButtonTimerEventHandler(EventLoopTimer *timer)
{
    if (ConsumeEventLoopTimerEvent(timer) != 0) {
        exitCode = ExitCode_ButtonTimer_Consume;
        return;
    }

    GPIO_Value_Type A,B;

    int result_A = GPIO_GetValue(buttonA, &A);
    if (result_A != 0) {
        Log_Debug("ERROR: Could not read button GPIO: %s (%d).\n", strerror(errno), errno);
        exitCode = ExitCode_ButtonTimer_GetValue;
        return;
    }

    int result_B = GPIO_GetValue(buttonB, &B);
    if (result_B != 0) {
        Log_Debug("ERROR: Could not read button GPIO: %s (%d).\n", strerror(errno), errno);
        exitCode = ExitCode_ButtonTimer_GetValue;
        return;
    }

    change_color(GPIO_Value_High & A ,GPIO_Value_High & B, GPIO_Value_High  & A);

    //Lectura del boton A
    //Logica Pullup
    if (A != buttonState) {
        if(A == GPIO_Value_Low){
            SendUartMessage(uart, "Boton A \n");
            Log_Debug("Boton A \n");
        }
        A = buttonState;
    }

    //Lectura del boton B
    //Logica Pullup
    if (B != buttonState) {
        if(B == GPIO_Value_Low){
            SendUartMessage(uart, "Boton B \n");
            Log_Debug("Boton B \n");
        }
        B = buttonState;
    }

}