cmake_minimum_required(VERSION 3.2)
project(PluginCenter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_SYSTEM_NAME Windows)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} RootSrc)
set(source_files ${RootSrc} PluginCenter.rc)

get_filename_component(DUILIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../DuiLib" ABSOLUTE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${DUILIB_DIR})
include_directories(${DUILIB_DIR}/Control ${DUILIB_DIR}/Core ${DUILIB_DIR}/Core/Plugin)
include_directories(${DUILIB_DIR}/Layout ${DUILIB_DIR}/Utils)

add_executable(PluginCenter WIN32 ${source_files})
add_dependencies(PluginCenter DuiLib)
target_compile_options(PluginCenter PRIVATE /utf-8)

target_link_libraries(PluginCenter
    "${CMAKE_BINARY_DIR}/lib/$<CONFIG>/DuiLib.lib"
    user32.lib gdi32.lib comctl32.lib)

set_target_properties(PluginCenter PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/plugins)

add_custom_command(TARGET PluginCenter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/PluginCenter.exe ${CMAKE_SOURCE_DIR}/bin/PluginCenter.exe)

add_custom_command(TARGET PluginCenter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_center_main.xml ${CMAKE_SOURCE_DIR}/bin/plugin_center_main.xml)
add_custom_command(TARGET PluginCenter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_center_dlg.xml ${CMAKE_SOURCE_DIR}/bin/plugin_center_dlg.xml)

add_custom_command(TARGET PluginCenter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_center_main.xml ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugin_center_main.xml)
add_custom_command(TARGET PluginCenter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_center_dlg.xml ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/plugin_center_dlg.xml)