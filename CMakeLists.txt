cmake_minimum_required (VERSION 3.8)

if(ESP_PLATFORM AND NOT __idf_component_context)
    set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test/")
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
endif()

project(macrothreading)

# Declare the project name variable
set(MACROTHREADING_LIBRARY_NAME ${CMAKE_PROJECT_NAME})

# Get the source files
file(GLOB sources "src/*.c")

if(ESP_PLATFORM)
    if(__idf_component_context)
        idf_component_register(
            SRCS ${sources}
            INCLUDE_DIRS "inc"
        )
    else()
        set(IS_ESP_COMPONENT True)
    endif()
else()
    # Include directory
    include_directories(inc)

    # Create the library
    add_library(${CMAKE_PROJECT_NAME} ${sources})

    # Create the library
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT)
        target_compile_definitions(${CMAKE_PROJECT_NAME}
            PUBLIC MACROTHREADING_PTHREADS)
        target_link_libraries (${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
    # Win32
    elseif(WIN32)
        target_compile_definitions(${CMAKE_PROJECT_NAME}
            PUBLIC MACROTHREADING_WINDOWS)
    else()
        message(FATAL_ERROR "No supported threading method found")
    endif()

    enable_testing()
    add_subdirectory(test)
endif()