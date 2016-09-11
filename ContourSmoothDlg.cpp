// ContourSmoothDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "DiscoverKernel.h"
#include "ContourSmoothDlg.h"
#include "afxdialogex.h"


#include "ppl.h"

// ContourSmoothDlg 對話方塊

IMPLEMENT_DYNAMIC(ContourSmoothDlg, CDialogEx)

ContourSmoothDlg::ContourSmoothDlg(		
	CDiscoverKernelCtrl *pCDiscoverKernelCtrl,
	CContourManager *pContourManager,
	CWnd* pParent /*=NULL*/)
	: CDialogEx(ContourSmoothDlg::IDD, pParent),
	m_pCDiscoverKernelCtrl(pCDiscoverKernelCtrl),
	m_pContourManager(pContourManager),
	m_nSelectedPage(1)
	,m_dbHigh(0)
	,m_dbWidth(0)
	, m_dbHighDown(0)
	, m_dbWidthDown(0)
	, m_dbX1(0)
	, m_dbX2(0)
	, m_dbY1(0)
	, m_dbY2(0)
{

}

ContourSmoothDlg::~ContourSmoothDlg()
{
	RestoreSelectedContourPoint();
}

void ContourSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_HIGH, m_ScrHigh);
	DDX_Control(pDX, IDC_SCROLLBAR_Width, m_SrcWidth);
	DDX_Text(pDX, IDC_EDIT_HIGH, m_dbHigh);
	DDX_Text(pDX, IDC_EDIT_Width, m_dbWidth);
	DDX_Text(pDX, IDC_EDIT_HIGH_Down, m_dbHighDown);
	DDX_Text(pDX, IDC_EDIT_Width_Down, m_dbWidthDown);
	DDX_Control(pDX, IDC_SCROLLBAR_HIGH_Down, m_ScrHighDown);
	DDX_Control(pDX, IDC_SCROLLBAR_Width_Down, m_SrcWidthDown);
	DDX_Text(pDX, IDC_EDIT_RangeX1, m_dbX1);
	DDX_Text(pDX, IDC_EDIT_RangeX2, m_dbX2);
	DDX_Text(pDX, IDC_EDIT_RangeY1, m_dbY1);
	DDX_Text(pDX, IDC_EDIT_RangeY2, m_dbY2);
}


BEGIN_MESSAGE_MAP(ContourSmoothDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ContourSmoothDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ContourSmoothDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_APPLY, &ContourSmoothDlg::OnBnClickedApply)
	ON_EN_CHANGE(IDC_EDIT_HIGH, &ContourSmoothDlg::OnEnChangeEditHigh)
	ON_EN_CHANGE(IDC_EDIT_Width, &ContourSmoothDlg::OnEnChangeEditWidth)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_HIGH_Down, &ContourSmoothDlg::OnEnChangeEditHighDown)
	ON_EN_CHANGE(IDC_EDIT_Width_Down, &ContourSmoothDlg::OnEnChangeEditWidthDown)
	ON_EN_CHANGE(IDC_EDIT_RangeX1, &ContourSmoothDlg::OnEnChangeEditRangex1)
	ON_EN_CHANGE(IDC_EDIT_RangeY1, &ContourSmoothDlg::OnEnChangeEditRangey1)
	ON_EN_CHANGE(IDC_EDIT_RangeX2, &ContourSmoothDlg::OnEnChangeEditRangex2)
	ON_EN_CHANGE(IDC_EDIT_RangeY2, &ContourSmoothDlg::OnEnChangeEditRangey2)
END_MESSAGE_MAP()


// ContourSmoothDlg 訊息處理常式


void ContourSmoothDlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnOK();
	OnBnClickedApply();
}


void ContourSmoothDlg::OnBnClickedCancel()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnCancel();
	RestoreSelectedContourPoint();

}

bool ContourSmoothDlg::SetSelectedContour(const CString &rstrSelectedContourName,int nSelectedPage)
{
	RestoreSelectedContourPoint(0);
	m_listBackUpContourPointList.clear();
	m_strSelectedContourName=rstrSelectedContourName;
	m_nSelectedPage=nSelectedPage;
	return Updata2dViewer();
}

