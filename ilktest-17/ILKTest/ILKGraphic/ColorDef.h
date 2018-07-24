#pragma once
#define STRING_EMPTY				_T("")

#define COLOR_RED    RGB(255, 0,   0  ) // ��ɫ
#define COLOR_CYAN   RGB(0,   255, 255) // ��ɫ
#define COLOR_BLUE   RGB(0,   0,   255) // ��ɫ
#define COLOR_GREEN  RGB(0,   255, 0  ) // ��ɫ
#define COLOR_YELLOW RGB(255, 255, 0  ) // ��ɫ
#define COLOR_PINK   RGB(255, 97,  71 ) // ��ɫ
#define COLOR_BLACK  RGB(0,   0,   0  ) // ��ɫ
#define COLOR_WHITE  RGB(255, 255, 255) // ��ɫ
#define COLOR_PURPLE RGB(255, 0,   255)  // ��ɫ
#define COLOR_LBLUE   RGB(85,120, 182)   // ǳ��ɫ
#define COLOR_BROWN  RGB(0xCD,0x9B,0x1D)//��ɫ

#define COLOR_GRAY_100	RGB(100, 100, 100) // ��ɫ
#define COLOR_GRAY_200	RGB(200, 200, 200) 
#define COLOR_GRAY	RGB(128, 128, 128) 


CBrush* RedBrush();
CBrush* BlueBrush();
CBrush* GreenBrush();
CBrush* YellowBrush();
CBrush* WhiteBrush();
CBrush* BlackBrush();
CBrush* CyanBrush();

CPen*   RedPen();
CPen*   BluePen();
CPen*   BlackPen();
CPen*	InsuPen();
CPen*	InsuRedPen();
CPen*   SectionPen();
CPen*   SectionRedPen();
CPen*	SectionBluePen();
CPen*	SectionWhitePen();
CPen*	SectionYellowPen();
CPen*   SectionGreenPen();
CPen*   SectionBlackPen();
CPen*   InsuNullPen();
CPen*   LamppostPen();
CPen*	WhitePen();
CPen*	YellowPen();
CPen*   SectionPurplePen();
CPen*   SectionBrownPen();
CPen*   PurplePen();
CPen *  PinkPen();
CPen*   SignalCrossPen();
CPen* AfgOpenPen();
CPen* AfgClosePen();

CPen* LogicATPen();
CPen* LogicUTPen();