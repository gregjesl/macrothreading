#include "macrothreading_thread.h"
#include "macrothreading_condition.h"
#include "test.h"

#define INITIAL_VALUE 1

int test_value = INITIAL_VALUE;
macrothread_condition_t condition;
macrothread_condition_t answer;

void function(void* arg)
{
    // Wait for the signal
    macrothread_condition_wait(condition);

    // Increment the value
    test_value += *(int*)arg;

    // Wait some time
    macrothread_delay(10);

    // Send the signal
    macrothread_condition_signal(answer);
}

int main(void)
{
    // Create the thread handle
    macrothread_handle_t handle = macrothread_handle_init();

    // Set the stack depth
    macrothread_set_stack_depth(handle, 2048);

    // Initialize the condition
    condition = macrothread_condition_init();
    answer = macrothread_condition_init();

    // Start the thread
    const int arg = 1;
    macrothread_start_thread(handle, function, (void*)&arg);

    // Wait some time
    macrothread_delay(10);

    // Verify no change
    TEST_EQUAL(test_value, INITIAL_VALUE);

    // Send the signal
    macrothread_condition_signal(condition);

    // Wait for the signal
    macrothread_condition_wait(answer);

    // Join the thread
    macrothread_join(handle);

    // Verify the result
    TEST_EQUAL(test_value, INITIAL_VALUE + arg);

    // Destroy the condition
    macrothread_condition_destroy(condition);
    macrothread_condition_destroy(answer);
}