bool ContourSmoothDlg::RestoreSelectedContourPoint(bool bIsFlash)
{
	auto pSelectedContour=GetSelectedContour();
	if (!m_listBackUpContourPointList.empty() && pSelectedContour)
	{
		pSelectedContour->SetContour(&m_listBackUpContourPointList);
		if (m_pCDiscoverKernelCtrl && bIsFlash)
		{
			m_pCDiscoverKernelCtrl->FireEvent_3DBuilderOperation("Refresh",(LONGLONG)GetSelectedContourFirstPoint());
		}

		return true;
	}

	return false;
}


void ContourSmoothDlg::OnBnClickedApply()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (Updata2dViewer())
	{
		auto pSelectedContour=GetSelectedContour();
		m_listBackUpContourPointList=*pSelectedContour->GetContour();
	}	
	
}

CSingleContour*ContourSmoothDlg::GetSelectedContour()const
{
	return 
		m_pContourManager?
		m_pContourManager->FindContour(m_strSelectedContourName,m_nSelectedPage):
		nullptr;
}

BOOL ContourSmoothDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	m_ScrHigh.SetScrollRange(-100, 100);
	m_SrcWidth.SetScrollRange(-100, 100);
	m_ScrHighDown.SetScrollRange(-100, 100);
	m_SrcWidthDown.SetScrollRange(-100, 100);

	
	EditValueToSrcForContourSmoothDlg();

	return TRUE; 

}

void ContourSmoothDlg::EditValueToSrcForContourSmoothDlg()
{
	UpdateData(TRUE);
	m_ScrHigh.SetScrollPos(m_dbHigh);
	m_SrcWidth.SetScrollPos(m_dbWidth);
	m_ScrHighDown.SetScrollPos(m_dbHighDown);
	m_SrcWidthDown.SetScrollPos(m_dbWidthDown);

}

void ContourSmoothDlg::ScrValueToEditForContourSmoothDlg()
{
	m_dbHigh=m_ScrHigh.GetScrollPos();
	m_dbWidth=m_SrcWidth.GetScrollPos();
	m_dbHighDown=m_ScrHighDown.GetScrollPos();
	m_dbWidthDown=m_SrcWidthDown.GetScrollPos();
	UpdateData(FALSE);
}

//Peter 2016/08/22
void ContourSmoothDlg::RemoveBumpFront(
	 CONTOUR *pCSingleContour, 
     std::list<DPOINT>::iterator itrContourPoint1,
	 std::list<DPOINT>::iterator itrContourPoint4)

{

	   
  //itrContourPoint1  = pCSingleContour->begin();
  itrContourPoint1;
  //itrContourPoint2=pCSingleContour->end()--;
  itrContourPoint4;
  pCSingleContour->erase (itrContourPoint1,itrContourPoint4);

		
}
void ContourSmoothDlg::RemoveBumpBehind(
	 CONTOUR *pCSingleContour, 
     std::list<DPOINT>::iterator itrContourPoint3,
	 std::list<DPOINT>::iterator itrContourPoint5)

