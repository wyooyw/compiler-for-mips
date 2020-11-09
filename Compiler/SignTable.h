#pragma once
#include "Sign.h"
#include "Word.h"
#include<stdio.h>
#include<string>
#include<string.h>
class SignTable {
public:
	SignTable();
	void push(Sign sign);	//插入一个符号
	void popTopLevel(int level);		//“删除”当前level最高的所有符号
	bool getSignRecent(char* name,Sign &sign);//通过名称查找当前最近的符号
	bool havaSignInSameLevel(char* name,int level);//是否有name和level与所给信息相同的符号
	void checkPara(char* name, int paralen, int paratype[],int row);//检查最近的函数定义中，参数和给定参数信息是否一致
	void show();//	打印符号表
	int getSignType(char* name);		//查找符号的类型
	int getSignCategory(char* name);
	int getSignReturn(char* name);
	bool havaSign(char* name);
	void checkReturn(int retType,int row);

	void addVar(int type, char* name, int dimen,int level);
	void addConst(int type, char* name, int dimen,int level);
	void addFunc(int type, char* name,int level);
	void refillFunc(char* name, int level, int paralen, int paratype[]);
	void addPara(int type, char* name, int level);
	

private:
	Sign *signs;
	int valid[10000];
	int top;
};