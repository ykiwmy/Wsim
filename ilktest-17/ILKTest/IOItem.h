#pragma once

class Relay;
class CIOCaptionWnd;

class IOItem
{
public:
	IOItem();
	~IOItem();
public:
	CIOCaptionWnd* pCaption_; // 标题（编号），包含显示继电器的吸起落下状态。
	CButton* pBtn_;	// CheckBox，设置 IO 端口是否有效
public:
	BOOL isPortValid();
};

