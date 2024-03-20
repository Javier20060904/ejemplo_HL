/*******************************************************************************
* Title                 :   handlers  
* Filename              :   handlers.c
* Author                :   Javier Perez Macias
* Origin Date           :   19/01/2024
* Version               :   1.0.0
* Compiler              :   CMAKE
* Target                :   Azure Sphere MT3620
******************************************************************************/


/******************************************************************************
* Includes
*******************************************************************************/

#include "handlers.h"
#include "gpio.h"
#include "adc.h"
#include "uart.h"

/******************************************************************************
* Definicion de variables
*******************************************************************************/

EventLoop *eventLoop = NULL;
EventLoopTimer *buttonPollTimer = NULL;
EventLoopTimer *adcPollTimer = NULL;
EventRegistration *updateEventReg = NULL;

/******************************************************************************
* Definicion de funciones
*******************************************************************************/

//Inicializacion de los perifericos
ExitCode InitPeripheralsAndHandlers(void)
{
    struct sigaction action = {.sa_handler = TerminationHandler};
    sigaction(SIGTERM, &action, NULL);

    ExitCode i_gpio = initGpio();
    if (i_gpio != ExitCode_Success)
        return i_gpio;

    ExitCode i_uart = initUart(); 
    if (i_uart != ExitCode_Success)
        return i_uart;

    ExitCode i_adc = initAdc();
    if (i_adc != ExitCode_Success)
        return i_adc;
    
    /*Iniciacion del eventloop*/
    ExitCode localExitCode = SetUpSysEventHandler();

    return localExitCode;
}

//Inicializacion del ciclo
ExitCode SetUpSysEventHandler(void)
{
    //Creacion del eventloop
    eventLoop = EventLoop_Create();
    if (eventLoop == NULL) {
        Log_Debug("ERROR: could not create event loop\n");
        return ExitCode_SetUpSysEvent_EventLoop;
    }

    /*Tiempo que se le asignara a la lectura de botones*/
    static const struct timespec buttonPollInterval100Ms = {.tv_sec = 0,
                                                            .tv_nsec = 100 * 1000 * 1000};
    /*Creacion del proceso para los botones*/
    buttonPollTimer =
        CreateEventLoopPeriodicTimer(eventLoop, &ButtonTimerEventHandler, &buttonPollInterval100Ms);
    if (buttonPollTimer == NULL) {
        return ExitCode_SetUpSysEvent_ButtonTimer;
    }

    /*Tiempo que se le asignara a la lectura del ADC*/
    struct timespec adcCheckPeriod = {.tv_sec = 1, .tv_nsec = 0};
    /*Creacion del proceso para el ADC*/
    adcPollTimer = CreateEventLoopPeriodicTimer(eventLoop, &AdcPollingEventHandler, &adcCheckPeriod);
    if (adcPollTimer == NULL) {
        return ExitCode_Init_AdcPollTimer;
    }

    return ExitCode_Success;
}

//Manejo de la señal de finalización
void TerminationHandler(int signalNumber)
{
    exitCode = ExitCode_TermHandler_SigTerm;
}

//Libera los recursos 
void FreeSysEventHandler(void)
{
    DisposeEventLoopTimer(buttonPollTimer);
    SysEvent_UnregisterForEventNotifications(updateEventReg);
    EventLoop_Close(eventLoop);
}


//Imprime el error
void CloseFdAndPrintError(int fd, const char *fdName)
{
    if (fd >= 0) {
        int result = close(fd);
        if (result != 0) {
            Log_Debug("ERROR: Could not close fd %s: %s (%d).\n", fdName, strerror(errno), errno);
        }
    }
}

//Cierra los perifericos y termina el loop
void ClosePeripheralsAndHandlers(void)
{
    FreeSysEventHandler();

    CloseFdAndPrintError(buttonA, "buttonA");
    CloseFdAndPrintError(buttonB, "buttonA");

    CloseFdAndPrintError(LedRedFd, "LedRedFd");
    CloseFdAndPrintError(LedGreenFd, "LedGreenFd");
    CloseFdAndPrintError(LedBlueFd, "LedBlueFd");
}

//Espera por la señal de finalización
ExitCode WaitForSigTerm(time_t timeoutSecs)
{
    sigset_t sigtermSet, oldMask;

    sigemptyset(&sigtermSet);
    sigaddset(&sigtermSet, SIGTERM);

    // Block SIGTERM - disables the existing SIGTERM handler
    if (sigprocmask(SIG_BLOCK, &sigtermSet, &oldMask) == -1) {
        Log_Debug("ERROR: Could not set process signal mask: %d (%s)", errno, strerror(errno));
        return ExitCode_SigTerm_SetSigMaskFailure;
    }

    struct timespec timeout = {.tv_sec = timeoutSecs, .tv_nsec = 0};

    int result = sigtimedwait(&sigtermSet, NULL, &timeout);

    switch (result) {
    case SIGTERM:
        Log_Debug("INFO: SIGTERM received; exiting.\n");
        return ExitCode_Success;
    case -1:
        if (errno == EAGAIN) {
            Log_Debug("ERROR: Timed out waiting for SIGTERM\n");
            return ExitCode_SigTerm_Timeout;
        } else {
            Log_Debug("ERROR: Waiting for SIGTERM: %d (%s)\n", errno, strerror(errno));
            return ExitCode_SigTerm_OtherFailure;
        }
    default:
        Log_Debug("WARNING: Unexpected signal received when waiting for SIGTERM: %d\n", result);
        return ExitCode_SigTerm_UnexpectedSignal;
    }
}