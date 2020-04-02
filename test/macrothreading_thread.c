#include "macrothreading_thread.h"
#include "test.h"

const int initial_value = 1;
int test_value = initial_value;

void function(void* arg)
{
    macro_thread_delay(10);
    test_value += *(int*)arg;
}

int main(void)
{
    // Create the thread handle
    macro_thread_handle handle;

    // Set the stack depth
    macro_thread_set_stack_depth(&handle, 2048);

    // Start the thread
    const int arg = 1;
    macro_thread_start_thread(&handle, function, (void*)&arg);

    // Join the thread
    macro_thread_join(&handle);

    // Verify the result
    TEST_EQUAL(test_value, initial_value + arg);
}