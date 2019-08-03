
// MFCApplication2.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2App

BEGIN_MESSAGE_MAP(CMFCApplication2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCApplication2App 생성

CMFCApplication2App::CMFCApplication2App()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CMFCApplication2App 개체입니다.

CMFCApplication2App theApp;


// CMFCApplication2App 초기화

BOOL CMFCApplication2App::InitInstance()
{
	CWinApp::InitInstance();

	CMFCApplication2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	return FALSE;
}

