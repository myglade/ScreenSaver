
// ScreenSaverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenSaver.h"
#include "ScreenSaverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if 1
// CScreenSaverDlg dialog

CScreenSaverDlg::CScreenSaverDlg(CWnd* pParent /*=NULL*/)
    : CDHtmlDialog()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bPreview = FALSE;
    m_bGotCursorPos = FALSE;
    m_screensaver_mode = true;
}

void CScreenSaverDlg::DoDataExchange(CDataExchange* pDX)
{
    CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenSaverDlg, CDHtmlDialog)
    ON_WM_SETCURSOR()
    ON_WM_ACTIVATE()
    ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()


// CScreenSaverDlg message handlers

BOOL CScreenSaverDlg::OnInitDialog()
{
    CDHtmlDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    CRect r;
    GetBoundingRect(FALSE, NULL, r);
    MoveWindow(r);
    ::ShowCursor(FALSE);
  //     m_host = L"http://192.168.1.16:5000/index.html";
  //     m_host = L"http://www.google.com";
    this->Navigate(m_host);

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
        CDHtmlDialog::OnPaint();
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

        CWnd::GetDesktopWindow()->GetWindowRect(rRect);
    }

    return TRUE;
}


void CScreenSaverDlg::Close()
{
    if (!m_bPreview)
        DestroyWindow();
}


BOOL CScreenSaverDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        if (!m_screensaver_mode) {
            if (pMsg->wParam == VK_SHIFT) {
                ::ShowCursor(FALSE);
                SetCursor(NULL);
                m_screensaver_mode = true;
            }
        }
        else {
            if (pMsg->wParam == VK_CONTROL) {
                ::ShowCursor(TRUE);
                m_screensaver_mode = false;
            }
            else {
                Close();
                return TRUE;
            }
        }
    }
    else if (m_screensaver_mode) {
        if (pMsg->message == WM_MOUSEMOVE) {
            if (m_bGotCursorPos)
            {
                int nDeltaX = pMsg->pt.x - m_LastPos.x;
                if (nDeltaX < 0)
                    nDeltaX *= -1;
                int nDeltaY = pMsg->pt.y - m_LastPos.y;
                if (nDeltaY < 0)
                    nDeltaY *= -1;
                m_LastPos = pMsg->pt;

                if (nDeltaX + nDeltaY > 3) {
                    Close();
                    return TRUE;
                }
            }
            else
            {
                m_bGotCursorPos = TRUE;
                m_LastPos = pMsg->pt;
            }
        }
        if (pMsg->message == WM_LBUTTONDOWN
            || pMsg->message == WM_RBUTTONDOWN
            || pMsg->message == WM_MBUTTONDOWN)
        {
            Close();
            return TRUE;
        }
    }

    return CDHtmlDialog::PreTranslateMessage(pMsg);
}


void CScreenSaverDlg::PostNcDestroy()
{
    CDHtmlDialog::PostNcDestroy();
    delete this;
}


BOOL CScreenSaverDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bPreview)
    {
        //Let the base class do its thing
        return __super::OnSetCursor(pWnd, nHitTest, message);
    }
    else
    {
        SetCursor(NULL);
        return TRUE;
    }
}


void CScreenSaverDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    return;
    if (nState == WA_INACTIVE)
        Close();
    else
        __super::OnActivate(nState, pWndOther, bMinimized);
}


void CScreenSaverDlg::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
    return;
    if (!bActive)
        Close();
    else
        __super::OnActivateApp(bActive, dwThreadID);
}

#endif
