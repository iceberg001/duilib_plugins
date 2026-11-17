cmake_minimum_required(VERSION 3.2)

project(DuiLib VERSION 1.0.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type: Debug/Release" FORCE)
endif()

if(NOT CMAKE_GENERATOR_TOOLSET)
    set(CMAKE_GENERATOR_TOOLSET "v100" CACHE STRING "VS Toolset (v100=VS2010, v140=VS2015, v141=VS2017, v142=VS2019, v143=VS2022)")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_definitions(-DCMAKE)
add_definitions(-DUNICODE -D_UNICODE)

add_compile_options(/W3 /WX-)
add_definitions(/D_UNICODE /DUNICODE)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 5.01)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# sub projects
add_subdirectory(DuiLib)
add_subdirectory(Demos)
add_subdirectory(Plugins/ExamplePlugin)
add_subdirectory(Plugins/PluginCenterPlugin)

# build with powershell
# mkdir build ; cd build
# cmake -G "Visual Studio 10 2010" -A Win32 ..
# cmake --build ../build