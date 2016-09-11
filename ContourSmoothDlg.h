#pragma once
#include "afxwin.h"
#include "DiscoverKernelCtl.h"


// ContourSmoothDlg 對話方塊

class ContourSmoothDlg : public CDialogEx
{
private:
	CContourManager *m_pContourManager;//管理Contour的物件
	CDiscoverKernelCtrl *m_pCDiscoverKernelCtrl;//顯示點顏色到螢幕上的物件		
	
	
	CScrollBar m_ScrHigh;
	CScrollBar m_SrcWidth;
	double m_dbHigh;
	double m_dbWidth;

	CString m_strSelectedContourName;
	int m_nSelectedPage;
	CONTOUR m_listBackUpContourPointList;

	DECLARE_DYNAMIC(ContourSmoothDlg)

public:
	ContourSmoothDlg(
		CDiscoverKernelCtrl *pCDiscoverKernelCtrl,
		CContourManager *pContourManager,
		CWnd* pParent = NULL);   // 標準建構函式
	virtual ~ContourSmoothDlg();

// 對話方塊資料
	enum { IDD = IDD_DIALOG_ContourSmooth };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();


	bool ContourSmooth(
		CONTOUR *pCSingleContour,
		int nHight,int nWidth,
		int nHightDown,int nWidthDown,
		CRect CInSmoothRange
	);

	static void ContourSmoothDlg::RemoveBumpFront(
	 CONTOUR *pCSingleContour, 
     std::list<DPOINT>::iterator itrContourPoint1,
	 std::list<DPOINT>::iterator itrContourPoint4
	 );

	static void ContourSmoothDlg::RemoveBumpBehind(
	 CONTOUR *pCSingleContour, 
     std::list<DPOINT>::iterator itrContourPoint3,
	 std::list<DPOINT>::iterator itrContourPoint5
	 );
	static void ContourSmoothDlg::InsertLine(
	 CONTOUR *pCSingleContour, 
     std::list<DPOINT>::iterator itrContourPoint1,
	 std::list<DPOINT>::iterator itrContourPoint5
	 );
	virtual BOOL OnInitDialog();
	void ScrValueToEditForContourSmoothDlg();
	void EditValueToSrcForContourSmoothDlg();

	afx_msg void OnEnChangeEditHigh();
	afx_msg void OnEnChangeEditWidth();

	void ChangeEditValueForContourSmoothDlg();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool Updata2dViewer(bool bIsFlash = 1);
	CSingleContour*GetSelectedContour()const;
	bool SetSelectedContour(const CString &rstrSelectedContourName,int nSelectedPage);
	bool RestoreSelectedContourPoint(bool bIsFlash = 1);
	void ContourSmoothDlgOperate(const CString &strCommand, LONGLONG pParameterList);//和ContourSmoothDlg功能有關的操作接口
	DPOINT*GetSelectedContourFirstPoint()const;
protected:
	double m_dbHighDown;
	double m_dbWidthDown;
	CScrollBar m_ScrHighDown;
	CScrollBar m_SrcWidthDown;
	double m_dbX1;
	double m_dbX2;
	double m_dbY1;
	double m_dbY2;
public:
	afx_msg void OnEnChangeEditHighDown();
	afx_msg void OnEnChangeEditWidthDown();
	afx_msg void OnEnChangeEditRangex1();
	afx_msg void OnEnChangeEditRangey1();
	afx_msg void OnEnChangeEditRangex2();
	afx_msg void OnEnChangeEditRangey2();
};
