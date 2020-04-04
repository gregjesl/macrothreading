#include "macrothreading_thread.h"
#include "test.h"

#define INITIAL_VALUE 1

int test_value = INITIAL_VALUE;

void function(void* arg)
{
    macrothread_delay(10);
    test_value += *(int*)arg;
}

int main(void)
{
    // Create the thread handle
    macrothread_handle handle;

    // Set the stack depth
    macrothread_set_stack_depth(&handle, 2048);

    // Start the thread
    const int arg = 1;
    macrothread_start_thread(&handle, function, (void*)&arg);

    // Join the thread
    macrothread_join(&handle);

    // Verify the result
    TEST_EQUAL(test_value, INITIAL_VALUE + arg);
}