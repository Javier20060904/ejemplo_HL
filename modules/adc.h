/*******************************************************************************
* Title                 :   adc  
* Filename              :   adc.h
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

#ifndef ADC_H
#define ADC_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "librerias.h"
#include <applibs/adc.h>

/******************************************************************************
* Variables
*******************************************************************************/
//Variable estatica para guardar la descricion del puerto
extern int adcControllerFd; 

//Numero de samples (Se asigna en su inicializacion)
extern int sampleBitCount; 

//Voltaje de referencia
extern float sampleMaxVoltage; 

/******************************************************************************
* Prototipo de funciones
*******************************************************************************/
ExitCode initAdc(void);
void AdcPollingEventHandler(EventLoopTimer *timer);

#endif