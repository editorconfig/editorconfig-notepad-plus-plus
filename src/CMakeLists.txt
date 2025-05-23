# this file is part of EditorConfig plugin for Notepad++
#
# Copyright (C) 2011-2016 EditorConfig Team <https://editorconfig.org>
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
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
endif(MSVC)

# suppress warning C4996 for MSVC
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

## section: source files
set(NppEditorConfig_SOURCE_FILES
    ./DlgAbout.cpp
    ./NppPluginEditorConfig.cpp
    ./PluginDefinition.cpp)

## section: header files
set(NppEditorConfig_HEADER_FILES
    ./menuCmdID.hpp
    ./DlgAbout.hpp
    ./Notepad_plus_msgs.hpp
    ./PluginDefinition.hpp
    ./PluginInterface.hpp
    ./Resource.hpp
    ./Scintilla.hpp)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
if(EDITORCONFIG_CORE_PREFIX)
    include_directories(BEFORE "${EDITORCONFIG_CORE_PREFIX}/include")
endif()

set_source_files_properties(NppEditorConfig_HEADER_FILES
    PROPERTIES HEADER_FILE_ONLY TRUE)
list(APPEND NppEditorConfig_SOURCE_FILES ${NppEditorConfig_HEADER_FILES})

# version.rc, containing version number info
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY)

# add unicode target
link_directories("${EDITORCONFIG_CORE_PREFIX}/lib" "${PCRE2_LIB_DIR}")
add_library(NppEditorConfig_unicode SHARED
    ${NppEditorConfig_SOURCE_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
set_target_properties(NppEditorConfig_unicode PROPERTIES
    OUTPUT_NAME NppEditorConfig
    COMPILE_DEFINITIONS UNICODE
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/unicode
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/unicode
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/unicode)
target_link_libraries(NppEditorConfig_unicode editorconfig_static)
target_link_libraries(NppEditorConfig_unicode Shlwapi)
target_link_libraries(NppEditorConfig_unicode
                      debug pcre2-8-staticd
                      optimized pcre2-8-static)