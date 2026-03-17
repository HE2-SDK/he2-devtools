cmake_minimum_required(VERSION 3.28)
project(imgui-club LANGUAGES CXX)

add_library(imgui-memory-editor INTERFACE)
target_link_libraries(imgui-memory-editor INTERFACE imgui)
target_sources(imgui-memory-editor
    INTERFACE FILE_SET HEADERS BASE_DIRS imgui_club/imgui_memory_editor FILES
        imgui_club/imgui_memory_editor/imgui_memory_editor.h
)
