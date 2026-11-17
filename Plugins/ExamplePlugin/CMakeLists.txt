cmake_minimum_required(VERSION 3.2)
project(ExamplePlugin VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_SYSTEM_NAME Windows)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PluginSrc)
get_filename_component(DUILIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../DuiLib" ABSOLUTE)
include_directories(${DUILIB_DIR})
include_directories(${DUILIB_DIR}/Control)
include_directories(${DUILIB_DIR}/Core)
include_directories(${DUILIB_DIR}/Core/Plugin)
include_directories(${DUILIB_DIR}/Layout)
include_directories(${DUILIB_DIR}/Utils)
add_library(ExamplePlugin SHARED ${PluginSrc})
add_dependencies(ExamplePlugin DuiLib)
target_compile_options(ExamplePlugin PRIVATE /utf-8)
target_link_libraries(ExamplePlugin
    "${CMAKE_BINARY_DIR}/lib/$<CONFIG>/DuiLib.lib"
    user32.lib gdi32.lib comctl32.lib)
set_target_properties(ExamplePlugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/plugins)

add_custom_command(TARGET ExamplePlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/ExamplePlugin.dll
    ${CMAKE_SOURCE_DIR}/bin/plugins/ExamplePlugin.dll)

add_custom_command(TARGET ExamplePlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/ExamplePlugin.dll
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugins/ExamplePlugin.dll)
