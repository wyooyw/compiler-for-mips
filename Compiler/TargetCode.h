#include "ASTNode.h"
#include "Output.h"
#include "SignTable.h"
#include <vector>
#pragma once
#define $sp 29
#define $gp 28
#define $zero 0
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $a0 4
#define $v0 2
class TargetCode {
public:
	TargetCode(SignTable* signTable);

	void t_strings();
	void t_begin(ASTNode* astnode_program);
	void t_signs_global();
	void t_signs_main();
	void t_program(ASTNode* astnode_program);
	void t_main(ASTNode* astnode_program);
	void t_stmt_list(ASTNode* stmt_program);
	int t_factor(ASTNode* factor_program,int& type);//返回结果存放的地址
	void t_assign(ASTNode* assign_program);
	void t_print(ASTNode* print_program);
	void t_scanf(ASTNode* scanf_program);
	void t_printstring(ASTNode* printstring);
	void t_printexpression(ASTNode* printexpression);
	int loadVar(int regis, char* name);
	void saveVar(int regis, char* name);
	void t_globalVarInit();
	void t_mainVarInit();
private:
	int allocSpMemory();
	int getOffsetOfVar(char* name);
	int addStr(char *str);//保存字符串常量，返回地址
	vector<char*> strs;
	int data_offset=0;
	int sp_offset=0;
	int gp_offset=0;

	SignTable* signTable;
};