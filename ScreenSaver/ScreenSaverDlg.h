
// ScreenSaverDlg.h : header file
//

#pragma once


// CScreenSaverDlg dialog
class CScreenSaverDlg : public CDHtmlDialog
{
// Construction
public:
	CScreenSaverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENSAVER_DIALOG, IDH = IDR_HTML_SCREENSAVER_DIALOG };
#endif

    void Close();
    void SetPreviewFlag(BOOL bFlag) { m_bPreview = FALSE; }
    void SetHost(CString host) { m_host = host; }

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
    BOOL m_bPreview;
    BOOL m_bGotCursorPos;
    CPoint m_LastPos;
    CString m_host;

    BOOL GetBoundingRect(BOOL bChild, CWnd* pParentWnd, CRect& rRect);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void PostNcDestroy();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
};
