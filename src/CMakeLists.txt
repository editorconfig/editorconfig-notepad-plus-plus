
# replace all /MD with /MT for msvc
if(MSVC)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
endif(MSVC)

add_definitions(-DUNICODE)

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