{

	   
  //itrContourPoint1  = pCSingleContour->begin();
  itrContourPoint3;
  //itrContourPoint2=pCSingleContour->end()--;
  itrContourPoint5;
  pCSingleContour->erase (itrContourPoint3,itrContourPoint5);

		
}
void ContourSmoothDlg::InsertLine(
	 CONTOUR *pCSingleContour, 
     std::list<DPOINT>::iterator itrContourPoint1,
	 std::list<DPOINT>::iterator itrContourPoint5)
{
     //for (itrContourPoint1; itrContourPoint1!=itrContourPoint5; --itrContourPoint5) 
		//{
         //++itrContourPoint1->y=--itrContourPoint1->y;
		 //++itrContourPoint1->x=itrContourPoint1->x+1;
		 //The following two lines are used in another case 9/11
		 // ++itrContourPoint1->y= (  (itrContourPoint5->y-itrContourPoint1->y))  /  (( itrContourPoint5->x-itrContourPoint1->x)  ) *( ++itrContourPoint1->x - itrContourPoint1->x ) + itrContourPoint1->y;
		long shiftX=itrContourPoint1->x	  ;
		long shiftY=itrContourPoint1->y	  ;
		DPOINT ShiftPoint(shiftX,shiftY,0);	 //declare a DPoint object as a container 
	   while(ShiftPoint.x != itrContourPoint5->x) //if shiftpoint's x haven't reached x2's x
	   {
		ShiftPoint.x=--shiftX; //shift x 	
		ShiftPoint.y=shiftY;  //shift y
		pCSingleContour->push_back(ShiftPoint);  //push the DPoint object to the list.
		//pCSingleContour->push_back(++itrContourPoint1->x);

	   }
   		    
	  
	    //}
}
bool ContourSmoothDlg::ContourSmooth(
	CONTOUR *pCSingleContour, 
	int nHight,int nWidth,
	int nHightDown,int nWidthDown	,
	CRect CInSmoothRange
	)
{
	CONTOUR *pCSingleContourPtr;
	if (!pCSingleContour)
	{
		return false;
	}
//peter 2016/08/23 set the rectangle
	CInSmoothRange.left=1260;
	CInSmoothRange.right=1293;
	CInSmoothRange.top=417;
	CInSmoothRange.bottom=439;

	DPOINT CShiftPoint(nWidth,nHight,0);
	TRACE("\nContour Point (X,Y) : \n");

	unsigned unIndex(0);
	std::list<DPOINT>::iterator itrContourPoint1=pCSingleContour->begin(),itrContourPoint5=pCSingleContour->begin(),itrContourPoint3=pCSingleContour->begin();

    std::list<DPOINT>::iterator itrContourPoint2,itrContourPoint_down,itrContourPoint_level, badPoint;
	
	//test contour end
	std::list<DPOINT>::iterator itrContourPoint4=--pCSingleContour->end();

/*	for (auto &rDPoint:*pCSingleContour)
	{	
		TRACE("(%d,%d)\t",(int)rDPoint.x,(int)rDPoint.y);
		rDPoint+=CShiftPoint;		
	}
*/

	/*
	while (itrContourPoint1!=pCSingleContour->end())
	{	
		
		TRACE("(%d,%d)\t",(int)itrContourPoint1->x,(int)itrContourPoint1->y);
		
	}
	*/
	//let contourpoint1 +30   
	for(int j=0;j<=100;j++)
	{itrContourPoint1++;}
	itrContourPoint1;
	//2016.9.7 find itrContourPoint1 
	for(;itrContourPoint1!=itrContourPoint4;itrContourPoint1++)
	{
		
//Peter 2016/08/22        
	//test whether the contour point is inside the rectangle
		if(((int)itrContourPoint1->x >= CInSmoothRange.left && (int)itrContourPoint1->x <= CInSmoothRange.right)
		&&((int)itrContourPoint1->y >= CInSmoothRange.top && (int)itrContourPoint1->y <= CInSmoothRange.bottom))
		{
// the pointer is inside the rectangle
// use itrContourPoint1 to remember the first point of the bump
//use for loop to correct the second bump		  
//			for(int i=0;i<3;i++)
//		   {
//set the same interval between point1 and point2	   			 
			itrContourPoint2 = itrContourPoint1;
			itrContourPoint2 ++;
			itrContourPoint2 ++;
			itrContourPoint2 ++;

		
			 
//detect where the bump starts
			while(((int)itrContourPoint1->y - (int)(itrContourPoint2->y)) <=2)
				{
					//move point1 and point2 one step forward.
					itrContourPoint1 ++;
					itrContourPoint2 ++;
				//itrContourPoint2 is pointing to the next 3 contour point relative to itrContourPoint1
				/*
				itrContourPoint2 = itrContourPoint1--;
				itrContourPoint2 = itrContourPoint2--;
				itrContourPoint2 = itrContourPoint2--;
				itrContourPoint2 = itrContourPoint2--;
				itrContourPoint1--;
				*/
				}//remeber itrContourPoint1
			
			 //detect if (itrContourPoint2->y)-(itrContourPoint1->y)>1
/*				for(;itrContourPoint5!=itrContourPoint3;itrContourPoint5++)
				{
					while((itrContourPoint1->y)-(itrContourPoint5->y)>1)
					{	
					itrContourPoint5++;		
					}
			 //delete the contour from contour1 to contour2
				  RemoveBump(pCSingleContour,itrContourPoint1,itrContourPoint5);
		     //insert the contour from contour1 to contour2
				  InsertBump(pCSingleContour,itrContourPoint1,itrContourPoint5);
			 //	   itrContourPoint1 = itrContourPoint2;
				}
*/ /*		if(((int)itrContourPoint1->y - (int)(itrContourPoint2->y)) >=2 )
		{
			   if((itrContourPoint2->y)-(itrContourPoint1->y)<=1)
			   {
				   
				  //delete the contour from contour1 to contour2
				  RemoveBump(pCSingleContour,itrContourPoint1,itrContourPoint2);
				  //insert the contour from contour1 to contour2
				  InsertBump(pCSingleContour,itrContourPoint1,itrContourPoint2);
			   }
*/ //      }
/*		while(((itrContourPoint1->y)-(itrContourPoint2->y)<=1))
		//we do not see the end of the bump, continue to advance the pointer 
		itrContourPoint1++;	
*/		
			
		break; 
		} 
	}
	//delete the contour from contour1 to contour2
   	++itrContourPoint1;
	RemoveBumpFront(pCSingleContour,itrContourPoint1,itrContourPoint4);
	
	
	
	 while((itrContourPoint1->y)-(itrContourPoint5->y)>0)
	 {
	 	  itrContourPoint5++;
	 }
	 //delete the contour from contour1 to contour2
	 --itrContourPoint5;
	 RemoveBumpBehind(pCSingleContour,itrContourPoint3,itrContourPoint5);
	 //insert the contour from contour1 to contour2
	 --itrContourPoint1;
	 ++itrContourPoint5;
	 InsertLine(pCSingleContour,itrContourPoint1,itrContourPoint5);
/*	for(;(itrContourPoint1->y)-(itrContourPoint5->y)>=0;itrContourPoint5++)
	{
		
			 
		     //delete the contour from contour1 to contour2
				  RemoveBumpBehind(pCSingleContour,itrContourPoint3,itrContourPoint5);
			//insert the contour from contour1 to contour2
				  InsertLine(pCSingleContour,itrContourPoint1,itrContourPoint5);
			 //	   itrContourPoint1 = itrContourPoint2;
	}			
*/		//        itrContourPoint2=itrContourPoint1;
		//record the staring point in itrContourPoint1
	//after the bump starts, we should detect when the bump ends
//by detecting the y comes down to the original level with 1 difference
//detect the contour where start to decline 
/*     	itrContourPoint_down=itrContourPoint1++;
		itrContourPoint_down=itrContourPoint_down++;
	    itrContourPoint_down=itrContourPoint_down++;
		
		if(((int)itrContourPoint1->y - (int)(itrContourPoint_down->y)) >=2 )
		{
		  if(((int)(itrContourPoint1->y) - (int)(++itrContourPoint1->y)) >=1 )
		  itrContourPoint_level=itrContourPoint1;
		}
*/		 
//		(if((itrContourPoint2->y)-(itrContourPoint1->y)<=1)
		
	
    //delete the points between the itrContourPoint1 and itrContourPoint2   
/*		Delete();
		itrContourPoint1 = itrContourPoint2;
		{	 itrContourPoint3=itrContourPoint2;
		  for( ; (itrContourPoint3->x)<(itrContourPoint1->x)  ; itrContourPoint3++ )
         
	 //算出更正後的y值
	 itrContourPoint3->y= (  (itrContourPoint1->y-itrContourPoint2->y)  /  ( itrContourPoint1->x-itrContourPoint2->x)  )* ( itrContourPoint3->x - itrContourPoint2->x ) + itrContourPoint2->y;
*/	 
	 
        

        
				
/*			badPoint =itrContourPoint3;
		
		for(,badPoint<=itrContourPoint_level,badPoint)
		{
			badPoint->y= ((itrContourPoint_level->y)-(itrContourPoint3->y))/(itrContourPoint_level->x-itrContourPoint3->x)*(badPoint->x-itrContourPoint3->x) + itrContourPoint3->y;
		}
*/
}
		
		
		
