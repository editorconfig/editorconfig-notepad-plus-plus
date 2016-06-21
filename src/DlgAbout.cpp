// this file is part of EditorConfig plugin for Notepad++
//
// Copyright (C) 2011-2016 EditorConfig Team <http://editorconfig.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <commctrl.h>
#include "PluginDefinition.hpp"
#include "Resource.hpp"

//
// Initialize the dialog
//
static BOOL onInitDialog(HWND hDlg)
{
    centerWindow(hDlg);

    // Get the version of the Core-C library
    int major, minor, subminor;
    editorconfig_get_version(&major, &minor, &subminor);

    // And put in the dialog
    TCHAR version[MAX_PATH];
    swprintf(version, MAX_PATH, TEXT("%d.%d.%d"), major, minor, subminor);
    SetDlgItemText(hDlg, IDC_CORE_VERSION, version);

    // Let windows set focus
    return TRUE;
}

//
// Handle all the messages for the dialog
//
static BOOL CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
            return onInitDialog(hDlg);
        }
        case WM_NOTIFY:
        {
            switch (((LPNMHDR)lParam)->code)
            {
                case NM_CLICK:
                case NM_RETURN:
                {
                    PNMLINK pNMLink = (PNMLINK) lParam;
                    LITEM item = pNMLink->item;
                    ShellExecute(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOW);
                }
            }
            return FALSE;
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCANCEL:
                {
                    EndDialog(hDlg, 0);
                    return TRUE;
                }
            }
            return FALSE;
        }
    }
    return FALSE;
}

//
// Show the About Dialog
//
void showAboutDlg()
{
    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nppData._nppHandle, (DLGPROC) dlgProc);
}
