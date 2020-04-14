cmake_minimum_required (VERSION 3.8)

if(ESP_PLATFORM AND NOT __idf_component_context)
    set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test/")
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
endif()

project(macrothreading)

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
    # Create the library
    add_library(${PROJECT_NAME} ${sources})
    target_include_directories(${PROJECT_NAME} PUBLIC inc)

    # Create the library
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT)
        target_compile_definitions(${PROJECT_NAME}
            PUBLIC MACROTHREADING_PTHREADS)
        target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
    # Win32
    elseif(WIN32)
        target_compile_definitions(${PROJECT_NAME}
            PUBLIC MACROTHREADING_WINDOWS)
    else()
        message(FATAL_ERROR "No supported threading method found")
    endif()

    # Check for top-level project
    if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
        enable_testing()
        add_subdirectory(test)
    endif()
endif()