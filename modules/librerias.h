/*******************************************************************************
* Title                 :   librerias  
* Filename              :   librerias.h
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

#ifndef LIBRERIAS_H
#define LIBRERIAS_H

/******************************************************************************
* Includes
*******************************************************************************/

//Librerias estandar
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//API de Azure
#include "../applibs_versions.h"
#include <applibs/log.h>
#include <applibs/eventloop.h>
#include <applibs/sysevent.h>

//Definiciones de hardware
#include <hw/mt3620_rdb.h>


//Control de procesos
#include <sys/timerfd.h>
#include "../eventloop_timer_utilities.h"

/******************************************************************************
* Typedefs
*******************************************************************************/
//Codigos de error del programa
typedef enum {
    ExitCode_Success = 0,

    ExitCode_TermHandler_SigTerm = 1,

    ExitCode_ButtonTimer_Consume = 2,
    ExitCode_ButtonTimer_GetValue = 3,

    ExitCode_UpdateCallback_UnexpectedEvent = 4,
    ExitCode_UpdateCallback_GetUpdateEvent = 5,
    ExitCode_UpdateCallback_DeferEvent = 6,
    ExitCode_UpdateCallback_FinalUpdate = 7,
    ExitCode_UpdateCallback_UnexpectedStatus = 8,
    ExitCode_SetUpSysEvent_EventLoop = 9,
    ExitCode_SetUpSysEvent_RegisterEvent = 10,
    ExitCode_SetUpSysEvent_ButtonTimer = 11,

    ExitCode_Init_OpenRedLed = 12,
    ExitCode_Init_OpenGreenLed = 13,
    ExitCode_Init_OpenBlueLed = 14,
    ExitCode_Init_OpenPendingLed = 15,
    ExitCode_Init_OpenButton = 16,

    ExitCode_Main_EventLoopFail = 17,

    ExitCode_SigTerm_SetSigMaskFailure = 18,
    ExitCode_SigTerm_Timeout = 19,
    ExitCode_SigTerm_OtherFailure = 20,
    ExitCode_SigTerm_UnexpectedSignal = 21,
    
    ExitCode_Init_UartOpen = 22,

    ExitCode_Init_AdcOpen = 23,
    ExitCode_Init_GetBitCount = 24,
    ExitCode_Init_UnexpectedBitCount = 25,
    ExitCode_Init_SetRefVoltage = 26,
    ExitCode_Init_AdcPollTimer = 27,

    ExitCode_AdcTimerHandler_Consume = 28,
    ExitCode_AdcTimerHandler_Poll = 29,
} ExitCode;

static volatile sig_atomic_t exitCode = ExitCode_Success;

#endif