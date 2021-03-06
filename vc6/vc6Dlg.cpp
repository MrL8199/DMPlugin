// vc6Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "vc6.h"
#include "vc6Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVc6Dlg dialog

CVc6Dlg::CVc6Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVc6Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVc6Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVc6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVc6Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVc6Dlg, CDialog)
	//{{AFX_MSG_MAP(CVc6Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVc6Dlg message handlers

BOOL CVc6Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	// 更改标题
	CString title;
	GetWindowText(title);
	title += "  当前插件版本号<";
	title += dm_ver;
	title += ">";
	SetWindowText(title);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVc6Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVc6Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVc6Dlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CVc6Dlg::OnButton1() 
{
	// 启动线程
	ScriptInfo *script_info = StartOneScript();

	if (script_info)
	{
		all_script_info.push_back(script_info);
	}
}

void CVc6Dlg::OnButton2() 
{
	// 等待线程结束,千万不可强制结束.比如调用TerminateThread ExitThread _endscriptex等接口. 
	// 必须自然等待结束. 否则带来的后果就是资源泄漏,导致窗口崩溃
	StopAllScript(all_script_info);
}

void CVc6Dlg::OnOK() 
{
	CDialog::OnOK();
}

LRESULT CVc6Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_CLOSE)
    {
        EndDialog(0);
    }
	return CDialog::WindowProc(message, wParam, lParam);
}

void CVc6Dlg::OnDestroy() 
{
	// 等待所有线程结束
	StopAllScript(all_script_info);
	CDialog::OnDestroy();
}
