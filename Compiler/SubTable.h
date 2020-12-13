#pragma once
#include <string>
#include <map>
#include <list>
#include "Sign.h"
using namespace std;
const int WORD_SIZE = 4;//4B
class SubTable {
public:
	bool hasName(char* name);				//检测是否有该名
	Sign* getSign(char* name);				//获取符号
	void addSign(Sign sign);				//添加符号
	list<Sign*>* getSignList();				//获取所有符号
	void print();
	int getSpOffset();
	int getId();
	void setId(int id);
private:
	int sp_offset = 0;
	map<string, Sign> signs;
	int id;
};