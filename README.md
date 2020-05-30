[![Build status](https://ci.appveyor.com/api/projects/status/9bi8fyqit1m958h8?svg=true)](https://ci.appveyor.com/project/gregjesl/macrothreading)
# Macrothreading
Cross-platform multithreading support written in C for Linux, Mac, Windows, and the [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview). 

## Motiviation
The ability to target multiple platforms makes any application or library more flexible, thus making it more useful. The specific motiviation for this project was the ability to run (and debug) applications meant for the ESP32 on a normal computer for debugging. 

## Requirements
- [CMake](https://cmake.org/) is used for project compilation
- For Linux and Mac, [pthreads](https://en.wikipedia.org/wiki/POSIX_Threads) is required (but likely already installed)
- Targeting ESP32 platforms requires installing the [ESP IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/), which you've likely already done if you're building ESP32 applications

## Linking
To include the header files and link the library to your project, you can add
```cmake
add_subdirectory(macrothreading)
...
target_link_libraries(${CMAKE_PROJECT_NAME} macrothreading)
```
to your project's `CMakeLists.txt`. 

## Usage
Macrothreading includes three main tools: threads, mutexes, and conditions

### Threads
Starting a thread is as simple as
```c
#include "macrothreading_thread.h"

void function(void*)
{
    // This is the function the thread will execute
}

int main(void)
{
    // Create the thread handle
    macrothread_handle_t handle = macrothread_handle_init();

    // Start the thread
    macrothread_start_thread(handle, function, NULL);

    // Join the thread
    macrothread_join(handle);

    // Destory the handle
    macrothread_handle_destroy(handle);
}
```

The following functions can be used to configure the thread:

Function | Descrition | Relevant Platforms
---------|------------|--------------------
`macrothread_set_stack_depth` | Sets the stack depth of the thread | All
`macrothread_set_name` | Sets the name of the thread | ESP32
`macrothread_set_priority` | Sets the priority of the thread | ESP32, Linux, Mac
`macrothread_set_core` | Sets the core the thread will run on | ESP32

The functions listed above can be called from any platform even if they are only relevant to some platforms. These functions must be called before `macrothread_start_thread` to have an effect on the thread behavior. 

Cross-platform thread delays are also supported via `macrothread_delay`. 

By default, threads must be joined when completed to prevent memory leaks. Threads can also be launched in a detached state; detached threads do not require a call to `macrothread_join()` and will automaticaly destroy their handle when the thread is terminated. 

### Mutex
Mutexes are very straight-forward:
```c
#include "macrothreading_mutex.h"

int main(void)
{
    // Initialize the mutex
    macrothread_mutex_t mutex = macrothread_mutex_init();
    
    // Lock the mutex
    macrothread_mutex_lock(mutex);

    // Unlock the mutex
    macrothread_mutex_unlock(mutex);

    // Destroy the mutex
    macrothread_mutex_destroy(mutex);
}
```

### Condition
Conditions are used to block a thread until another thread provides a signal:
```c
#include "macrothreading_thread.h"
#include "macrothreading_condition.h"

macrothread_condition_t condition;
macrothread_condition_t answer;

void function(void* arg)
{
    // Wait for the signal
    macrothread_condition_wait(condition);

    // Wait some time (simulate work)
    macrothread_delay(10);

    // Send the signal
    macrothread_condition_signal(answer);
}

int main(void)
{
    // Create the thread handle
    macrothread_handle_t handle = macrothread_handle_init();

    // Initialize the condition
    condition = macrothread_condition_init();
    answer = macrothread_condition_init();

    // Start the thread
    macrothread_start_thread(handle, function, NULL);

    // Wait some time (simulate work)
    macrothread_delay(10);

    // Send the signal
    macrothread_condition_signal(condition);

    // Wait for the answer
    macrothread_condition_wait(answer);

    // Join the thread
    macrothread_join(handle);

    // Destroy the condition
    macrothread_condition_destroy(condition);
    macrothread_condition_destroy(answer);

    // Destroy the handle
    macrothread_handle_destroy(handle);
}
```

## Testing
This library follows the standard CMake build & test process:
```bash
mkdir build
cd build
cmake ../
make
make test
```