#include "stdafx.h"
#include "ColorDef.h"
#include "CADDevice.h"

static CBrush RedBrush_	(COLOR_RED);
static CBrush BlueBrush_(COLOR_BLUE);
static CBrush GreenBrush_(COLOR_GREEN);
static CBrush YellowBrush_(COLOR_YELLOW);
static CBrush WhiteBrush_(COLOR_WHITE);
static CBrush BlackBrush_(COLOR_BLACK);
static CBrush CyanBrush_(COLOR_CYAN);
static CBrush LBlueBrush_(COLOR_LBLUE);
static CBrush BrownBrush_(COLOR_BROWN);

static CPen   RedPen_		(PS_SOLID, 1, COLOR_RED);
static CPen   BluePen_ (PS_SOLID, 1, COLOR_BLUE);
static CPen   BlackPen_(PS_SOLID, 1, COLOR_BLACK);
static CPen   YellowPen_(PS_SOLID, 1, COLOR_YELLOW);  //add by yu 2016-1-26
static CPen   LamppostPen_	(PS_SOLID, 2, COLOR_WHITE);
static CPen   InsuPen_(PS_SOLID, /*CDevice::NullInsuWidth_*/2, COLOR_GRAY);
static CPen   InsuRedPen_(PS_SOLID, /*CDevice::NullInsuWidth_*/1, COLOR_RED);
static CPen   SectionPen_(PS_SOLID, CADDevice::SectionWidth_,COLOR_LBLUE);// COLOR_CYAN);// 
static CPen   SectionRedPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_RED);
static CPen   SectionBluePen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_BLUE);
static CPen   SectionWhitePen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_WHITE);
static CPen   SectionYellowPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_YELLOW);
static CPen   SectionGreenPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_GREEN);
static CPen   SectionBlackPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_BLACK);
static CPen   SectionBrownPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_BROWN);
static CPen   SectionPurplePen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_PURPLE);
static CPen   InsuNullPen_(PS_SOLID, 2, COLOR_BLACK);
static CPen   WhitePen_(PS_SOLID, 1, COLOR_WHITE);
static CPen   PurplePen_(PS_SOLID,1, COLOR_PURPLE);
static CPen   PinkPen_(PS_SOLID, 1, COLOR_PINK);
static CPen   SignalCrossPen_(PS_SOLID, 2, COLOR_BLACK);
static CPen   AfgOpenPen_(PS_SOLID, 2, COLOR_RED);
static CPen   AfgClosePen_(PS_SOLID, 2, COLOR_GREEN);

static CPen	  LogicATPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_PURPLE);
static CPen	  LogicUTPen_(PS_SOLID, CADDevice::SectionWidth_, COLOR_BROWN);

CBrush* RedBrush()
{
	return &RedBrush_;
}


CBrush* BlueBrush()
{
	return &BlueBrush_;
}


CBrush* GreenBrush()
{
	return &GreenBrush_;
}


CBrush* YellowBrush()
{
	return &YellowBrush_;
}


CBrush* WhiteBrush()
{
	return &WhiteBrush_;
}


CBrush* BlackBrush()
{
	return &BlackBrush_;
}


CBrush* CyanBrush()
{
	return &CyanBrush_;
}


CPen* RedPen()
{
	return &RedPen_;
}

CPen* BluePen()
{
	return &BluePen_;
}

CPen* BlackPen()
{
	return &BlackPen_;
}

CPen* YellowPen()
{
	return &YellowPen_;
}
CPen* InsuPen()
{
	return &InsuPen_;
}
CPen* InsuRedPen()
{
	return &InsuRedPen_;
}
CPen* SectionPen()
{
	return &SectionPen_;
}
CPen* LogicATPen()
{
	return &LogicATPen_;
}
CPen* LogicUTPen()
{
	return &LogicUTPen_;
}

CPen* InsuNullPen()
{
	return &InsuNullPen_;
}

CPen* SectionGreenPen()
{
	return &SectionGreenPen_;
}

CPen* SectionBlackPen()
{
	return &SectionBlackPen_;
}

CPen* SectionRedPen()
{
	return &SectionRedPen_;
}
CPen* SectionBluePen()
{
	return &SectionBluePen_;
}


CPen* SectionWhitePen()
{
	return &SectionWhitePen_;
}


CPen* SectionYellowPen()
{
	return &SectionYellowPen_;
}


CPen* LamppostPen()
{
	return &LamppostPen_;
}


CPen* WhitePen()
{
	return &WhitePen_;
}

CPen* SectionPurplePen()
{
	return &SectionPurplePen_;
}
CPen* SectionBrownPen()
{
	return &SectionBrownPen_;
}
CPen* PurplePen()
{
	return &PurplePen_;
}
CPen* PinkPen()
{
	return &PinkPen_;
}
CPen* SignalCrossPen()
{
	return &SignalCrossPen_;
}

CPen* AfgOpenPen()
{
	return &AfgOpenPen_;
}
CPen* AfgClosePen()
{
	return &AfgClosePen_;
}