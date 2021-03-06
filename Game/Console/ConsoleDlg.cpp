
// ConsoleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "ConsoleDlg.h"
#include "afxdialogex.h"
#include "MainDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConsoleDlg 对话框



CConsoleDlg::CConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONSOLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CConsoleDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CConsoleDlg::OnBnClickedButtonRegister)
END_MESSAGE_MAP()


// CConsoleDlg 消息处理程序

BOOL CConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConsoleDlg::OnOK()
{
	// TODO: 屏蔽掉系统的ONOK函数

	//CDialogEx::OnOK();
}

void CConsoleDlg::OnShow()
{
	VMProtectBegin("OnShow");
	CDialogEx::OnOK();

	CMainDialog dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
	VMProtectEnd();
}


void CConsoleDlg::OnBnClickedButtonLogin()
{
	VMProtectBegin("OnBnClickedButtonLogin");
	// TODO: 登录
	char szAcc[100];
	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(szAcc, sizeof(szAcc));
	char szPwd[100];
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(szPwd, sizeof(szPwd));

	auto ret = pAuth->UserAuth(_bstr_t(szAcc), _bstr_t(szPwd));

	switch (ret)
	{
	case 0:
		((CConsoleDlg*)(theApp.m_pMainWnd))->OnShow();
		break;
	case -1:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("注册码不存在");
		break;
	case -2:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("注册码被禁用");
		break;
	case -3:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("绑定机器超限");
		break;
	case -4:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("注册码已在线");
		break;
	case -5:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("已过期");
		break;
	case -6:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("用户余额不足");
		break;
		break;
	}
	VMProtectEnd();
}


void CConsoleDlg::OnBnClickedButtonRegister()
{
	// TODO: 注册
}


BOOL CConsoleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
