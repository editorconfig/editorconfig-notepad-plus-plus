
add_definitions(-DUNICODE -MT)

## section: source files
set(NppEditorConfig_SOURCE_FILES
    ./NppPluginEditorConfig.cpp
    ./PluginDefinition.cpp)

## section: header files
set(NppEditorConfig_HEADER_FILES
    ./menuCmdID.h
    ./Notepad_plus_msgs.h
    ./PluginDefinition.h
    ./PluginInterface.h
    ./Scintilla.h)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(NppEditorConfig_HEADER_FILES
    PROPERTIES HEADER_FILE_ONLY TRUE)
list(APPEND NppEditorConfig_SOURCE_FILES ${NppEditorConfig_HEADER_FILES})

## section: add target
add_library(NppEditorConfig SHARED ${NppEditorConfig_SOURCE_FILES})

target_link_libraries(NppEditorConfig editorconfig)

