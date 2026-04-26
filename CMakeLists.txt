cmake_minimum_required(VERSION 3.21)
project(map_converter_for_ssf
        VERSION 0.6.8
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/Map_converter_for_SSF)

file(GLOB SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

if(WIN32)
  list(APPEND SRC ${SRC_DIR}/Map_converter_for_SSF.rc)
endif()

add_executable(map_converter ${SRC})

target_include_directories(map_converter PRIVATE
  ${SRC_DIR}
  ${CMAKE_SOURCE_DIR}/libs/gzip-hpp-master/include
)

if(WIN32)
  target_include_directories(map_converter PRIVATE ${CMAKE_SOURCE_DIR}/libs/zlib/include)
  target_link_directories(map_converter PRIVATE ${CMAKE_SOURCE_DIR}/libs/zlib/lib)
  target_link_libraries(map_converter PRIVATE zlibstatic)
else()
  find_package(ZLIB REQUIRED)
  target_link_libraries(map_converter PRIVATE ZLIB::ZLIB)
endif()

if(MSVC)
  target_compile_options(map_converter PRIVATE /W4 /MP /Zc:__cplusplus /utf-8)
  target_compile_definitions(map_converter PRIVATE _CRT_SECURE_NO_WARNINGS UNICODE _UNICODE)
endif()
