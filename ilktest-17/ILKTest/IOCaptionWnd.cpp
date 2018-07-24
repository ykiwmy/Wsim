// IOCaptionWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ILKTest.h"
#include "IOCaptionWnd.h"
#include "Relay.h"
#include "ZcCommRelay.h"
#include "ILKGraphic\ColorDef.h"
#include "ILKGraphic\CADDevice.h"

// CIOCaptionWnd

IMPLEMENT_DYNAMIC(CIOCaptionWnd, CWnd)

CIOCaptionWnd::CIOCaptionWnd(CString caption, Relay* pRelay)
: caption_(caption)
, pRelay_(pRelay)
{
	bZc_ = false;
}

CIOCaptionWnd::CIOCaptionWnd(CString caption, CZcCommRelay* pZcRelay, bool bZc)
: caption_(caption)
, pZcRelay_(pZcRelay)
, bZc_(bZc)
{
}

CIOCaptionWnd::~CIOCaptionWnd()
{
}


BEGIN_MESSAGE_MAP(CIOCaptionWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CIOCaptionWnd 消息处理程序


void CIOCaptionWnd::OnPaint()
{
	CPaintDC dc(this); 
	dc.SelectStockObject(DEFAULT_GUI_FONT);

	if (bZc_)
	{
		if (pZcRelay_)
		{
			switch (pZcRelay_->stat_)
			{
			case CZcCommRelay::COMM_BOTH:
				dc.SetTextColor(COLOR_WHITE);
				break;
			case CZcCommRelay::COMM_NEIGHER:
				dc.SetTextColor(COLOR_BLACK);
				break;
			case CZcCommRelay::COMM_LOW:
				dc.SetTextColor(COLOR_BLUE);
				break;
			case CZcCommRelay::COMM_HIGH:
				dc.SetTextColor(COLOR_RED);//COLOR_PURPLE);
				break;
			default:
				dc.SetTextColor(COLOR_GRAY); // 正常情况下不会出现！！
				break;
			}
		}
		else
		{
			// 本端口没有使用！！
			dc.SetTextColor(COLOR_RED);
		}
	}
	else
	{
		if (pRelay_)
		{
			switch (pRelay_->stat_)
			{
			case Relay::STAT_BOTH:
				dc.SetTextColor(COLOR_WHITE);
				break;
			case Relay::STAT_NEIGHER:
				dc.SetTextColor(COLOR_BLACK);
				break;
			case Relay::STAT_DOWN:
				dc.SetTextColor(COLOR_BLUE);
				break;
			case Relay::STAT_UP:
				dc.SetTextColor(COLOR_RED);//COLOR_PURPLE);
				break;
			default:
				dc.SetTextColor(COLOR_GRAY); // 正常情况下不会出现！！
				break;
			}
		}
		else
		{
			// 本端口没有使用！！
			dc.SetTextColor(COLOR_RED);
		}
	}

	dc.SetBkMode(TRANSPARENT);
	dc.TextOutW(0, 0, caption_);
}



void CIOCaptionWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (bZc_)
	{
		if (pZcRelay_ != nullptr)
		{
			if (pZcRelay_->pDevice_ != NULL)
			{
				pZcRelay_->pDevice_->OnClickZcCommRelay(pZcRelay_);
			}
			else
			{
				AfxMessageBox(L"分支");
			}
		}
		else
		{
			AfxMessageBox(L"点击了空配置");
		}
	}
	else
	{
		if (pRelay_ != nullptr)
		{
			if (pRelay_->pDevice_ != NULL)
			{//如果继电器已关联设备
				pRelay_->pDevice_->OnClickRelay(pRelay_);


			}
			else
			{
				//继电器未关联设备
				pRelay_->ChangeRelayStat();
			}
		}
		else
		{
			AfxMessageBox(L"无继电器配置");
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}
