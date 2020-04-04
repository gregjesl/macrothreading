#ifndef MACROTHREADING_THREAD_H
#define MACROTHREADING_THREAD_H

#include <stdlib.h>

// Include files
#if defined MACROTHREADING_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
typedef struct macro_thread_handle_t {
    TaskHandle_t handle;
    const char *name;
    uint32_t stack_depth;
    UBaseType_t priority;
    BaseType_t core_id;
} macro_thread_handle;

typedef uint32_t stack_depth_t;
#elif defined MACROTHREADING_PTHREADS
#include <pthread.h>
#include <unistd.h>
typedef void (macrothread_fun_t)(void*);
typedef struct macro_thread_handle_t {
    pthread_t handle;
    size_t stack_depth;
    macrothread_fun_t *thread_fun;
    void* arguement;
} macro_thread_handle;

typedef size_t stack_depth_t;
#elif defined MACROTHREADING_WINDOWS
#include <windows.h>
typedef void (macrothread_fun_t)(void*);
typedef struct macro_thread_handle_t {
    HANDLE handle;
    SIZE_T stack_depth;
    DWORD thread_id;
    macrothread_fun_t* thread_fun;
    void* arguement;
} macro_thread_handle;
typedef SIZE_T stack_depth_t;
#else
typedef struct macro_thread_handle_t {
    // No elements
} macro_thread_handle;

typedef size_t stack_depth_t;
#endif

void macro_thread_set_stack_depth(macro_thread_handle *handle, stack_depth_t stack_depth);

void macro_thread_start_thread(
    macro_thread_handle *handle, 
    void (function)(void*), 
    void* arg
);

void macro_thread_delay(unsigned long int milliseconds);

void macro_thread_join(macro_thread_handle *handle);

#endif