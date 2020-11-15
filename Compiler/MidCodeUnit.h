#pragma once

/*
	中间代码的一行
*/
using namespace std;
enum MidCode_Cal {//计算类指令
	CAL_OP,CAL_LEFT,CAL_RIGHT,CAL_TO
};

class MidCodeUnit {
public:
	int getType();
	void setType(int para_type);
	int get(int name);
	void set(int name,int value);
	void print();
private:
	int type;
	int codeunit[5];
};