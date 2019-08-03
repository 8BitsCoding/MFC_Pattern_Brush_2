
// MFCApplication2Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2Dlg 대화 상자



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CMFCApplication2Dlg 메시지 처리기

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_normal_image.Load(L"normal_image.png");
	m_small_image.Load(L"small_image.png");

	CRect win_rect, client_rect;
	GetWindowRect(win_rect);
	GetClientRect(client_rect);

	CBitmap * p_bmp = CBitmap::FromHandle(m_normal_image);
	m_normal_brush.CreatePatternBrush(p_bmp);

	p_bmp = CBitmap::FromHandle(m_small_image);
	m_small_brush.CreatePatternBrush(p_bmp);

	int w_fix = win_rect.Width() - client_rect.Width();
	int h_fix = win_rect.Height() - client_rect.Height();

	SetWindowPos(NULL, 0, 0, m_small_image.GetWidth()+ w_fix, m_small_image.GetHeight()+ h_fix, SWP_NOMOVE);

	SetBackgroundColor(RGB(0, 0, 0));
	m_mem_view.Create(m_small_image.GetWidth(), m_small_image.GetHeight(), 32);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication2Dlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// m_small_image.Draw(dc, 0, 0);
		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	CDC * p_dc = CDC::FromHandle(m_mem_view.GetDC());

	p_dc->FillSolidRect(0, 0, m_small_image.GetWidth(), m_small_image.GetHeight(), RGB(0, 0, 0));
	m_small_image.AlphaBlend(*p_dc, 0, 0, m_alpha);

	CBrush * p_old_brush;

	if (m_is_clicked) {
		p_dc->SetBrushOrg(point.x * (-1), point.y * (-1));
		p_old_brush = p_dc->SelectObject(&m_normal_brush);
	}
	else {
		p_dc->SetBrushOrg(0, 0);
		p_old_brush = p_dc->SelectObject(&m_small_brush);
	}
	
	p_dc->Ellipse(point.x - m_radius, point.y - m_radius, point.x + m_radius, point.y + m_radius);
	p_dc->SelectObject(p_old_brush);

	if (m_is_clicked) {
		CPen grid_pen(PS_DOT, 1, RGB(160, 160, 160));
		CPen * p_old_pen = p_dc->SelectObject(&grid_pen);
		p_dc->SetBkMode(TRANSPARENT);
		p_dc->MoveTo(point.x, point.y - m_radius);
		p_dc->LineTo(point.x, point.y + m_radius);

		p_dc->MoveTo(point.x - m_radius, point.y);
		p_dc->LineTo(point.x + m_radius, point.y);

		p_dc->SelectObject(p_old_pen);
	}

	m_mem_view.ReleaseDC();
	m_mem_view.Draw(dc, 0, 0);
	// CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCApplication2Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_is_clicked = 1;
	SetCapture();
	OnMouseMove(nFlags, point);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCApplication2Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_is_clicked = 0;
	ReleaseCapture();
	OnMouseMove(nFlags, point);

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCApplication2Dlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CMFCApplication2Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int old_radius = m_radius;
	if (zDelta > 0) {
		// 휠을 미는 것
		if(m_radius >= 20) m_radius -= 10;

		if (nFlags & MK_CONTROL) {
			m_alpha -= 10;
			if (m_alpha <= 0) m_alpha = 0;
		}
	}
	else {
		// 휠을 당기는 것
		if(m_radius < 100) m_radius += 10;

		if (nFlags & MK_CONTROL) {
			m_alpha += 10;
			if (m_alpha >= 255) m_alpha = 255;
		}
	}
	if (old_radius != m_radius) {
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(&pos);

		OnMouseMove(0, pos);
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
