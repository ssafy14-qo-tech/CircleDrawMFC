
// CircleDrawMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CircleDrawMFC.h"
#include "CircleDrawMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CCircleDrawMFCDlg 대화 상자



CCircleDrawMFCDlg::CCircleDrawMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLEDRAWMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCircleDrawMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircleDrawMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CCircleDrawMFCDlg::OnResetButton)
	ON_EN_CHANGE(IDC_EDIT5, &CCircleDrawMFCDlg::OnNumberChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &CCircleDrawMFCDlg::OnNumberChangeEdit)
END_MESSAGE_MAP()


// CCircleDrawMFCDlg 메시지 처리기

BOOL CCircleDrawMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (!m_image.IsNull())
	{
		m_image.Destroy();
	}
	m_pointCount = 0;
	m_image.Create(640, 480, 24);
	SetDlgItemInt(IDC_EDIT5, m_radius);
	SetDlgItemInt(IDC_EDIT6, m_outline);
	m_ctrlIds = { IDC_EDIT2 , IDC_EDIT3 , IDC_EDIT4 };
	for (auto ctrlId : m_ctrlIds) SetTextFormatPoint(ctrlId, CPoint{}, true);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCircleDrawMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCircleDrawMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		//CDialogEx::OnPaint();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		if (m_image.IsNull() || fm == nullptr) return;

		int nWidth = m_image.GetWidth();
		int nHeight = m_image.GetHeight();
		int nPitch = m_image.GetPitch();
		int nBpp = m_image.GetBPP();

		unsigned char* pTop = fm + (nHeight - 1) * nPitch;
		memset(pTop, 0xff, abs(nPitch) * nHeight);

		CPaintDC dc(this);

		auto circle = GetCircle();
		if (circle.second > 0)
		{
			auto center = circle.first;
			int radius = circle.second;
			// 정원은 radius가 유지
			DrawCirclePointToImage(center, radius + m_outline, m_outline);
		}

		// 점 원 그리기
		for (auto& pointData : m_points)
		{
			CPoint center = pointData.center;
			int radius = m_radius;
			DrawCirclePointToImage(center, radius);
		}
		m_image.Draw(dc, 0, 0);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircleDrawMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CCircleDrawMFCDlg::IsCircleBounded(CPoint point)
{
	int idx = 0;
	for (const auto& pointData : m_points)
	{
		int minX = pointData.center.x - m_radius;
		int maxX = pointData.center.x + m_radius;
		int minY = pointData.center.y - m_radius;
		int maxY = pointData.center.y + m_radius;
		if (minX <= point.x && point.x <= maxX && minY <= point.y && point.y <= maxY)
		{
			return idx;
		}
		++idx;
	}
	return -1;
}

bool CCircleDrawMFCDlg::IsInCircle(CPoint pointA, CPoint pointB, int radius)
{
	CPoint d = pointA - pointB;
	if ((d.x * d.x) + (d.y * d.y) <= (radius * radius)) {
		return true;
	}
	return false;
}

void CCircleDrawMFCDlg::DrawCirclePointToImage(CPoint point, int radius, int outline)
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int width = m_image.GetWidth();
	int height = m_image.GetHeight();
	int pitch = m_image.GetPitch();
	int nBpp = m_image.GetBPP();
	int x1 = point.x - radius;
	int y1 = point.y - radius;
	int x2 = point.x + radius;
	int y2 = point.y + radius;
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 > width) x2 = width;
	if (y2 > height) y2 = height;
	
	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			bool check = IsInCircle(point, CPoint{ j, i }, radius);
			if (outline < 0 && check)
			{
				if (nBpp == 24)
				{
					unsigned char* pPixel = fm + (i * pitch) + (j * 3);
					pPixel[0] = 0; // Blue
					pPixel[1] = 0; // Green
					pPixel[2] = 0; // Red
				}
			}
			else if (outline > 0 && check && !IsInCircle(point, CPoint{ j, i }, radius - outline))
			{
				if (nBpp == 24)
				{
					unsigned char* pPixel = fm + (i * pitch) + (j * 3);
					pPixel[0] = 0; // Blue
					pPixel[1] = 0; // Green
					pPixel[2] = 0; // Red
				}
			}
		}
	}
}

