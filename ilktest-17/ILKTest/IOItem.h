#pragma once

class Relay;
class CIOCaptionWnd;

class IOItem
{
public:
	IOItem();
	~IOItem();
public:
	CIOCaptionWnd* pCaption_; // ���⣨��ţ���������ʾ�̵�������������״̬��
	CButton* pBtn_;	// CheckBox������ IO �˿��Ƿ���Ч
public:
	BOOL isPortValid();
};

