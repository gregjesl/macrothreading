#ifndef MACROTHREADING_CONDITION_H
#define MACROTHREADING_CONDITION_H

#if defined ESP_PLATFORM
#define MACROTHREADING_ESP32
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#if defined MACROTHREADING_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#define MACROTHREADING_EVENT_MASK 1 << 0
typedef EventGroupHandle_t macrothread_condition_t;
#elif defined MACROTHREADING_PTHREADS
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
typedef struct macrothread_condition_struct
{
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
    bool signaled;
} macrothread_condition_struct_t;
typedef macrothread_condition_struct_t* macrothread_condition_t;
#elif defined MACROTHREADING_WINDOWS
#include <windows.h>
#include <stdbool.h>
typedef struct macrothread_condition_struct
{
    CRITICAL_SECTION *mutex;
    CONDITION_VARIABLE *cond;
    bool signaled;
} macrothread_condition_struct_t;
typedef macrothread_condition_struct_t* macrothread_condition_t;
#else
#include <stdbool.h>
typedef bool macrothread_condition_t;
#endif

macrothread_condition_t macrothread_condition_init();
void macrothread_condition_wait(macrothread_condition_t cond);
void macrothread_condition_signal(macrothread_condition_t cond);
void macrothread_condition_destroy(macrothread_condition_t cond);

#ifdef __cplusplus
}
#endif

#endif