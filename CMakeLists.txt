cmake_minimum_required (VERSION 3.8)

if(ESP_PLATFORM)
    set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test/")
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
endif()

project(macrothreading)

if(ESP_PLATFORM)
    file(GLOB sources
        "../src/*.c"
    )

    if(__idf_component_context)
        idf_component_register(
            SRCS ${sources}
            INCLUDE_DIRS "inc"
        )
    else()
        set(IS_ESP_COMPONENT True)
    endif()
else()
    enable_testing()
    add_subdirectory(test)
endif()