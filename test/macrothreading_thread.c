#include "macrothreading_thread.h"
#include "test.h"

const int initial_value = 1;
int test_value = initial_value;

MACRO_THREAD_FUNCTION_RETURN function(MACRO_THREAD_FUNCTION_ARGUEMENT arg)
{
    test_value += *(int*)arg;
    return NULL;
}

int main(void)
{
    // Create the thread handle
    macro_thread_handle handle;

    // Set the stack depth
    macro_thread_set_stack_depth(&handle, 2048);

    // Start the thread
    const int arg = 1;
    macro_thread_start_thread(&handle, function, (MACRO_THREAD_FUNCTION_ARGUEMENT)&arg);

    // Join the thread
    macro_thread_join(&handle);

    // Verify the result
    TEST_EQUAL(test_value, initial_value + arg);
}