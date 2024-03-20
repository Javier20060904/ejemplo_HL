/*******************************************************************************
* Title                 :   handlers  
* Filename              :   handlers.h
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/

#ifndef HANDLERS_H
#define HANDLERS_H

#include "librerias.h"

extern EventLoop *eventLoop;
extern EventLoopTimer *buttonPollTimer;
extern EventLoopTimer *adcPollTimer;
extern EventRegistration *updateEventReg;


ExitCode InitPeripheralsAndHandlers(void);
ExitCode SetUpSysEventHandler(void);

void TerminationHandler(int signalNumber);
void FreeSysEventHandler(void);

void CloseFdAndPrintError(int fd, const char *fdName);
void ClosePeripheralsAndHandlers(void);
ExitCode WaitForSigTerm(time_t);

#endif