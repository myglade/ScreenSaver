
// ScreenSaverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenSaver.h"
#include "ScreenSaverDlg.h"
#include "afxdialogex.h"
#include "Launcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScreenSaverDlg dialog

CScreenSaverDlg::CScreenSaverDlg(CWnd* pParent /*=NULL*/)
	: CDialog()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bPreview = FALSE;
}

void CScreenSaverDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenSaverDlg, CDialog)
END_MESSAGE_MAP()


// CScreenSaverDlg message handlers


BOOL CScreenSaverDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect r(0, 0, 10, 10);
    //GetBoundingRect(FALSE, NULL, r);
    MoveWindow(r);
 //   m_host = L"http://localhost:5000/index.html";
 //   m_host = L"http://www.apple.com";
//    this->Navigate(m_host);

 //   CString app = L"C:\\chromium\\bin\\chrome.exe";
//    CString host = L"http://www.apple.com";

    CLauncher::Instance().Start(this, m_appPath, m_host);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScreenSaverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScreenSaverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CScreenSaverDlg::GetBoundingRect(BOOL bChild, CWnd* pParentWnd, CRect& rRect)
{
    rRect.SetRectEmpty();

    if (bChild)
    {
        if ((pParentWnd == NULL) || !pParentWnd->GetSafeHwnd())
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }
        pParentWnd->GetClientRect(&rRect);
    }
    else
    {
        //Get the bounding rect of the screen, This
        //will also handle the case of multiple monitors
        HDC hScreenDC = ::GetDC(NULL);
        ::GetClipBox(hScreenDC, rRect);
        ::ReleaseDC(NULL, hScreenDC);
    }

    return TRUE;
}


void CScreenSaverDlg::Close()
{
    if (!m_bPreview)
        DestroyWindow();
}


void CScreenSaverDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();
    delete this;
}
