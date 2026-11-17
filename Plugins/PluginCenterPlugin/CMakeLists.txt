cmake_minimum_required(VERSION 3.2)
project(PluginCenterPlugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_SYSTEM_NAME Windows)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PluginSrc)

get_filename_component(DUILIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../DuiLib" ABSOLUTE)
include_directories(${DUILIB_DIR})
include_directories(${DUILIB_DIR}/Control ${DUILIB_DIR}/Core ${DUILIB_DIR}/Core/Plugin)
include_directories(${DUILIB_DIR}/Layout ${DUILIB_DIR}/Utils)

add_library(PluginCenterPlugin SHARED ${PluginSrc})
add_dependencies(PluginCenterPlugin DuiLib)
target_compile_options(PluginCenterPlugin PRIVATE /utf-8)

target_link_libraries(PluginCenterPlugin
    "${CMAKE_BINARY_DIR}/lib/$<CONFIG>/DuiLib.lib"
    user32.lib gdi32.lib comctl32.lib)

set_target_properties(PluginCenterPlugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/plugins)

add_custom_command(TARGET PluginCenterPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/PluginCenterPlugin.dll
    ${CMAKE_SOURCE_DIR}/bin/plugins/PluginCenterPlugin.dll)

add_custom_command(TARGET PluginCenterPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/PluginCenterPlugin.dll
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugins/PluginCenterPlugin.dll)