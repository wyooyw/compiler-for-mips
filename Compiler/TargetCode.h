#pragma once
#include "ASTNode.h"
#include "Output.h"
#include "SignTable.h"
#include "RegisterManager.h"
#include <vector>
#include <sstream>
#include <cstdlib>
#define MAX_CASE_NUM 500

class TargetCode {
public:
	TargetCode(SignTable* signTable);

	void t_strings();
	void t_begin(ASTNode* astnode_program);
	void t_signs_global();
	void t_signs_main();
	void t_program(ASTNode* astnode_program);
	void t_main(ASTNode* astnode_program);
	void t_stmt(ASTNode* astnode_stmt);
	void t_stmt_list(ASTNode* stmt_program);
	int t_factor(ASTNode* factor_program,int& type);//返回结果存放的寄存器
	void t_assign(ASTNode* assign_program);
	void t_print(ASTNode* print_program);
	void t_scanf(ASTNode* scanf_program);
	void t_printstring(ASTNode* printstring);
	void t_printexpression(ASTNode* printexpression);
	void t_globalVarInit();
	void t_mainVarInit();

	int t_cond(ASTNode* astnode_cond);	//符号,左寄存器，右寄存器
	void t_if(ASTNode* astnode_if);
	void t_ifelse(ASTNode* astnode_if);
	void t_switch(ASTNode* astnode_switch);

	void t_while(ASTNode* astnode_while);
	void t_for(ASTNode* astnode_for);

	void t_func(ASTNode* astnode_func);
	void t_paralist(ASTNode* astnode_paralist);
	void t_varinit(char* funcname);
	void t_func_call(ASTNode* astnode_call);
	void t_ret(ASTNode* astnode_ret);
	void t_valuelist(ASTNode* astnode_valuelist);
	int t_arr(ASTNode* astnode_arr,int &type);		//返回的寄存器里是数组该项的地址

private:
	int allocSpMemory();
	int allocLabel();
	int getOffsetOfVar(char* name);
	int addStr(char *str);//保存字符串常量，返回地址
	void bindRegWithTmpMem(int reg, char* name);
	int bindRegWithTmpMem(int reg);				//返回id
	void freeTmpMem(int reg);

	vector<char*> strs;
	int data_offset=0;
	int sp_offset=0;
	int gp_offset=0;
	int label = 0;
	int tmp_mem_count;

	SignTable* signTable;
	RegisterManager* regManager;

	char* my_itoa(int n,char str[]);
};