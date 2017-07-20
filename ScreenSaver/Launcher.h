#pragma once

class CLauncher
{
public:
    CLauncher();
    ~CLauncher();

    static CLauncher & Instance() {
        static CLauncher object;

        return object;
    }

    void InstallHook();
    void UninstallHook();
    BOOL Start(CWnd *parent, CString app, CString host);
    void Stop();

    void StopPrevInstance();
    

    HHOOK m_mousehook = NULL;
    HHOOK m_kbdhook = NULL;
    HANDLE m_process = NULL;
    HANDLE m_thread = NULL;
    BOOL m_screensaver_mode = TRUE;
    CString m_host;
    CString m_app;
    CWnd *m_parent;

    CPoint m_LastPos;

};

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam); //callback declaration
LRESULT WINAPI MyKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam); //callback declaration

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644960(v=vs.85).aspx#installing_releasing
// https://www.unknowncheats.me/wiki/C%2B%2B:WindowsHookEx_Mouse
