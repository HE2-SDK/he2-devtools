cmake_minimum_required(VERSION 3.28)
project(imbrowser LANGUAGES CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>")
endif()

add_library(imbrowser STATIC)
target_compile_features(imbrowser PRIVATE cxx_std_20)
target_link_libraries(imbrowser PUBLIC imgui)
target_sources(imbrowser
    PRIVATE
        imbrowser/imbrowser.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS imbrowser FILES
        imbrowser/imbrowser.h
)
