cmake_minimum_required(VERSION 3.7.2)

project(EXAMPLE VERSION 0.0.1)

add_subdirectory(JUCE)

juce_add_console_app(example
    PRODUCT_NAME "Example"
    )

juce_generate_juce_header(example)

target_sources(example PRIVATE main.cpp)

target_compile_definitions(example PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0)

target_link_libraries(example PRIVATE
    juce::juce_core
    juce::juce_audio_utils)

set_target_properties(example PROPERTIES CXX_STANDARD 17)