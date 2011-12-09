#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <windows.h>
#include "Scintilla.h"
#include "Notepad_plus_msgs.h"

const int nbChar = 64;

typedef const TCHAR * (__cdecl * PFUNCGETNAME)();

struct NppData {
    HWND _nppHandle;
    HWND _scintillaMainHandle;
    HWND _scintillaSecondHandle;
};

typedef void (__cdecl * PFUNCSETINFO)(NppData);
typedef void (__cdecl * PFUNCPLUGINCMD)();
typedef void (__cdecl * PBENOTIFIED)(SCNotification *);
typedef LRESULT (__cdecl * PMESSAGEPROC)(UINT Message, WPARAM wParam, LPARAM lParam);


struct ShortcutKey {
    bool _isCtrl;
    bool _isAlt;
    bool _isShift;
    UCHAR _key;
};

struct FuncItem {
    TCHAR _itemName[nbChar];
    PFUNCPLUGINCMD _pFunc;
    int _cmdID;
    bool _init2Check;
    ShortcutKey *_pShKey;
};

typedef FuncItem * (__cdecl * PFUNCGETFUNCSARRAY)(int *);

// You should implement (or define an empty function body) those functions which are called by Notepad++ plugin manager
extern "C" __declspec(dllexport) void setInfo(NppData);
extern "C" __declspec(dllexport) const TCHAR * getName();
extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *);
extern "C" __declspec(dllexport) void beNotified(SCNotification *);
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam);

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode();
#endif //UNICODE

#endif //PLUGININTERFACE_H
