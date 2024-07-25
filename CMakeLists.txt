# this file is part of EditorConfig plugin for Notepad++
#
# Copyright (C)2011-2016 EditorConfig Team <http://editorconfig.org>
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

cmake_minimum_required(VERSION 3.16.3)

project(NppEditorConfig CXX)

set (editorconfig_notepad_plus_plus_VERSION_MAJOR 0)
set (editorconfig_notepad_plus_plus_VERSION_MINOR 4)
set (editorconfig_notepad_plus_plus__VERSION_SUBMINOR 0)

# set the EDITORCONFIG_CORE_PREFIX. Default is the default installation
# directory of editorconfig
if(WIN32)
    set(EDITORCONFIG_CORE_DEFAULT_PREFIX "C:/Program Files/editorconfig")
endif()

# if the default path does not exist, set it to empty string
if(NOT IS_DIRECTORY ${EDITORCONFIG_CORE_DEFAULT_PREFIX})
    set(EDITORCONFIG_CORE_DEFAULT_PREFIX "")
endif()

if(NOT EDITORCONFIG_CORE_PREFIX)
    set(EDITORCONFIG_CORE_PREFIX ${EDITORCONFIG_CORE_DEFAULT_PREFIX}
        CACHE PATH "Choose the prefix of EditorConfig C Core.")
endif()

if(NOT PCRE2_LIB_DIR)
    set(PCRE2_LIB_DIR "" CACHE PATH "Choose the dir that contains pcre2.lib.")
endif()

add_subdirectory(src)
