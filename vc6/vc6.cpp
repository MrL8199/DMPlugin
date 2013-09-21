// vc6.cpp : Defines the class behaviors for the application.
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
// CVc6App

BEGIN_MESSAGE_MAP(CVc6App, CWinApp)
	//{{AFX_MSG_MAP(CVc6App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVc6App construction

CVc6App::CVc6App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVc6App object

CVc6App theApp;

static BOOL RegDll(LPCTSTR file) 
{
	HMODULE hLib = LoadLibrary(file); 
	if (hLib < (HMODULE)HINSTANCE_ERROR) 	
	{ 
		return FALSE;
	} 
	

	HRESULT hResult = ::OleInitialize(NULL);
	if(hResult != S_OK)
	{
		FreeLibrary(hLib);
		return FALSE;
	}



	FARPROC lpDllEntryPoint = GetProcAddress(hLib,"DllRegisterServer"); 
	if (lpDllEntryPoint == NULL)
	{
		FreeLibrary(hLib);
		::OleUninitialize();
		return FALSE;
	}
	
	
	if(FAILED((*lpDllEntryPoint)()))
	{ 
		FreeLibrary(hLib);
		::OleUninitialize();
		return FALSE;
		
	}; 
	
	::OleUninitialize();
	FreeLibrary(hLib);
	
	return TRUE;
}


// �ж�Ŀ¼�Ƿ����
static BOOL FolderExists(CString s)
{
    DWORD attr; 
    attr = GetFileAttributes(s); 
    return (attr != (DWORD)(-1) ) &&
        ( attr & FILE_ATTRIBUTE_DIRECTORY); 
}

// ����Ŀ¼
static BOOL CreateDir(CString P)
{
    int len=P.GetLength();
    if ( len <2 ) return false; 
    
    if('\\'==P[len-1])
    {
        P=P.Left(len-1);
        len=P.GetLength();
    }
    if ( len <=0 ) return false;
    
    if (len <=3) 
    {
        if (FolderExists(P))return true;
        else return false; 
    }
    
    if (FolderExists(P))return true;
 
    
    
    CString Parent;
    Parent=P.Left(P.ReverseFind('\\') );
    
    if(Parent.GetLength()<=0)return false; 
    
    BOOL Ret=CreateDir(Parent); 
    
    if(Ret) 
    {
        SECURITY_ATTRIBUTES sa;
        sa.nLength=sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor=NULL;
        sa.bInheritHandle=0;
        Ret=(CreateDirectory(P,&sa)==TRUE);
        return Ret;
    }
    else
        return FALSE;
}


static BOOL ExtraceRes(DWORD res_id,char * path)
{
	HINSTANCE instance = AfxGetInstanceHandle();
	
    //��λ���ǵ��Զ�����Դ��������Ϊ�����Ǵӱ�ģ�鶨λ��Դ�����Խ�����򵥵���ΪNULL����
	HRSRC hRsrc = FindResource(instance, MAKEINTRESOURCE(res_id), TEXT("Res"));
	if (NULL == hRsrc)
		return FALSE;

	//��ȡ��Դ�Ĵ�С
	DWORD dwSize = SizeofResource(instance, hRsrc); 
	if (0 == dwSize)
		return FALSE;

	//������Դ
	HGLOBAL hGlobal = LoadResource(instance, hRsrc); 
	if (NULL == hGlobal)
		return FALSE;
    
	//������Դ
	LPVOID pBuffer = LockResource(hGlobal); 
	if (NULL == pBuffer)
		return FALSE;

    FILE * f = fopen(path,"wb");
    if (f == NULL)
        return FALSE;
    
	fwrite(pBuffer,1,dwSize,f);

    fclose(f);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CVc6App initialization

BOOL CVc6App::InitInstance()
{
	AfxEnableControlContainer();
	
	// ����1 ����COM����߳�ģ��Ϊsta
	CoInitialize(NULL);
	
	// ����2 ����������ͷ���Դ�Ķ���. �����������Դ���ͷŵ�c:\test_game.
	// �������Ѿ���dm.dll��ӵ��˸�����
	// ���ǰ�dm.dll���ͷŵ�c:\test_game,������Դ�ͷ�����.
	// ����Ҫע�������ӵ���Դ��С����������Res. �����޷��ͷ�.
	CreateDir("c:\\test_game");
	ExtraceRes(IDR_RES_DM,"c:\\test_game\\dm.dll");

	
	// ����3 ע��dm�����ϵͳ��.
	char * dll_path = "c:\\test_game\\dm.dll";
	if (RegDll(dll_path) == FALSE)
	{
		MessageBox(NULL,"ע����ʧ��. ����ļ��Ƿ����","����",MB_OK);
		return FALSE;
	}

	//  ����4 ���汾�� �����еĲ���汾����3.1208C. ���Ҫ������������滻Res\dm.dll
	//  ����dm.hpp��dm.cpp����Ҫɾ�����µ���. ͬʱdm.hpp���ļ�ͷ������#pragma once
	char * need_ver = "3.1220";
	Idmsoft dm;
	if (dm.CreateDispatch("dm.dmsoft") == FALSE)
	{
		MessageBox(NULL,"��������ʧ��,����Ƿ�����ע��","����",MB_OK);
		return FALSE;
	}

	if (strcmp(need_ver,dm.Ver()) != 0)
	{
		CString tmp;
		tmp.Format("�汾���󣬵�ǰ�汾����:%s",dm.Ver());
		MessageBox(NULL,tmp,"����",MB_OK);
		return FALSE;
	}
#if 0
	// ����5 �շ��û�ע��. �����ʹ����ѹ���,�����Թ���һ��
	long ret = dm.Reg("abcde","");
	if (ret != 1)
	{
		CString tmp;
		tmp.Format("ע��ʧ��,����ֵ��:%d",ret);
		MessageBox(NULL,tmp,"����",MB_OK);
		return FALSE;
	}
#endif

#if 0
	// ����6 �����ʹ��DmGuard����, ���������￪��.���ûʹ�õ�������,�����Թ���һ��
	if (dm.DmGuard(1,"xxx") == 0)
	{
		MessageBox(NULL,"����������ʧ��","����",MB_OK);
		return FALSE;
	}
#endif
	// OK ���ˣ�����Ѿ����������ĵ�����. ����,����δ����߳���ʹ�ò���ľ������.
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CVc6Dlg dlg;
	dlg.dm_ver = dm.Ver();
	
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
