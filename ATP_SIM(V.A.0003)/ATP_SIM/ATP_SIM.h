
// ATP_SIM.h : ATP_SIM Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CATP_SIMApp:
// �йش����ʵ�֣������ ATP_SIM.cpp
//

class CATP_SIMApp : public CWinApp
{
public:
	CATP_SIMApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CATP_SIMApp theApp;
