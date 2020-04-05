#include "macrothreading_thread.h"

#ifdef MACROTHREADING_ESP32
void macrothread_esp_wrapper(void* arg)
{
    macrothread_handle_t parent = (macrothread_handle_t)arg;
    parent->thread_fun(parent->arguement);
    if(parent->detached) {
        vEventGroupDelete(parent->join_event);
        macrothread_handle_destroy(parent);
    } else {
        xEventGroupSetBits(parent->join_event, MACROTHREADING_JOIN_MASK);
    }
}
#endif

#ifdef MACROTHREADING_PTHREADS
void* macrothread_pthread_wrapper(void* arg)
{
    macrothread_handle_t parent = (macrothread_handle_t)arg;
    parent->thread_fun(parent->arguement);
    if(parent->detached) {
        macrothread_handle_destroy(parent);
    }
    return NULL;
}
#endif

#if defined MACROTHREADING_WINDOWS
DWORD WINAPI macrothread_windows_wrapper(LPVOID arg)
{
    macrothread_handle_t parent = (macrothread_handle_t)arg;
    parent->thread_fun(parent->arguement);
    if(parent->detached) {
        macrothread_handle_destroy(parent);
    }
    return 0;
}
#endif

macrothread_handle_t macrothread_handle_init()
{
    macrothread_handle_t handle;

    #if defined MACROTHREADING_ESP32
    macrothread_handle_struct_t result = {
        .handle = NULL,
        .detached = false,
        .thread_fun = NULL,
        .arguement = NULL,
        .name = NULL,
        .stack_depth = 1024,
        .priority = 5,
        .core_id = tskNO_AFFINITY
    };
    #elif defined MACROTHREADING_PTHREADS
    macrothread_handle_struct_t result = {
        .detached = false,
        .stack_depth = 8388608,
        .thread_fun = NULL,
        .arguement = NULL
    };
    #elif defined MACROTHREADING_WINDOWS
    macrothread_handle_struct_t result = {
        .handle = NULL,
        .detached = false,
        .stack_depth = 8388608,
        .thread_fun = NULL,
        .arguement = NULL
    };
    #else
    macrothread_handle_struct_t result;
    #endif

    handle = (macrothread_handle_t)malloc(sizeof(macrothread_handle_struct_t));
    memcpy(handle, &result, sizeof(macrothread_handle_struct_t));

    return handle;
}

void macrothread_set_stack_depth(macrothread_handle_t handle, stack_depth_t stack_depth)
{
    handle->stack_depth = stack_depth;
}

#ifdef MACROTHREADING_ESP32
void macrothread_set_name(macrothread_handle_t handle, const char* name)
{
    handle->name = name;
}

void macrothread_set_priority(macrothread_handle_t handle, unsigned int priority)
{
    handle->priority = priority;
}

void macrothread_set_core(macrothread_handle_t handle, int core)
{
    handle->core_id = core;
}
#endif

void macrothread_start_thread(
    macrothread_handle_t handle, 
    void (function)(void*), 
    void* arg)
{
    #if defined MACROTHREADING_ESP32
    handle->thread_fun = function;
    handle->arguement = arg;
    handle->join_event = xEventGroupCreate();
    xTaskCreatePinnedToCore(
        macrothread_esp_wrapper,
        handle->name,
        handle->stack_depth,
        handle,
        handle->priority,
        &handle->handle,
        handle->core_id
        );
    #elif defined MACROTHREADING_PTHREADS
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, handle->stack_depth);
    if(handle->detached) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }
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
    if(handle->detached) {
        CloseHandle(handle->handle);
    }
    #else
    function(arg);
    #endif
}

void macrothread_start_detached_thread(
    void (function)(void*), 
    void* arg
)
{
    macrothread_handle_t handle = macrothread_handle_init();
    handle->detached = true;
    macrothread_start_thread(handle, function, arg);
}

void macrothread_delay(unsigned long int milliseconds)
{
    #if defined MACROTHREADING_ESP32
    vTaskDelay(milliseconds / portTICK_PERIOD_MS);
    #elif defined MACROTHREADING_PTHREADS
    usleep(milliseconds * 1000);
    #elif defined MACROTHREADING_WINDOWS
    Sleep(milliseconds);
    #else
    #warning macrothread_delay not defined
    #endif
}

void macrothread_join(macrothread_handle_t input)
{
    if(input->detached) return;
    #if defined MACROTHREADING_ESP32
    xEventGroupWaitBits(
        input->join_event,
        MACROTHREADING_JOIN_MASK,
        pdTRUE,
        pdTRUE,
        portMAX_DELAY
    );
    vEventGroupDelete(input->join_event);
    #elif defined MACROTHREADING_PTHREADS
    pthread_join(input->handle, NULL);
    #elif defined MACROTHREADING_WINDOWS
    WaitForSingleObject(
        input->handle,
        INFINITE
    );
    CloseHandle(input->handle);
    #else
    #warning Join not applicable when threading is not set
    #endif
}

void macrothread_handle_destroy(macrothread_handle_t handle)
{
    free(handle);
}