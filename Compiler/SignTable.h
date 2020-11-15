#pragma once
#include "Sign.h"
#include "Word.h"
#include<stdio.h>
#include<string>
#include<string.h>
const int WORD_SIZE = 4;//4B
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

	bool getSign(char* name, Sign& sign);

	void addVar(int type, char* name, int dimen,int level,int init);
	void addConst(int type, char* name, int dimen,int level,int init);
	void addFunc(int type, char* name,int level);
	void refillFunc(char* name, int level, int paralen, int paratype[]);
	void addPara(int type, char* name, int level);
	
	int getGpOffset();
	int getSpOffset();

	void getGlobalSigns(Sign *&begin,int &len);
	void getMainSigns(Sign*& begin, int& len);
private:
	Sign *signs;
	int valid[50000];
	int top = 0;
	int gp_offset = 0;
	int sp_offset = 0;
	
	int globalVarCount = 0;	//ȫ�ֱ�������(ȫ�ֱ�����ʼΪsigns)
	Sign* mainVarBegin;		//main����������ʼ
	int mainVarCount = 0;	//main������������

};