//		unIndex++;
//advance the contour pointer to the next contour point until we
//find that we are already in the rectangle
//		itrContourPoint1++;
//	}


	//for (auto &rDPoint:*pCSingleContour)
	//{	
	//	TRACE("(%d,%d)\t",(int)rDPoint.x,(int)rDPoint.y);
	//	rDPoint+=CShiftPoint;		
	//}


//	return true;
//}


void ContourSmoothDlg::OnEnChangeEditHigh()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}


void ContourSmoothDlg::OnEnChangeEditWidth()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}

void ContourSmoothDlg::ChangeEditValueForContourSmoothDlg()
{
	UpdateData(TRUE);
	EditValueToSrcForContourSmoothDlg();
	RestoreSelectedContourPoint(0);
	Updata2dViewer(0);
	
	//此功能需強制刷新2D影像
	if (m_pCDiscoverKernelCtrl)
	{
		m_pCDiscoverKernelCtrl->FireEvent_3DBuilderOperation("Refresh",(LONGLONG)GetSelectedContourFirstPoint());
	}

}


void ContourSmoothDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if (pScrollBar)
	{

		int nScrPos = pScrollBar->GetScrollPos();    // 獲取水平滾動條當前位置   
		int Temp;
		switch (nSBCode)    
		{    
			// 如果向左滾動一列，則pos減1   
		case SB_LINEUP:    
			nScrPos -= 1;    
			break;    
			// 如果向右滾動一列，則pos加1   
		case SB_LINEDOWN:    
			nScrPos  += 1;    
			break;    
			// 如果向左滾動一頁，則pos減10   
		case SB_PAGEUP:    
			nScrPos -= 10;    
			break;    
			// 如果向右滾動一頁，則pos加10   
		case SB_PAGEDOWN:    
			nScrPos  += 10;    
			break;    
			// 如果滾動到最左端，則pos為1   
		case SB_TOP:    
			pScrollBar->GetScrollRange(&nScrPos,&Temp);    
			break;    
			// 如果滾動到最右端，則pos為100   
		case SB_BOTTOM:    
			nScrPos = pScrollBar->GetScrollLimit();    
			break;      
			// 如果拖動滾動塊滾動到指定位置，則pos賦值為nPos的值   
		case SB_THUMBPOSITION:    
			nScrPos = nPos;    
			break;    			    
		}    

		// 設置修正後的滾動塊位置   
		pScrollBar->SetScrollPos(nScrPos); 
		ScrValueToEditForContourSmoothDlg(); 

		//拖曳結束時才更新預覽結果
		if (nSBCode==SB_ENDSCROLL)
		{
			RestoreSelectedContourPoint(0);
			Updata2dViewer(0);
			//此功能需強制刷新2D影像
			if (m_pCDiscoverKernelCtrl)
			{
				m_pCDiscoverKernelCtrl->FireEvent_3DBuilderOperation("Refresh",(LONGLONG)GetSelectedContourFirstPoint());
			}
		}

	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

bool ContourSmoothDlg::Updata2dViewer(bool bIsFlash)
{	
	auto pSelectedContour=GetSelectedContour();
	if (pSelectedContour && (m_dbHigh!=0 || m_dbWidth!=0))
	{
		//還原或是備份先前的點
		if (m_listBackUpContourPointList.empty())
		{
			m_listBackUpContourPointList=*pSelectedContour->GetContour();

		}else
		{
			pSelectedContour->SetContour(&m_listBackUpContourPointList);
		}


		auto bContourSmooth = ContourSmooth(
			pSelectedContour->GetContour(),
			m_dbHigh,m_dbWidth,m_dbHighDown,m_dbWidthDown
			,CRect(m_dbX1,m_dbY1,m_dbX2,m_dbY2)
			);

		if (m_pCDiscoverKernelCtrl && bContourSmooth && bIsFlash)
		{
			m_pCDiscoverKernelCtrl->FireEvent_3DBuilderOperation("Refresh",(LONGLONG)GetSelectedContourFirstPoint());
		}

		return bContourSmooth;
	}

	return false;
}

DPOINT*ContourSmoothDlg::GetSelectedContourFirstPoint()const
{
	if (GetSelectedContour() && !GetSelectedContour()->GetContour()->empty())
	{
		return &(*GetSelectedContour()->GetContour()->begin());
	}

	return nullptr;
}

void ContourSmoothDlg::ContourSmoothDlgOperate(const CString &strCommand, LONGLONG pParameterList)
{
	if (!IsWindowVisible())
	{
		return;
	}


	if ( strCommand=="SetSelectedContour" )
	{
		vector<LONGLONG> &vctAnyTypeDataList=*((vector<LONGLONG>*)pParameterList);
		if (vctAnyTypeDataList.size()>=3)
		{
			vctAnyTypeDataList[2]=SetSelectedContour(
				*((CString*)vctAnyTypeDataList[0]),
				*((int*)vctAnyTypeDataList[1])
				);
		}
	}

}

void ContourSmoothDlg::OnEnChangeEditHighDown()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}


void ContourSmoothDlg::OnEnChangeEditWidthDown()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}


void ContourSmoothDlg::OnEnChangeEditRangex1()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}


void ContourSmoothDlg::OnEnChangeEditRangey1()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}


void ContourSmoothDlg::OnEnChangeEditRangex2()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}


void ContourSmoothDlg::OnEnChangeEditRangey2()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	ChangeEditValueForContourSmoothDlg();
}
