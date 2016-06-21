//this file is part of EditorConfig plugin for Notepad++
//
//Copyright (C)2003 Don HO <donho@altern.org>
//Copyright (C)2011-2016 EditorConfig Team <http://editorconfig.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef PLUGINDEFINITION_HPP
#define PLUGINDEFINITION_HPP

#include <string>
#include <sstream>

#include "editorconfig/editorconfig.h"

//
// All difinitions of plugin interface
//
#include "PluginInterface.hpp"

// tstring and tstringstream class
namespace std
{
#ifdef UNICODE
    typedef wstring tstring;
    typedef wstringstream tstringstream;
#else
    typedef string tstring;
    typedef stringstream tstringstream;
#endif
};

//-------------------------------------//
//-- STEP 1. DEFINE YOUR PLUGIN NAME --//
//-------------------------------------//
// Here define your plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT("EditorConfig");

//-----------------------------------------------//
//-- STEP 2. DEFINE YOUR PLUGIN COMMAND NUMBER --//
//-----------------------------------------------//
//
// Here define the number of your plugin commands
//
const int nbFunc = 3;


struct NppData;
extern HINSTANCE hInst;
extern NppData nppData;

//
// Initialization of your plugin data
// It will be called while plugin loading
//
void pluginInit(HANDLE hModule);

//
// Cleaning of your plugin
// It will be called while plugin unloading
//
void pluginCleanUp();

//
//Initialization of your plugin commands
//
void commandMenuInit();

//
//Clean up your plugin commands allocation (if any)
//
void commandMenuCleanUp();

//
// load config files
//
void loadConfig();

//
// Parse EditorConfig file
//
void onBeforeSave(HWND hWnd);

//
// Function which sets your command
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);

//
// Center the window, relative the NPP-window
//
void centerWindow(HWND hDlg);


#endif //PLUGINDEFINITION_HPP
