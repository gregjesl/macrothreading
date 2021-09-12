#include "macrothreading_condition.h"
#include <stdio.h>

macrothread_condition_t macrothread_condition_init()
{
    #if defined MACROTHREADING_ESP32
    return xEventGroupCreate();
    #elif defined MACROTHREADING_PTHREADS
    macrothread_condition_t result;
    result = (macrothread_condition_t)malloc(sizeof(macrothread_condition_struct_t));
    if(result == NULL) return NULL;

    result->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if(result->mutex == NULL) {
        free(result);
        return NULL;
    }
    if(pthread_mutex_init(result->mutex, NULL) != 0) {
        free(result->mutex);
        free(result);
        return NULL;
    }
    result->cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
    if(result->cond == NULL) {
        free(result->mutex);
        free(result);
        return NULL;
    }
    if(pthread_cond_init(result->cond, NULL) != 0) {
        free(result->mutex);
        free(result->cond);
        free(result);
        return NULL;
    }
    result->signaled = false;
    return result;
    #elif defined MACROTHREADING_WINDOWS
    macrothread_condition_t result;
    result = (macrothread_condition_t)malloc(sizeof(macrothread_condition_struct_t));
    if(result == NULL) return NULL;
    result->mutex = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
    if(result->mutex == NULL) {
        free(result);
        return NULL;
    }
    InitializeCriticalSection(result->mutex);
    result->cond = (CONDITION_VARIABLE*)malloc(sizeof(CONDITION_VARIABLE));
    if(result->cond == NULL) {
        free(result->mutex);
        free(result);
        return NULL;
    }
    InitializeConditionVariable(result->cond);
    result->signaled = false;
    return result;
    #else
    return false;
    #endif
}

void macrothread_condition_wait(macrothread_condition_t cond)
{
    #if defined MACROTHREADING_ESP32
    xEventGroupWaitBits(
        cond,
        MACROTHREADING_EVENT_MASK,
        pdTRUE,
        pdTRUE,
        portMAX_DELAY
    );
    #elif defined MACROTHREADING_PTHREADS
    if(pthread_mutex_lock(cond->mutex) != 0) {
        exit(1);
    }
    while(!cond->signaled) {
        if(pthread_cond_wait(cond->cond, cond->mutex) != 0) {
            exit(1);
        }
    }
    cond->signaled = false;
    if(pthread_mutex_unlock(cond->mutex) != 0) {
        exit(1);
    }
    #elif defined MACROTHREADING_WINDOWS
    EnterCriticalSection(cond->mutex);
    while(!cond->signaled) {
        SleepConditionVariableCS (cond->cond, cond->mutex, INFINITE);
    }
    cond->signaled = false;
    LeaveCriticalSection (cond->mutex);
    #else
    // Do nothing
    #endif
}

void macrothread_condition_signal(macrothread_condition_t cond)
{
    #if defined MACROTHREADING_ESP32
    xEventGroupSetBits(cond, MACROTHREADING_EVENT_MASK);
    #elif defined MACROTHREADING_PTHREADS
    if(pthread_mutex_lock(cond->mutex) != 0) {
        exit(1);
    }
    cond->signaled = true;
    if(pthread_cond_signal(cond->cond) != 0) {
        exit(1);
    }
    if(pthread_mutex_unlock(cond->mutex) != 0) {
        exit(1);
    }
    #elif defined MACROTHREADING_WINDOWS
    EnterCriticalSection(cond->mutex);
    cond->signaled = true;
    LeaveCriticalSection (cond->mutex);
    WakeConditionVariable(cond->cond);
    #else
    // Do nothing
    #endif
}

void macrothread_condition_destroy(macrothread_condition_t cond)
{
    #if defined MACROTHREADING_ESP32
    vEventGroupDelete(cond);
    #elif defined MACROTHREADING_PTHREADS
    if(pthread_cond_destroy(cond->cond) != 0) {
        perror("pthread_cond_destroy error");
    }
    if(pthread_mutex_destroy(cond->mutex) != 0) {
        perror("pthread_mutex_destroy error");
    }
    free(cond->cond);
    free(cond->mutex);
    free(cond);
    #elif defined MACROTHREADING_WINDOWS
    free(cond->cond);
    free(cond->mutex);
    free(cond);
    #else
    // Do nothing
    #endif
}