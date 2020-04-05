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
    set(config_name pthreads)
    foreach(test ${tests})
        set(THREADS_PREFER_PTHREAD_FLAG TRUE)
        string(REGEX REPLACE ".*/" "" test_name "${test}")
        string(REGEX REPLACE ".c$" "" test_name "${test_name}")
        set(full_name "${PROJECT_NAME}_${config_name}_${test_name}")
        add_executable ("${full_name}_test" ${test} ${sources})
        add_test(NAME "${full_name}" COMMAND "${full_name}_test")
        target_compile_definitions("${full_name}_test")
            PUBLIC MACROTHREADING_PTHREADS)
        target_link_libraries ("${full_name}_test" ${CMAKE_THREAD_LIBS_INIT})
    endforeach()
endif()

# Win32
if(WIN32)
    set(config_name windows)
    foreach(test ${tests})
        string(REGEX REPLACE ".*/" "" test_name "${test}")
        string(REGEX REPLACE ".c$" "" test_name "${test_name}")
        set(full_name "${PROJECT_NAME}_${config_name}_${test_name}")
        add_executable ("${full_name}_test" ${test} ${sources})
        add_test(NAME "${full_name}" COMMAND "${full_name}_test")
        target_compile_definitions("${full_name}_test")
            PUBLIC MACROTHREADING_WINDOWS)
    endforeach()
endif()
