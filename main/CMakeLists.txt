cmake_minimum_required (VERSION 3.8)

file(GLOB sources
    "../src/*.c"
)

if(ESP_PLATFORM AND NOT IS_ESP_COMPONENT)
    idf_component_register(
        SRCS "../test/macrothreading_thread.c" ${sources}
        INCLUDE_DIRS "../inc"
    )
endif()

