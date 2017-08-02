#include "stdafx.h"
#include "ScreenSaver.h"
#include "Launcher.h"
#include "ScreenSaverDlg.h"
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

using namespace std;

extern CString gSection;
extern CString gWindowMode;

CLauncher::CLauncher()
{
    m_app = L"C:\\chromium\\bin\\chrome.exe";
}


CLauncher::~CLauncher()
{
    Stop();
}

void CLauncher::InstallHook()
{
    /*
    SetWindowHookEx(
    WM_MOUSE_LL = mouse low level hook type,
    MyMouseCallback = our callback function that will deal with system messages about mouse
    NULL, 0);

    c++ note: we can check the return SetWindowsHookEx like this because:
    If it return NULL, a NULL value is 0 and 0 is false.
    */
    if (!(m_mousehook = SetWindowsHookEx(WH_MOUSE_LL, MyMouseCallback, NULL, 0))) {
        TRACE("Error: %x \n", GetLastError());
    }

    if (!(m_kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyboardCallback, NULL, 0))) {
        TRACE("Error: %x \n", GetLastError());
    }
}

void CLauncher::UninstallHook()
{
    if (m_mousehook)
        UnhookWindowsHookEx(m_mousehook);
    if (m_kbdhook)
        UnhookWindowsHookEx(m_kbdhook);
}

// https://www.unknowncheats.me/wiki/C%2B%2B:WindowsHookEx_Mouse

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    MSLLHOOKSTRUCT * pMouseStruct = (MSLLHOOKSTRUCT *)lParam; 

    if (nCode == 0 && CLauncher::Instance().m_screensaver_mode) { 
        switch (wParam) {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            TRACE("mouse down. Close\n");
            CLauncher::Instance().Stop();
            break;
        case WM_MOUSEMOVE:
            if (CLauncher::Instance().m_LastPos.x == INT_MIN)
                CLauncher::Instance().m_LastPos = pMouseStruct->pt;
            else {
                int nDeltaX = pMouseStruct->pt.x - CLauncher::Instance().m_LastPos.x;
                if (nDeltaX < 0)
                    nDeltaX *= -1;
                int nDeltaY = pMouseStruct->pt.y - CLauncher::Instance().m_LastPos.y;
                if (nDeltaY < 0)
                    nDeltaY *= -1;
                CLauncher::Instance().m_LastPos = pMouseStruct->pt;

                if (nDeltaX + nDeltaY > 3) {
                    TRACE("mouse move. Close\n");
                    CLauncher::Instance().Stop();
                }
            }
            break;
        }

    }

    return CallNextHookEx(CLauncher::Instance().m_mousehook, nCode, wParam, lParam);
}

LRESULT WINAPI MyKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT * pKbdStruct = (KBDLLHOOKSTRUCT *)lParam;

    if (nCode != 0 || wParam != WM_KEYDOWN) {
        return CallNextHookEx(CLauncher::Instance().m_kbdhook, nCode, wParam, lParam);
    }

    if (!CLauncher::Instance().m_screensaver_mode) {
        if (pKbdStruct->vkCode == VK_LSHIFT || pKbdStruct->vkCode == VK_RSHIFT) {
            ::ShowCursor(FALSE);
            SetCursor(NULL);
            CLauncher::Instance().m_screensaver_mode = TRUE;
            TRACE("VK_SHIFT\n");
        }
    }
    else {
        // screensaver mode
        if (pKbdStruct->vkCode == VK_LCONTROL || pKbdStruct->vkCode == VK_RCONTROL) {
            ::ShowCursor(TRUE);
            CLauncher::Instance().m_screensaver_mode = FALSE;
            TRACE("VK_CONTROL\n");
        }
        else {
            CLauncher::Instance().Stop();
            TRACE("Keyboard Stop\n");
        }
    }

    return CallNextHookEx(CLauncher::Instance().m_kbdhook, nCode, wParam, lParam);
}

BOOL CLauncher::Start(CWnd *parent, CString app, CString host)
{
    m_parent = parent;
    m_host = host;
    m_app = app;

    ::ShowCursor(FALSE);
    SetCursor(NULL);

    StopPrevInstance();
    InstallHook();

    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;
    CString s = L"C:\\chromium\\bin\\chrome.exe --kiosk http://www.apple.com";


    int window_mode = AfxGetApp()->GetProfileInt(gSection, gWindowMode, 0);

    // --incognito
    // https://peter.sh/experiments/chromium-command-line-switches/
 //   s.Format(L"%schrome.exe --kiosk --no-default-browser-check --incognito %s", m_app, m_host);
    
    if (window_mode == 0)
        s.Format(L"%schrome.exe --kiosk --no-default-browser-check  %s", m_app, m_host);
    else
        s.Format(L"%schrome.exe --no-default-browser-check  %s", m_app, m_host);

    if (!::CreateProcess(NULL,
            (LPWSTR)(LPCTSTR)s,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &info,
            &processInfo)) {

        return FALSE;
    }

    m_process = processInfo.hProcess;
    m_thread = processInfo.hThread;
    m_screensaver_mode = TRUE;

    return TRUE;
}

void CLauncher::HackChrome()
{
    TCHAR szPath[MAX_PATH];

    SHGetFolderPath(NULL,
        CSIDL_LOCAL_APPDATA,
        NULL,
        0,
        szPath);
    CString path = szPath;

    path += L"\\Chromium\\User Data\\Default\\Preferences";

    {
        ifstream in(path);

        if (!in.is_open())
            return;

        std::string str((std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>());
        

        string normal = "\"exit_type\":\"Normal\"";
        string crash = "\"exit_type\":\"Crashed\"";

        auto pos = str.find(crash);

        if (pos == string::npos) {
            in.close();
            return;
        }

        string str2 = str.replace(pos, crash.size(), normal);

        in.close();

        CString path2 = path + "2";
        ofstream out(path);

        out << str2;
        out.close();
    }
}

void CLauncher::Stop()
{
    UninstallHook();

    if (m_process == NULL)
        return;

    StopPrevInstance(); 
    Sleep(500);
    TerminateProcess(m_process, 0);
    CloseHandle(m_process);
    CloseHandle(m_thread);

   // HackChrome();

    m_thread = NULL;
    m_process = NULL;
    ((CScreenSaverDlg *)m_parent)->Close();
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TCHAR title[100], cls[100];


    //    wnd.Attach(hwnd);
    GetWindowText(hwnd, title, 100);
    GetClassName(hwnd, cls, 100);

    CString className = cls;
    CString titleName = title;
    CString name = L"Chromium";

    titleName = titleName.Right(name.GetLength());

    if (className == L"Chrome_WidgetWin_1" && titleName == name)
        SendMessage(hwnd, WM_CLOSE, 0, 0);

    return TRUE;
}

void CLauncher::StopPrevInstance()
{
    EnumWindows(EnumWindowsProc, 0);
}