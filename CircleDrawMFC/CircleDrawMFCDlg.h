
// CircleDrawMFCDlg.h: 헤더 파일
//

#pragma once

#include <iostream>
#include <vector>
#include <array>

// 원 정보
struct PointData
{
	CPoint center;   // 중심 좌표
	bool bSelected;  // 선택 여부
};

// CCircleDrawMFCDlg 대화 상자
class CCircleDrawMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CCircleDrawMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLEDRAWMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnResetButton();
	afx_msg void OnNumberChangeEdit();
private:
	// 점 최대 3개
	std::array<int, 3> m_ctrlIds;
	std::vector<PointData> m_points;
	CImage m_image;
	int m_pointCount;
	int m_radius = 2;
	int m_outline = 2;
	int m_selectIdx = -1;
	int IsCircleBounded(CPoint point);
	bool IsInCircle(CPoint pointA, CPoint pointB, int radius);
	void DrawCirclePointToImage(CPoint point, int radius, int outline = -1);
	std::pair<CPoint, int> GetCircle();
	void SetTextFormatPoint(int ctrlID, CPoint point, bool reset = false);
};
