#include "MidCodeUnit.h"
#include <vector>
#pragma once
using namespace std;
/*
	�м����
*/

class MidCode {
public:
	void add(MidCodeUnit* unit);
	void add(vector<MidCodeUnit*> units);
	void print();
	void add(MidCode* para_midcode);
private:
	vector<MidCodeUnit*> midcodes;
};