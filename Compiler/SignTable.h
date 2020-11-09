#pragma once
#include "Sign.h"
#include "Word.h"
#include<stdio.h>
#include<string>
#include<string.h>
class SignTable {
public:
	SignTable();
	void push(Sign sign);	//����һ������
	void popTopLevel(int level);		//��ɾ������ǰlevel��ߵ����з���
	bool getSignRecent(char* name,Sign &sign);//ͨ�����Ʋ��ҵ�ǰ����ķ���
	bool havaSignInSameLevel(char* name,int level);//�Ƿ���name��level��������Ϣ��ͬ�ķ���
	void checkPara(char* name, int paralen, int paratype[],int row);//�������ĺ��������У������͸���������Ϣ�Ƿ�һ��
	void show();//	��ӡ���ű�
	int getSignType(char* name);		//���ҷ��ŵ�����
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