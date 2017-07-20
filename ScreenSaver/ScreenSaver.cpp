
// ScreenSaver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ScreenSaver.h"
#include "ScreenSaverDlg.h"
#include "Config.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString gSection = _T("screensaver");
CString gKey = _T("host");
CString gAppPath = _T("app_path");

CString default_host = _T("http://localhost:8888/");
CString default_appPath = _T("C:\\chromium\\bin\\");

// CScreenSaverApp

BEGIN_MESSAGE_MAP(CScreenSaverApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CScreenSaverApp construction

CScreenSaverApp::CScreenSaverApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CScreenSaverApp object

CScreenSaverApp theApp;


// CScreenSaverApp initialization

BOOL CScreenSaverApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();


    AfxEnableControlContainer();

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Heesung"));

    m_host = AfxGetApp()->GetProfileString(gSection, gKey, _T(""));
    if (m_host == "")
    {
        m_host = default_host;
        AfxGetApp()->WriteProfileString(gSection, gKey, m_host);
    }

    m_appPath = AfxGetApp()->GetProfileString(gSection, gAppPath, _T(""));
    if (m_appPath == "")
    {
        m_appPath = default_appPath;
        AfxGetApp()->WriteProfileString(gSection, gAppPath, m_appPath);
    }

    CScreenSaverCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    return ProcessShellCommand(cmdInfo);
}



int CScreenSaverApp::ExitInstance()
{
#ifndef _AFXDLL
    ControlBarCleanUp();
#endif

    delete m_pMainWnd;

    return CWinApp::ExitInstance();
}


BOOL CScreenSaverApp::ProcessShellCommand(CScreenSaverCommandLineInfo& rCmdInfo)
{
    switch (rCmdInfo.m_nShellCommand)
    {
    case CScreenSaverCommandLineInfo::ScreenSaverMode:
    {
        CScreenSaverDlg *dlg = new CScreenSaverDlg();
        m_pMainWnd = dlg;
        dlg->SetHost(m_host, m_appPath);
        dlg->SetPreviewFlag(FALSE);
        if (!dlg->Create(IDD_SCREENSAVER_DIALOG))
            return FALSE;

        return TRUE;
    }
    case CScreenSaverCommandLineInfo::PreviewMode:
    {
        CScreenSaverDlg *dlg = new CScreenSaverDlg();
        m_pMainWnd = dlg;
        dlg->SetHost(m_host, m_appPath);
        dlg->SetPreviewFlag(TRUE);

        CWnd wndParent;
        wndParent.Attach(rCmdInfo.m_hParentWnd);
        if (!dlg->Create(IDD_SCREENSAVER_DIALOG, &wndParent))
            return FALSE;

        wndParent.Detach();
        return TRUE;
    }
    case CScreenSaverCommandLineInfo::ShowConfigureDialog:
    {
        if (rCmdInfo.m_hParentWnd)
        {
            CWnd wndParent;
            wndParent.Attach(rCmdInfo.m_hParentWnd);
            ShowConfigureDialog(&wndParent);
            wndParent.Detach();
        }
        else
            ShowConfigureDialog(CWnd::GetActiveWindow());

        return FALSE;
    }
    default:
    {
        ASSERT(FALSE);
        break;
    }
    }
    return FALSE;
}

void CScreenSaverApp::ParseCommandLine(_In_ CScreenSaverCommandLineInfo& rCmdInfo)
{
    for (int i = 1; i < __argc; i++)
    {
#ifdef _UNICODE
        LPCTSTR pszParam = __wargv[i];
#else
        LPCTSTR pszParam = __argv[i];
#endif
        BOOL bFlag = FALSE;
        BOOL bLast = ((i + 1) == __argc);
        if (pszParam[0] == _T('-') || pszParam[0] == _T('/'))
        {
            //remove flag specifier
            bFlag = TRUE;
            ++pszParam;
        }
        rCmdInfo.ParseParam(pszParam, bFlag, bLast);
    }
}

void CScreenSaverApp::ShowConfigureDialog(CWnd *wnd)
{
    CConfig config(wnd);

    config.m_host = AfxGetApp()->GetProfileString(gSection, gKey, _T(""));
    if (config.m_host == "") {
        config.m_host = _T("http://192.168.1.15:5000/index.html");
        AfxGetApp()->WriteProfileString(gSection, gKey, config.m_host);
    }

    config.m_path = AfxGetApp()->GetProfileString(gSection, gAppPath, _T(""));
    if (config.m_path == "") {
        config.m_path = _T("C:\\chromium\\bin\\");
        AfxGetApp()->WriteProfileString(gSection, gAppPath, config.m_path);
    }
    
    if (config.DoModal() == IDOK)
    {
        if (config.m_host != "")
            AfxGetApp()->WriteProfileString(gSection, gKey, config.m_host);
        if (config.m_path != "") {
            if (config.m_path.Right(1) != "\\")
                config.m_path += "\\";

            AfxGetApp()->WriteProfileString(gSection, gAppPath, config.m_path);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
CScreenSaverCommandLineInfo::CScreenSaverCommandLineInfo() : m_nShellCommand(ShowConfigureDialog), m_hParentWnd(0)
{
}

void CScreenSaverCommandLineInfo::ParseParam(const TCHAR* pszParam, BOOL /*bFlag*/, BOOL /*bLast*/)
{
    if (_tcsicmp(pszParam, _T("c")) == 0)
        m_nShellCommand = ShowConfigureDialog;
    else if (_tcsicmp(pszParam, _T("p")) == 0)
        m_nShellCommand = PreviewMode;
    else if (_tcsicmp(pszParam, _T("s")) == 0)
        m_nShellCommand = ScreenSaverMode;
    else if ((_tcsstr(pszParam, _T("c:")) ||
        _tcsstr(pszParam, _T("C:"))) && _tcslen(pszParam) > 2) //Handle the undocumented format of "/c:HWND"
    {
        m_nShellCommand = ShowConfigureDialog;
#ifdef _WIN64
        m_hParentWnd = reinterpret_cast<HWND>(_ttoi64(pszParam + 2));
#else
#pragma warning(disable:4312)
        m_hParentWnd = reinterpret_cast<HWND>(_ttoi(pszParam + 2));
#pragma warning(default:4312)
#endif //#ifdef _WIN64
    }
    else
    {
        //Convert the parameter to a window handle
#ifdef _WIN64
        m_hParentWnd = reinterpret_cast<HWND>(_ttoi64(pszParam));
#else
#pragma warning(disable:4312)
        m_hParentWnd = reinterpret_cast<HWND>(_ttoi(pszParam));
#pragma warning(default:4312)
#endif //#ifdef _WIN64
    }
}

