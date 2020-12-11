#include <algorithm>
using namespace std;
#pragma once
class Register {
public:
	int getVarId();
	void setVarId(int id);

	void setFree();
	void setTemp();
	void setVar();
	bool isFree();
	bool isTemp();
	bool isVar();
	int getRegId();
	void setRegId(int regId);
	pair<int,int> getReflect();				//base,offset
	void setReflect(pair<int, int> addr);
	int getAllocTime();
	void setAllocTime(int time);
	int getArrN();
	void setArrN(int n);
	int getArrM();
	void setArrM(int m);
private:
	int type = 0;			//0:未使用;1:临时；2：变量
	int regId;
	int reflect_base;
	int reflect_offset;
	int alloc_time;
	int var_id;
	int arr_n;
	int arr_m;
};