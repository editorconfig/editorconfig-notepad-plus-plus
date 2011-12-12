# this file is part of EditorConfig plugin for Notepad++
# 
# Copyright (C)2011 EditorConfig Team <http://editorconfig.org>
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
# 

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

# suppress warning C4996 for MSVC
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

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

