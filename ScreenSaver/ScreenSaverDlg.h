
// ScreenSaverDlg.h : header file
//

#pragma once


// CScreenSaverDlg dialog
class CScreenSaverDlg : public CDialog
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
    void SetHost(CString host, CString appPath) { m_host = host; m_appPath = appPath; }

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
    BOOL m_bPreview;
    CString m_host;
    CString m_appPath;

    BOOL GetBoundingRect(BOOL bChild, CWnd* pParentWnd, CRect& rRect);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    virtual void PostNcDestroy();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