std::pair<CPoint, int> CCircleDrawMFCDlg::GetCircle()
{
	std::pair<CPoint, int> result{ {-1, -1}, -1 };
	if (m_pointCount != 3)
	{
		return result;
	}

	int x1 = m_points[0].center.x;
	int y1 = m_points[0].center.y;
	int x2 = m_points[1].center.x;
	int y2 = m_points[1].center.y;
	int x3 = m_points[2].center.x;
	int y3 = m_points[2].center.y;

	// 크래머 공식

	// 분모
	double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	if (D != 0) {
		double s1 = x1 * x1 + y1 * y1;
		double s2 = x2 * x2 + y2 * y2;
		double s3 = x3 * x3 + y3 * y3;

		double cx = (s1 * (y2 - y3) + s2 * (y3 - y1) + s3 * (y1 - y2)) / D;
		double cy = (s1 * (x3 - x2) + s2 * (x1 - x3) + s3 * (x2 - x1)) / D;

		// 반지름
		double radius = sqrt((cx - x1) * (cx - x1) + (cy - y1) * (cy - y1));
		
		result.first.x = cx;
		result.first.y = cy;
		result.second = radius;
	}

	return result;
}

void CCircleDrawMFCDlg::SetTextFormatPoint(int ctrlID, CPoint point, bool reset)
{
	CString text;
	if (reset)
	{
		text.Format(_T("---"));
		SetDlgItemText(ctrlID, text);
	}
	else
	{
		text.Format(_T("x : %d, y : %d"), point.x, point.y);
		SetDlgItemText(ctrlID, text);
	}
}

void CCircleDrawMFCDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_DELETE:
	{
		m_points.erase(std::remove_if(m_points.begin(), m_points.end(), [&circleCountRef = m_pointCount](PointData& point) {
			if (point.bSelected == true)
			{
				--circleCountRef;
				return true;
			}
			return false;
		}), m_points.end());

		// 렌더링 갱신
		Invalidate();
	}
	break;
	}

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CCircleDrawMFCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	TRACE(_T("마우스 클릭됨! X: %d, Y: %d\n"), point.x, point.y);

	// 선택된거 초기화
	m_selectIdx = -1;
	for (auto& pointData : m_points)
	{
		pointData.bSelected = false;
	}

	// 기존 원 클릭 참고
	int selectIdx = IsCircleBounded(point);
	if (selectIdx >= 0)
	{
		m_points[selectIdx].bSelected = true;
		m_selectIdx = selectIdx;
	}
	else
	{
		// 점 생성
		if (m_pointCount < 3)
		{
			m_points.push_back(PointData{ point, true });
			m_selectIdx = m_pointCount;
			SetTextFormatPoint(m_ctrlIds[m_selectIdx], point);
			++m_pointCount;
		}
	}

	Invalidate();

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircleDrawMFCDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_selectIdx = -1;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircleDrawMFCDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_selectIdx >= 0 && m_points.size() > m_selectIdx && m_points[m_selectIdx].bSelected)
	{
		m_points[m_selectIdx].center = point;
		SetTextFormatPoint(m_ctrlIds[m_selectIdx], point);
		Invalidate(FALSE);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircleDrawMFCDlg::OnResetButton()
{
	m_selectIdx = -1;
	m_pointCount = 0;
	m_points.clear();
	Invalidate();
}

void CCircleDrawMFCDlg::OnNumberChangeEdit()
{
	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd == nullptr) return;

	int ctrlID = pFocusWnd->GetDlgCtrlID();
	int value = GetDlgItemInt(ctrlID, NULL, FALSE);

	if (1 > value)
	{
		value = 1;
	}
	else if (value > 10)
	{
		value = 10;
	}

	switch (ctrlID)
	{
	case IDC_EDIT5:
		if (m_radius != value)
		{
			m_radius = value;
			Invalidate();
		}
		break;
	case IDC_EDIT6:
		if (m_outline != value)
		{
			m_outline = value;
			Invalidate();
		}
		break;
	}
}
