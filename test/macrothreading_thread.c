#include "macrothreading_thread.h"
#include "macrothreading_condition.h"
#include "test.h"

#define INITIAL_VALUE 1

int test_value = INITIAL_VALUE;

void function(void* arg)
{
    macrothread_delay(10);
    test_value += *(int*)arg;
}

void detached_function(void *arg)
{
    macrothread_condition_t signal = (macrothread_condition_t)arg;
    macrothread_condition_signal(signal);
}

#if defined ESP_PLATFORM
void app_main(void)
#else
int main(void)
#endif
{
    // Create the thread handle
    macrothread_handle_t handle = macrothread_handle_init();

    // Verify the handle
    TEST_NOT_NULL(handle);

    // Set the stack depth
    macrothread_set_stack_depth(handle, 2048);

    // Start the thread
    const int arg = 1;
    macrothread_start_thread(handle, function, (void*)&arg);

    // Join the thread
    macrothread_join(handle);

    // Verify the result
    TEST_EQUAL(test_value, INITIAL_VALUE + arg);

    // Destory the handle
    macrothread_handle_destroy(handle);

    // Test a detached thread
    macrothread_condition_t signal = macrothread_condition_init();
    macrothread_start_detached_thread(detached_function, signal);
    macrothread_condition_wait(signal);
    macrothread_condition_destroy(signal);
}