#pragma once

class CChildView;
class CIOView;
// CSplitterFrame ���

class CSplitterFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSplitterFrame)

public:
	CSplitterFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSplitterFrame();
public:
	CChildView*				pChildView_;			// վ��ͼ��
	CIOView*				pIOView_;				// ���ɰ�
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

	BOOL					bShowIOView_;			// �Ƿ���ʾ���ɰ�
public:
	void On32771();
	void On32772();
};


