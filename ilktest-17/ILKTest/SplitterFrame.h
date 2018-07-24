#pragma once

class CChildView;
class CIOView;
// CSplitterFrame 框架

class CSplitterFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSplitterFrame)

public:
	CSplitterFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CSplitterFrame();
public:
	CChildView*				pChildView_;			// 站场图形
	CIOView*				pIOView_;				// 驱采板
protected:
	CSplitterWnd m_wndSplitter;
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	enum
	{
		IO_VIEW_WIDTH = 300,
	};

	BOOL					bShowIOView_;			// 是否显示驱采板
public:
	void On32771();
	void On32772();
};


