#include "macrothreading_thread.h"

#ifdef MACROTHREADING_PTHREADS
void* macrothread_pthread_wrapper(void* arg)
{
    macro_thread_handle *parent = (macro_thread_handle*)arg;
    parent->thread_fun(parent->arguement);
    return NULL;
}
#endif

#if defined MACROTHREADING_WINDOWS
DWORD WINAPI macrothread_windows_wrapper(LPVOID arg)
{
    macro_thread_handle* parent = (macro_thread_handle*)arg;
    parent->thread_fun(parent->arguement);
    return 0;
}
#endif


void macro_thread_set_stack_depth(macro_thread_handle *handle, stack_depth_t stack_depth)
{
    #if defined MACROTHREADING_ESP32 \
        || defined MACROTHREADING_PTHREADS \
        || defined MACROTHREADING_WINDOWS
    handle->stack_depth = stack_depth;
    #endif
}

void macro_thread_start_thread(
    macro_thread_handle *handle, 
    void (function)(void*), 
    void* arg)
{
    #if defined MACROTHREADING_ESP32
    xTaskCreatePinnedToCore(
        function,
        handle->name,
        handle->stack_depth,
        arg,
        handle->priority,
        &handle->handle,
        handle->core_id;
        );
    #elif defined MACROTHREADING_PTHREADS
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, handle->stack_depth);
    handle->thread_fun = function;
    handle->arguement = arg;
    pthread_create(&handle->handle, &attr, macrothread_pthread_wrapper, (void*)handle);
    pthread_attr_destroy(&attr);
    #elif defined MACROTHREADING_WINDOWS
    handle->thread_fun = function;
    handle->arguement = arg;
    handle->handle = CreateThread(
        NULL,
        handle->stack_depth,
        macrothread_windows_wrapper,
        (void*)handle,
        0,
        &handle->thread_id
    );
    if (handle->handle == NULL) {
        ExitProcess(1);
    }
    #else
    function(arg);
    #endif
}

void macro_thread_delay(unsigned long int milliseconds)
{
    #if defined MACROTHREADING_ESP32
    vTaskDelay(milliseconds / portTickRateMS);
    #elif defined MACROTHREADING_PTHREADS
    usleep(milliseconds * 1000);
    #elif defined MACROTHREADING_WINDOWS
    Sleep(milliseconds);
    #else
    #warning macro_thread_delay not defined
    #endif
}

void macro_thread_join(macro_thread_handle *input)
{
    #if defined MACROTHREADING_ESP32
    #warning Not implemented
    #elif defined MACROTHREADING_PTHREADS
    pthread_join(input->handle, NULL);
    #elif defined MACROTHREADING_WINDOWS
    WaitForSingleObject(
        input->handle,
        INFINITE
    );
    #else
    #warning Join not applicable when threading is not set
    #endif
}