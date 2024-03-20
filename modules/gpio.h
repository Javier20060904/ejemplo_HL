/*******************************************************************************
* Title                 :   gpio
* Filename              :   gpio.h
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

#ifndef GPIO_H
#define GPIO_H

/******************************************************************************
* Includes
*******************************************************************************/

#include "librerias.h"
#include <applibs/gpio.h>

/******************************************************************************
* Variables
*******************************************************************************/

//Salidas
extern int LedRedFd;
extern int LedGreenFd;
extern int LedBlueFd;
//Entradas
extern int buttonA;
extern int buttonB;

/******************************************************************************
* Prototipo de funciones
*******************************************************************************/

ExitCode initGpio(void);
void change_color(GPIO_Value_Type R, GPIO_Value_Type G, GPIO_Value_Type B);
void ButtonTimerEventHandler(EventLoopTimer *timer);
void change_color(GPIO_Value_Type R, GPIO_Value_Type G, GPIO_Value_Type B);

#endif