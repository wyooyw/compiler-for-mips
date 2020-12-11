#pragma once
#include "Register.h"
#include "SignTable.h"
#define $sp 29
#define $gp 28
#define $zero 0
#define $v0 2
#define $v1 3
#define $a0 4
#define $a1 5
#define $a2 6
#define $a3 7
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $t5 13
#define $t6 14
#define $t7 15
#define $s0 16
#define $s1 17
#define $s2 18
#define $s3 19
#define $s4 20
#define $s5 21
#define $s6 22
#define $s7 23
#define $t8 24
#define $t9 25
#define $ra 31

#define MAX_REG_RECORD_TIMES 100
using namespace std;

class RegisterManager {
public:
	RegisterManager();

	int allocTmpReg();			//������ʱ�Ĵ���
	int loadVar(char* name);				//�������ص��Ĵ�����
	int loadArrItem(char* name, int reg);		//һά����ĳһ����ص��Ĵ�����(�±��ɼĴ�������)
	int loadArrItem(char* name, int regn, int regm);//��ά����ĳһ����ص��Ĵ�����(�±��ɼĴ�������)
	void freeReg(int regid);			//�ͷżĴ�����д���ڴ�
	void freeTmpReg(int regid);		//�ͷ���ʱ�Ĵ���
	void saveVar(int reg,char* name);	//���Ĵ�����ֵ�����ڴ�(�����Ӧ�ļĴ���)
	void record();						//���浱ǰ�Ĵ�����������Ϣ
	void recover();
	void freeAll();
	void freeAllAndRecover();
	void setSignTable(SignTable *table);
	void bindRegAndVar(int reg, char* varname);
	void bindRegAndArrItem(int reg, char* varname,int n);

	int allocTmpMemReg();		//���� �ɴ����ڴ����ʱ�Ĵ���,����id�ţ����ǼĴ����š�
	int loadTmpMemReg(int id);		//����id�� �ڴ���ʱ�Ĵ��� �����ݷ�����ʱ�Ĵ����У�����
	//allocTmpReg�൱��allocTmpMemReg��loadTmpMemReg����ִ��

	void freeRegDirectly(int regid);
	void freeAllDirectly();

	void printReg(int regid);

private:
	Register recordRegisters[MAX_REG_RECORD_TIMES][32];
	int regRecordTop = 0;
	Register registers[32];
	int time = 0;
	SignTable *signTable;
	int nextTime();
};