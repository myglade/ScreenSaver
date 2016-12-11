// Config.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenSaver.h"
#include "Config.h"
#include "afxdialogex.h"


// CConfig dialog

IMPLEMENT_DYNAMIC(CConfig, CDialogEx)

CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
    , m_host(_T(""))
{

}

CConfig::~CConfig()
{
}

void CConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_host);
}


BEGIN_MESSAGE_MAP(CConfig, CDialogEx)
END_MESSAGE_MAP()


// CConfig message handlers
