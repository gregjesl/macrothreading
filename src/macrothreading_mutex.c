#include "macrothreading_mutex.h"

macrothread_mutex_t macrothread_mutex_init()
{
    #if defined MACROTHREADING_ESP32
    return xSemaphoreCreateMutex();
    #elif defined MACROTHREADING_PTHREADS
    pthread_mutex_t *mutex;
    mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if(mutex == NULL) {
        return NULL;
    }
    return pthread_mutex_init(mutex, NULL) == 0 ? mutex : NULL;
    #elif defined MACROTHREADING_WINDOWS
    HANDLE *mutex;
    mutex = (HANDLE*)malloc(sizeof(HANDLE));
    *mutex = CreateMutex( 
        NULL,   // default security attributes
        FALSE,  // initially not owned
        NULL);  // unnamed mutex
    return mutex;
    #else
    return false;
    #endif
}

void macrothread_mutex_lock(macrothread_mutex_t mutex)
{
    #if defined MACROTHREADING_ESP32
    xSemaphoreTake(mutex, portMAX_DELAY);
    #elif defined MACROTHREADING_PTHREADS
    if(pthread_mutex_lock(mutex) != 0) {
        exit(1);
    }
    #elif defined MACROTHREADING_WINDOWS
    DWORD result;
    result = WaitForSingleObject( 
        *mutex,     // handle to mutex
        INFINITE);  // no time-out interval

    switch (result)
    {
    case WAIT_OBJECT_0:
        break;
    case WAIT_ABANDONED:
        ExitProcess(1);
    }
    #else
    if(mutex) {
        exit(1);
    }
    mutex = true;
    #endif
}

void macrothread_mutex_unlock(macrothread_mutex_t mutex)
{
    #if defined MACROTHREADING_ESP32
    xSemaphoreGive(mutex);
    #elif defined MACROTHREADING_PTHREADS
    if(pthread_mutex_unlock(mutex) != 0) {
        exit(1);
    }
    #elif defined MACROTHREADING_WINDOWS
    if(! ReleaseMutex(*mutex)) {
        ExitProcess(1);
    }
    #else
    if(!mutex) {
        exit(1);
    }
    mutex = false;
    #endif
}

void macrothread_mutex_destroy(macrothread_mutex_t mutex)
{
    #if defined MACROTHREADING_ESP32
    vSemaphoreDelete(mutex);
    #elif defined MACROTHREADING_PTHREADS
    if(pthread_mutex_destroy(mutex) != 0) {
        exit(1);
    }
    free(mutex);
    #elif defined MACROTHREADING_WINDOWS
    if(!CloseHandle(*mutex)) {
        ExitProcess(1);
    }
    free(mutex);
    #else
    if(mutex) {
        exit(1);
    }
    #endif
}