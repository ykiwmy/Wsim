#pragma once
#include <vector>
using namespace std;

class RailSwitch;

class SwitchSection
{
public:
	SwitchSection(char* name, int addr);
	~SwitchSection();
public:
	CString name_;
	int addr_;
	vector<RailSwitch*> vSwitch_;
};

