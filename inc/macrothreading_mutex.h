#ifndef MACROTHREADING_MUTEX_H
#define MACROTHREADING_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#if defined MACROTHREADING_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
typedef SemaphoreHandle_t macrothread_mutex_t;
#elif defined MACROTHREADING_PTHREADS
#include <pthread.h>
#include <string.h>
typedef pthread_mutex_t* macrothread_mutex_t;
#elif defined MACROTHREADING_WINDOWS
#include <windows.h>
#include <string.h>
typedef HANDLE* macrothread_mutex_t;
#else
#include <stdbool.h>
typedef bool macrothread_mutex_t;
#endif

macrothread_mutex_t macrothread_mutex_init();
void macrothread_mutex_lock(macrothread_mutex_t mutex);
void macrothread_mutex_unlock(macrothread_mutex_t mutex);
void macrothread_mutex_destroy(macrothread_mutex_t mutex);

#ifdef __cplusplus
}
#endif

#endif