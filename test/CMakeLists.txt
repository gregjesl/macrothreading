cmake_minimum_required (VERSION 3.8)

include_directories(
    ./
    ../inc
)

file(GLOB tests
    "*.c"
)

file(GLOB sources
    "../src/*.c"
)

# Pthreads
find_package(Threads)
if(CMAKE_USE_PTHREADS_INIT)
    foreach(test ${tests})
        set(THREADS_PREFER_PTHREAD_FLAG TRUE)
        string(REGEX REPLACE ".*/" "" test_name "${test}")
        string(REGEX REPLACE ".c$" "" test_name "${test_name}")
        add_executable ("${PROJECT_NAME}_pthreads_${test_name}_test" ${test} ${sources})
        add_test(NAME "${PROJECT_NAME}_pthreads_${test_name}" COMMAND "${PROJECT_NAME}_pthreads_${test_name}_test")
        target_compile_definitions("${PROJECT_NAME}_pthreads_${test_name}_test"
            PUBLIC MACROTHREADING_PTHREADS)
    endforeach()
endif()
