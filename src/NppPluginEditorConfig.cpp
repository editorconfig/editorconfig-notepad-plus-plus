//this file is part of EditorConfig plugin for Notepad++
//
//Copyright (C)2003 Don HO <donho@altern.org>
//Copyright (C)2011 EditorConfig Team <http://editorconfig.org>
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

#include <string>
#include <sstream>
#include "PluginDefinition.hpp"
#include "editorconfig/editorconfig.h"

extern FuncItem funcItem[nbFunc];
extern NppData nppData;

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

BOOL APIENTRY DllMain(HANDLE hModule, 
        DWORD  reasonForCall, 
        LPVOID lpReserved)
{
    switch (reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        pluginInit(hModule);
        break;

    case DLL_PROCESS_DETACH:
        commandMenuCleanUp();
        pluginCleanUp();
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
    nppData = notpadPlusData;
    commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
    return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
    *nbF = nbFunc;
    return funcItem;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
    editorconfig_handle eh;
#ifdef UNICODE
    TCHAR fileNamet[MAX_PATH];
#endif
    char  fileName[MAX_PATH];
    HWND curScintilla;
    int which;
    int name_value_count;
    int err_num;

    switch (notifyCode->nmhdr.code) {
    case NPPN_BUFFERACTIVATED: // When new file opened, run set the conf
        eh = editorconfig_handle_init();

        // get the file name
        ::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH,
#ifdef UNICODE
                (LPARAM)fileNamet
#else
                (LPARAM)fileName
#endif
                );
#ifdef UNICODE
        wcstombs(fileName, fileNamet, MAX_PATH);
#endif

        /* start parsing */
        if ((err_num = editorconfig_parse(fileName, eh)) != 0 &&
                /* Ignore full path error, whose error code is
                 * EDITORCONFIG_PARSE_NOT_FULL_PATH */
                err_num != EDITORCONFIG_PARSE_NOT_FULL_PATH) {
            std::tstringstream err_msg;
            err_msg << TEXT("EditorConfig Error: ") << err_num;
            ::MessageBox(NULL, err_msg.str().c_str(),
                    TEXT("EditorConfig"), MB_OK);
            editorconfig_handle_destroy(eh);
            return;
        }

        // Get the current scintilla
        which = -1;
        ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0,
                (LPARAM)&which);
        if (which == -1)
            return;
        curScintilla = (which == 0) ? nppData._scintillaMainHandle :
            nppData._scintillaSecondHandle;

        struct
        {
            const char*     indent_style;
            int             indent_size;
            int             tab_width;
            const char*     end_of_line;
        } ecConf; // obtained EditorConfig settings will be here

        memset(&ecConf, 0, sizeof(ecConf));

        // apply the settings
        name_value_count = editorconfig_handle_get_name_value_count(eh);

        for (int i = 0; i < name_value_count; ++i) {
            const char* name;
            const char* value;

            editorconfig_handle_get_name_value(eh, i, &name, &value);

            if (!strcmp(name, "indent_style"))
                ecConf.indent_style = value;
            else if (!strcmp(name, "tab_width"))
                ecConf.tab_width = atoi(value);
            else if (!strcmp(name, "indent_size"))
                ecConf.indent_size = atoi(value);
            else if (!strcmp(name, "end_of_line"))
                ecConf.end_of_line = value;
        }

        if (ecConf.indent_style) {
            if (!strcmp(ecConf.indent_style, "tab"))
                ::SendMessage(curScintilla, SCI_SETUSETABS, (WPARAM)true, 0);
            else if (!strcmp(ecConf.indent_style, "space"))
                ::SendMessage(curScintilla, SCI_SETUSETABS, (WPARAM)false, 0);
        }
        if (ecConf.indent_size > 0) {
            ::SendMessage(curScintilla, SCI_SETINDENT,
                    (WPARAM)ecConf.indent_size, 0);

            // We set the tab width here, so that this could be overrided then
            // if ecConf.tab_wdith > 0
            ::SendMessage(curScintilla, SCI_SETTABWIDTH,
                    (WPARAM)ecConf.indent_size, 0);
        }

        if (ecConf.tab_width > 0)
            ::SendMessage(curScintilla, SCI_SETTABWIDTH,
                    (WPARAM)ecConf.tab_width, 0);

        // set eol
        if (ecConf.end_of_line) {
            if (!strcmp(ecConf.end_of_line, "lf"))
                    ::SendMessage(curScintilla, SCI_SETEOLMODE,
                        (WPARAM)SC_EOL_LF, 0);
            else if (!strcmp(ecConf.end_of_line, "cr"))
                    ::SendMessage(curScintilla, SCI_SETEOLMODE,
                        (WPARAM)SC_EOL_CR, 0);
            else if (!strcmp(ecConf.end_of_line, "crlf"))
                    ::SendMessage(curScintilla, SCI_SETEOLMODE,
                        (WPARAM)SC_EOL_CRLF, 0);
        }

        editorconfig_handle_destroy(eh);

        break;
    default:
        break;
    }
}


// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT message,
        WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
