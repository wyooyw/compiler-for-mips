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
	void push(Sign sign);	//����һ������
	bool havaSignInSameLevel(char* name,int level);//�Ƿ���name��level��������Ϣ��ͬ�ķ���
	void checkPara(char* name, int paralen, int paratype[],int row);//�������ĺ��������У������͸���������Ϣ�Ƿ�һ��
	void show();//	��ӡ���ű�
	int getSignType(char* name);		//���ҷ��ŵ�����
	int getFuncType(char* name);		//���Һ���������
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
	
	int globalVarCount = 0;	//ȫ�ֱ�������(ȫ�ֱ�����ʼΪsigns)
	Sign* mainVarBegin;		//main����������ʼ
	int mainVarCount = 0;	//main������������

	map<string, int> funname2id;

	SubTable subTables[MAX_FUNC_NUM+1];//0Ϊȫ�ֱ�����1λ�����������ӷ��ű�
	int subTablesTop = 1;			//��ʼ��Ϊ1

	int current = 0;					//0����ȫ�֣�����0��������,��ӦsubTables[current]
	char currentName[MAX_WORD_LEN];

	int signId = 0;
};