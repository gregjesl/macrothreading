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
#define MACRO_THREAD_FUNCTION_RETURN void
#define MACRO_THREAD_FUNCTION_ARGUEMENT void*
#elif defined MACROTHREADING_PTHREADS
#include <pthread.h>
typedef struct macro_thread_handle_t {
    pthread_t handle;
    size_t stack_depth;
} macro_thread_handle;

typedef size_t stack_depth_t;
typedef void* (macro_thread_function)(void*);
#define MACRO_THREAD_FUNCTION_RETURN void*
#define MACRO_THREAD_FUNCTION_ARGUEMENT void*
#elif defined MACTROTHREADING_WINDOWS
#include <windows.h>
typedef struct macro_thread_handle_t {
    HANDLE handle;
    SIZE_T stack_depth;
    DWORD thread_id;
} macro_thread_handle;
typedef void (macro_thread_function)(void*);
typedef SIZE_T stack_depth_t;
#define MACRO_THREAD_FUNCTION_RETURN DWORD WINAPI
#define MACRO_THREAD_FUNCTION_ARGUEMENT LPVOID
#else
typedef struct macro_thread_handle_t {
    // No elements
} macro_thread_handle;

typedef size_t stack_depth_t;
typedef void (macro_thread_function)(void*);
#define MACRO_THREAD_FUNCTION_RETURN void
#define MACRO_THREAD_FUNCTION_ARGUEMENT void*
#endif

void macro_thread_set_stack_depth(macro_thread_handle *handle, stack_depth_t stack_depth);

void macro_thread_start_thread(
    macro_thread_handle *handle, 
    MACRO_THREAD_FUNCTION_RETURN (function)(MACRO_THREAD_FUNCTION_ARGUEMENT), 
    MACRO_THREAD_FUNCTION_ARGUEMENT arg
);

void macro_thread_join(macro_thread_handle *handle);

#endif