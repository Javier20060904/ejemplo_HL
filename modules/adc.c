/*******************************************************************************
* Title                 :   adc  
* Filename              :   adc.c
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/

#include "adc.h"
#include "uart.h"

/******************************************************************************
* Definicion de variables
*******************************************************************************/

float sampleMaxVoltage = 2.5f; 
int sampleBitCount = -1; 
int adcControllerFd = -1; 

/******************************************************************************
* Definicion de funciones
*******************************************************************************/

ExitCode initAdc(){
    Log_Debug("Abriendo ADC \n");

    //Abriendo puerto ADC
    adcControllerFd = ADC_Open(MT3620_RDB_ADC_CONTROLLER0);
    if (adcControllerFd == -1) {
        Log_Debug("ADC_Open failed with error: %s (%d)\n", strerror(errno), errno);
        return ExitCode_Init_AdcOpen;
    }

    //Tomando el numero de bits para el sample
    sampleBitCount = ADC_GetSampleBitCount(adcControllerFd, MT3620_RDB_ADC_CONTROLLER0);
    if (sampleBitCount == -1) {
        Log_Debug("ADC_GetSampleBitCount failed with error : %s (%d)\n", strerror(errno), errno);
        return ExitCode_Init_GetBitCount;
    }
    if (sampleBitCount == 0) {
        Log_Debug("ADC_GetSampleBitCount returned sample size of 0 bits.\n");
        return ExitCode_Init_UnexpectedBitCount;
    }

    //Asignando el voltaje de referencia
    int result = ADC_SetReferenceVoltage(adcControllerFd, MT3620_RDB_ADC_CONTROLLER0,
                                         sampleMaxVoltage);
    if (result == -1) {
        Log_Debug("ADC_SetReferenceVoltage failed with error : %s (%d)\n", strerror(errno), errno);
        return ExitCode_Init_SetRefVoltage;
    }
    
    return ExitCode_Success;
}

void AdcPollingEventHandler(EventLoopTimer *timer)
{
    if (ConsumeEventLoopTimerEvent(timer) != 0) {
        exitCode = ExitCode_AdcTimerHandler_Consume;
        return;
    }

    //Lectura del ADC
    uint32_t value;
    int result = ADC_Poll(adcControllerFd, MT3620_RDB_ADC_CONTROLLER0, &value);
    if (result == -1) {
        Log_Debug("ADC_Poll failed with error: %s (%d)\n", strerror(errno), errno);
        exitCode = ExitCode_AdcTimerHandler_Poll;
        return;
    }

    float voltage = ((float)value * sampleMaxVoltage) / (float)((1 << sampleBitCount) - 1);
    Log_Debug("El valor del voltaje es %.3f V\n", voltage);
}