
// ILKTest.h : ILKTest Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "ChildView.h"
#include "DumpHelper.h"
// CILKTestApp:
// �йش����ʵ�֣������ ILKTest.cpp
//

class CILKTestApp : public CWinApp
{
public:
	CILKTestApp();


// ��д
public:
	virtual BOOL InitInstance();
	CChildView * thePView;
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CDumpHelper m_dumpHelper;
};

extern CILKTestApp theApp;
