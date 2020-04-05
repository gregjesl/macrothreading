#ifndef MACROTHREADING_THREAD_H
#define MACROTHREADING_THREAD_H

#include <stdlib.h>
#include <string.h>

// Include files
#if defined MACROTHREADING_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
typedef struct macrothread_handle_struct {
    TaskHandle_t handle;
    const char *name;
    uint32_t stack_depth;
    UBaseType_t priority;
    BaseType_t core_id;
} macrothread_handle_struct_t;

typedef uint32_t stack_depth_t;
#elif defined MACROTHREADING_PTHREADS
#include <pthread.h>
#include <unistd.h>
typedef void (macrothread_fun_t)(void*);
typedef struct macrothread_handle_struct {
    pthread_t handle;
    size_t stack_depth;
    macrothread_fun_t *thread_fun;
    void* arguement;
} macrothread_handle_struct_t;

typedef size_t stack_depth_t;
#elif defined MACROTHREADING_WINDOWS
#include <windows.h>
typedef void (macrothread_fun_t)(void*);
typedef struct macrothread_handle_struct {
    HANDLE handle;
    SIZE_T stack_depth;
    DWORD thread_id;
    macrothread_fun_t* thread_fun;
    void* arguement;
} macrothread_handle_struct_t;
typedef SIZE_T stack_depth_t;
#else
typedef struct macrothread_handle_struct {
    // No elements
} macrothread_handle_struct_t;

typedef size_t stack_depth_t;
#endif

typedef macrothread_handle_struct_t* macrothread_handle_t;

macrothread_handle_t macrothread_handle_init();

void macrothread_set_stack_depth(macrothread_handle_t handle, stack_depth_t stack_depth);
void macrothread_set_name(macrothread_handle_t handle, const char *name);
void macrothread_set_priority(macrothread_handle_t handle, unsigned int priority);
void macrothread_set_core(macrothread_handle_t handle, int core);

void macrothread_start_thread(
    macrothread_handle_t handle, 
    void (function)(void*), 
    void* arg
);

void macrothread_delay(unsigned long int milliseconds);

void macrothread_join(macrothread_handle_t handle);

#endif