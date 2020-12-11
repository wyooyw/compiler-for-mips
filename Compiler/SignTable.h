#pragma once
#include "SubTable.h"
#include "Sign.h"
#include "Word.h"
//#include "RegisterManager.h"
#include<stdio.h>
#include<string>
#include<string.h>
#define $sp 29
#define $gp 28
const int MAX_FUNC_NUM = 100;
class SignTable {
public:
	SignTable();
	void push(Sign sign);	//插入一个符号
	bool havaSignInSameLevel(char* name,int level);//是否有name和level与所给信息相同的符号
	void checkPara(char* name, int paralen, int paratype[],int row);//检查最近的函数定义中，参数和给定参数信息是否一致
	void show();//	打印符号表
	int getSignType(char* name);		//查找符号的类型
	int getFuncType(char* name);		//查找函数的类型
	int getSignCategory(char* name);
	int getSignReturn(char* name);
	bool havaSign(char* name);
	void checkReturn(int retType,int row);

	bool getSign(char* name, Sign& sign);

	void addVar(int type, char* name, int dimen,int level,int init);
	void addVar(int type, char* name, int dimen, int level);
	void addConst(int type, char* name, int dimen,int level,int init);
	void addConst(int type, char* name, int dimen, int level);
	void addVar(int type, char* name, int dimen, int dimen_n, int dimen_m, int level, vector<int> init);
	void addVar(int type, char* name, int dimen, int dimen_n, int dimen_m, int level);
	
	void addFunc(int type, char* name,int level);
	void refillFunc(char* name, int level, int paralen, int paratype[]);
	void addPara(int type, char* name, int level);
	
	int getGpOffset();
	int getSpOffset();


	list<Sign*> getGlobalSigns();
	list<Sign*> getMainSigns();
	list<Sign*> getFuncSigns();

	void setCurrent(char* funcName);
	void setCurrent(int cur);
	void showMainId();

private:
	int top = 0;
	int gp_offset = 0;
	
	int globalVarCount = 0;	//全局变量个数(全局变量起始为signs)
	Sign* mainVarBegin;		//main函数变量起始
	int mainVarCount = 0;	//main函数变量个数

	map<string, int> funname2id;

	SubTable subTables[MAX_FUNC_NUM+1];//0为全局变量，1位各个函数的子符号表
	int subTablesTop = 1;			//初始化为1

	int current = 0;					//0代表全局，大于0代表函数内,对应subTables[current]
	char currentName[MAX_WORD_LEN];

	int signId = 0;
};