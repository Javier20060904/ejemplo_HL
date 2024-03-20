/*******************************************************************************
* Title                 :   main  
* Filename              :   main.c
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
* Description           : Este programa consiste en el uso de ADC, GPIO y UART 
* usando las API's de Azure Sphere en alto nivel.
* Codigo hecho en base a los ejemplos proveidos por Microsoft:
* https://github.com/Azure/azure-sphere-samples
******************************************************************************/

#include "modules/gpio.h"
#include "modules/uart.h"
#include "modules/adc.h"
#include "modules/handlers.h"

//Funcion main
int main(void)
{
    Log_Debug("INFO: Iniciando Aplicacion\n");
    Log_Debug("INFO: Presiona BUTTON_A o BUTTON_B.\n");

    //Inicializacion de perifericos
    exitCode = InitPeripheralsAndHandlers(); 

    //Loop
    while (exitCode == ExitCode_Success) {
        EventLoop_Run_Result result = EventLoop_Run(eventLoop, -1, true);
        // Continue if interrupted by signal, e.g. due to breakpoint being set.
        if (result == EventLoop_Run_Failed && errno != EINTR) {
            exitCode = ExitCode_Main_EventLoopFail;
        }
    }
    
    //Cierre de perifericos
    ClosePeripheralsAndHandlers();

    if (exitCode == ExitCode_UpdateCallback_FinalUpdate) {
        Log_Debug("INFO: Waiting for SIGTERM\n");
        // SIGTERM should arrive in 10 seconds; allow 20 to be sure.
        exitCode = WaitForSigTerm(20);
    }

    Log_Debug("INFO: Application exiting\n");
    return exitCode;
}