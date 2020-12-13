#pragma once
#include <string>
#include <map>
#include <list>
#include "Sign.h"
using namespace std;
const int WORD_SIZE = 4;//4B
class SubTable {
public:
	bool hasName(char* name);				//����Ƿ��и���
	Sign* getSign(char* name);				//��ȡ����
	void addSign(Sign sign);				//��ӷ���
	list<Sign*>* getSignList();				//��ȡ���з���
	void print();
	int getSpOffset();
	int getId();
	void setId(int id);
private:
	int sp_offset = 0;
	map<string, Sign> signs;
	int id;
};