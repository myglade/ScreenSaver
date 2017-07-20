
// ScreenSaver.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CScreenSaverCommandLineInfo : public CCommandLineInfo
{
public:
    //Constructors / Destructors
    CScreenSaverCommandLineInfo();

    //Methods
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

    //Enums
    enum ShellCommand
    {
        ScreenSaverMode,
        PreviewMode,
        ShowConfigureDialog,
    };

    //Member variables
    ShellCommand m_nShellCommand;
    HWND         m_hParentWnd;
};


// CScreenSaverApp:
// See ScreenSaver.cpp for the implementation of this class
//

class CScreenSaverApp : public CWinApp
{
public:
	CScreenSaverApp();

// Overrides
public:
	virtual BOOL InitInstance();

    CString m_host;
    CString m_appPath;

// Implementation
    virtual void ParseCommandLine(CScreenSaverCommandLineInfo& rCmdInfo);
    virtual BOOL ProcessShellCommand(CScreenSaverCommandLineInfo& rCmdInfo);
    virtual void ShowConfigureDialog(CWnd* pParentWnd);
    
	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CScreenSaverApp theApp;