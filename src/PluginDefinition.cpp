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

#include "PluginDefinition.hpp"
#include "menuCmdID.hpp"
#include "DlgAbout.hpp"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Handle to the Notepad++ instance
//
HINSTANCE hInst;

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit(HANDLE hModule)
{
    hInst = (HINSTANCE) hModule;
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Parse the EditorConfig file
//
static bool parseConfig(editorconfig_handle eh)
{
    TCHAR fileNamet[MAX_PATH];
    char  fileName[MAX_PATH];

    // get the file name
    ::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)fileNamet);
    wcstombs(fileName, fileNamet, MAX_PATH);

    // start parsing
    int err_num;
    if ((err_num = editorconfig_parse(fileName, eh)) != 0 &&
            /* Ignore full path error, whose error code is
             * EDITORCONFIG_PARSE_NOT_FULL_PATH */
            err_num != EDITORCONFIG_PARSE_NOT_FULL_PATH) {
        std::tstringstream err_msg;
        err_msg << TEXT("EditorConfig Error: ") << err_num;
        ::MessageBox(NULL, err_msg.str().c_str(),
                TEXT("EditorConfig"), MB_OK);
        editorconfig_handle_destroy(eh);
        return false;
    }
    return true;
}

//
// Get the current scintilla
//
static HWND getCurrentScintilla()
{
    int which = -1;
    SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM) &which);
    if (which == -1)
        return NULL;
    return((which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle);
}

void loadConfig()
{
    int name_value_count;
    editorconfig_handle eh;

    // Parse the EditorConfig file
    eh = editorconfig_handle_init();
    if (!parseConfig(eh))
        return;

    // Get the current scintilla
    HWND curScintilla = getCurrentScintilla();
    if (curScintilla == NULL)
        return;

    struct
    {
        const char*     indent_style;
#define INDENT_SIZE_TAB (-1000) // indent_size = -1000 means indent_size = tab
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
        else if (!strcmp(name, "indent_size")) {
            int     value_i = atoi(value);

            if (!strcmp(value, "tab"))
                ecConf.indent_size = INDENT_SIZE_TAB;
            else if (value_i > 0)
                ecConf.indent_size = value_i;
        }
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

    if (ecConf.indent_size == INDENT_SIZE_TAB)
        // set indent_size to tab_width here
        ::SendMessage(curScintilla, SCI_SETINDENT,
                (WPARAM)::SendMessage(curScintilla, SCI_GETTABWIDTH, 0, 0), 0);

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
}

//
// Is the char c a newline character?
//
static bool isNewline(char c)
{
    return c == '\n' || c == '\r';
}

//
// insert the final newline or remove any remaining newlines
//
void insertFinalNewline(bool insert)
{
    HWND curScintilla = getCurrentScintilla();
    if (curScintilla == NULL)
        return;

    // Get the last line number
    int lastLine = SendMessage(curScintilla, SCI_GETLINECOUNT, 0, 0) - 1;

    // Get the length of that last line
    int length = SendMessage(curScintilla, SCI_LINELENGTH, lastLine, 0);

    // Do we need to insert a final newline?
    if (insert)
    {
        // If there is an empty last line, we're done
        if (length == 0)
            return;

        // What type of EOL must be inserted?
        char eolBuf[10];
        eolBuf[0] = 0;
        int eolMode = SendMessage(curScintilla, SCI_GETEOLMODE, 0, 0);
        switch (eolMode)
        {
            case SC_EOL_CRLF:
                strcpy(eolBuf, "\r\n");
                break;
            case SC_EOL_CR:
                strcpy(eolBuf, "\r");
                break;
            case SC_EOL_LF:
                strcpy(eolBuf, "\n");
                break;
        }

        // Add the EOL at the end of the document
        length  = strlen(eolBuf);
        if (length != 0)
            SendMessage(curScintilla, SCI_APPENDTEXT, length, (LPARAM) eolBuf);

        return;
    }

    // Is there an non-empty last line, we're done
    if (length != 0)
        return;

    // Find the last non-newline character
    length = SendMessage(curScintilla, SCI_GETTEXTLENGTH, 0, 0);
    int pos = length;
    while (pos > 0 && isNewline((char) SendMessage(curScintilla, SCI_GETCHARAT, pos - 1, 0)))
        pos--;

    // Remove the final newline(s)
    SendMessage(curScintilla, SCI_DELETERANGE, pos, length - pos);
}

void onBeforeSave(HWND hWnd)
{
    editorconfig_handle eh = editorconfig_handle_init();

    if (!parseConfig(eh))
        return;

    int name_value_count = editorconfig_handle_get_name_value_count(eh);

    for (int i = 0; i < name_value_count; ++i) {
        const char* name;
        const char* value;

        editorconfig_handle_get_name_value(eh, i, &name, &value);

        if (strcmp(name, "trim_trailing_whitespace") == 0) {
            if (strcmp(value, "true") == 0) {
                SendMessage(hWnd, NPPM_MENUCOMMAND, 0, IDM_EDIT_TRIMTRAILING);
            }
            break;
        }

        if (strcmp(name, "insert_final_newline") == 0) {
            if (strcmp(value, "true") == 0)
                insertFinalNewline(true);
            if (strcmp(value, "false") == 0)
                insertFinalNewline(false);
            break;
        }
    }

    editorconfig_handle_destroy(eh);
}

void onReloadEditorConfig()
{
    loadConfig();
}
//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Reload EditorConfig for this file"),
            onReloadEditorConfig, NULL, false);

    // Separator
    setCommand(1, TEXT(""), NULL, NULL, false);

    setCommand(2, TEXT("About..."), showAboutDlg, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
    // Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
    if (index >= nbFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//
// Center the window, relative the NPP-window
//
void centerWindow(HWND hDlg)
{
    RECT rc;
    GetClientRect(nppData._nppHandle, &rc);

    POINT center;
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    center.x = rc.left + (w / 2);
    center.y = rc.top + (h / 2);
    ClientToScreen(nppData._nppHandle, &center);

    RECT dlgRect;
    GetClientRect(hDlg, &dlgRect);
    int x = center.x - ((dlgRect.right - dlgRect.left) / 2);
    int y = center.y - ((dlgRect.bottom - dlgRect.top) / 2);

    SetWindowPos(hDlg, HWND_TOP, x, y, -1, -1, SWP_NOSIZE | SWP_SHOWWINDOW);
}
