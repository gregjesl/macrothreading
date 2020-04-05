#include "macrothreading_thread.h"
#include "macrothreading_mutex.h"
#include "test.h"

#define INITIAL_VALUE 1

int test_value = INITIAL_VALUE;
macrothread_mutex_t mutex;

void function(void* arg)
{
    // Lock the mutex
    macrothread_mutex_lock(mutex);

    // Increment the value
    test_value += *(int*)arg;

    // Unlock the mutex
    macrothread_mutex_unlock(mutex);
}

int main(void)
{
    // Create the thread handle
    macrothread_handle handle = macrothread_handle_init();

    // Set the stack depth
    macrothread_set_stack_depth(&handle, 2048);

    // Initialize the mutex
    mutex = macrothread_mutex_init();

    // Lock the mutex
    macrothread_mutex_lock(mutex);

    // Start the thread
    const int arg = 1;
    macrothread_start_thread(&handle, function, (void*)&arg);

    // Wait some time
    macrothread_delay(10);

    // Verify no change
    TEST_EQUAL(test_value, INITIAL_VALUE);

    // Unlock the mutex
    macrothread_mutex_unlock(mutex);

    // Loop until updated
    macrothread_mutex_lock(mutex);
    while(test_value == INITIAL_VALUE) {
        macrothread_mutex_unlock(mutex);
        macrothread_delay(1);
        macrothread_mutex_lock(mutex);
    }

    // Unlock the mutex
    macrothread_mutex_unlock(mutex);

    // Join the thread
    macrothread_join(&handle);

    // Verify the result
    TEST_EQUAL(test_value, INITIAL_VALUE + arg);

    // Destroy the mutex
    macrothread_mutex_destroy(mutex);
}