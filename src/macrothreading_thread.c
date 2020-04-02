#include "macrothreading_thread.h"

void macro_thread_set_stack_depth(macro_thread_handle *handle, stack_depth_t stack_depth)
{
    #if defined MACROTHREADING_ESP32 
        || defined MACROTHREADING_PTHREADS
        || defined MACTROTHREADING_WINDOWS
    handle->stack_depth = stack_depth;
    #endif
}

void macro_thread_start_thread(
    macro_thread_handle *handle, 
    MACRO_THREAD_FUNCTION_RETURN (function)(MACRO_THREAD_FUNCTION_ARGUEMENT), 
    MACRO_THREAD_FUNCTION_ARGUEMENT arg)
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
    pthread_create(&handle->handle, &attr, function, arg);
    pthread_attr_destroy(&attr);
    #elif defined MACTROTHREADING_WINDOWS
    handle->handle = CreateThread(
        NULL,
        handle->stack_depth,
        function,
        arg,
        0,
        &handle->thread_id
    );
    #else
    function(arg);
    #endif
}

void macro_thread_join(macro_thread_handle *handle)
{
    #if defined MACROTHREADING_ESP32
    #warning Not implemented
    #elif defined MACROTHREADING_PTHREADS
    pthread_join(handle->handle, NULL);
    #elif defined MACTROTHREADING_WINDOWS
    DWORD WaitForSingleObject(
        handle->handle,
        INFINITE
    );
    #else
    #warning Join not applicable when threading is not set
    #endif
}