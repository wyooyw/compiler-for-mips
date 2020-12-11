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

	int allocTmpReg();			//申请临时寄存器
	int loadVar(char* name);				//变量加载到寄存器中
	int loadArrItem(char* name, int reg);		//一维数组某一项加载到寄存器中(下标由寄存器给出)
	int loadArrItem(char* name, int regn, int regm);//二维数组某一项加载到寄存器中(下标由寄存器给出)
	void freeReg(int regid);			//释放寄存器，写会内存
	void freeTmpReg(int regid);		//释放临时寄存器
	void saveVar(int reg,char* name);	//将寄存器的值存入内存(或其对应的寄存器)
	void record();						//保存当前寄存器环境的信息
	void recover();
	void freeAll();
	void freeAllAndRecover();
	void setSignTable(SignTable *table);
	void bindRegAndVar(int reg, char* varname);
	void bindRegAndArrItem(int reg, char* varname,int n);

	int allocTmpMemReg();		//申请 可存入内存的临时寄存器,返回id号，不是寄存器号。
	int loadTmpMemReg(int id);		//根据id将 内存临时寄存器 的内容放入临时寄存器中，返回
	//allocTmpReg相当于allocTmpMemReg和loadTmpMemReg连续执行

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