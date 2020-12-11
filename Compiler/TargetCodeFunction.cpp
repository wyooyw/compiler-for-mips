#include "TargetCode.h"
void TargetCode::t_func(ASTNode* astnode_func) {
	ASTNode* astnode_paralist;
	ASTNode* astnode_stmtlist;
	char* name;

	astnode_paralist = astnode_func->getChild(ASTNode_Func_ParaList);
	astnode_stmtlist = astnode_func->getChild(ASTNode_Func_StmtList);
	name = astnode_func->getValueStr();

	Output::label(name);

	t_paralist(astnode_paralist);			//加载参数

	t_varinit(name);							//加载变量

	t_stmt_list(astnode_stmtlist);

	regManager->freeAll();

	Output::jr($ra);
}

void TargetCode::t_paralist(ASTNode* astnode_paralist) {
	int len = astnode_paralist->getValue();
	int reg;
	char* name;
	/*if (len >= 1) {	//加载$a0
		reg = regManager->allocTmpReg();
		name = astnode_paralist->getChild(0)->getValueStr();
		regManager->bindRegAndVar(reg, name);
		Output::addi(reg, $a0, 0);
	}
	if (len >= 2) { //加载$a1
		reg = regManager->allocTmpReg();
		name = astnode_paralist->getChild(1)->getValueStr();
		regManager->bindRegAndVar(reg, name);
		Output::addi(reg, $a1, 0);
	}
	if (len >= 3) { //加载$a2
		reg = regManager->allocTmpReg();
		name = astnode_paralist->getChild(2)->getValueStr();
		regManager->bindRegAndVar(reg, name);
		Output::addi(reg, $a2, 0);
	}
	if (len >= 4) { //加载$a3
		reg = regManager->allocTmpReg();
		name = astnode_paralist->getChild(3)->getValueStr();
		regManager->bindRegAndVar(reg, name);
		Output::addi(reg, $a3, 0);
	}
	if (len > 4) { //加载栈中参数
		//不用加载，就放内存里，用的时候再取
	}*/
}

void TargetCode::t_varinit(char* funcname) {
	sp_offset = signTable->getSpOffset();
	
	Sign* sign;
	int len;
	list<Sign*> *signs = signTable->getFuncSigns();
	list<Sign*>::iterator iter;
	int reg = regManager->allocTmpReg();
	for (iter = signs->begin(); iter != signs->end(); iter++) {
		sign = *iter;
		if (!sign->hasInitValue()) continue;		//如果没有初值，直接跳过
		int dimen = sign->getDimen();

		if (dimen == 0) {							//初始化普通变量
			//printf("\n%s ,%d", sign->getName(), sign->getOffset());
			int value = sign->getInitValue();
			int offset = sign->getOffset();
			Output::addi($t0, $zero, value);
			Output::sw($t0, offset, $sp);
		}
		else if (dimen == 1) {						//初始化一维数组
			for (int i = 0; i < sign->getDimenN(); i++) {

				int value = sign->getInitValue(i);
				int offset = sign->getOffset() + i * WORD_SIZE;
				Output::addi($t0, $zero, value);
				Output::sw($t0, offset, $sp);
				//printf("\n%s[%d] ,%d", sign->getName(), i, offset);
			}
		}
		else if (dimen == 2) {						//初始化二维数组
			for (int i = 0; i < sign->getDimenN(); i++) {
				for (int j = 0; j < sign->getDimenM(); j++) {
					int value = sign->getInitValue(i, j);
					int offset = sign->getOffset() + (i * sign->getDimenM() + j) * WORD_SIZE;
					Output::addi($t0, $zero, value);
					Output::sw($t0, offset, $sp);
					//printf("\n%s[%d][%d] ,%d", sign->getName(), i, j, offset);
				}
			}
		}
		else {
			//printf("未知维度？！");
		}
	}
	regManager->freeTmpReg(reg);
}

void TargetCode::t_func_call(ASTNode* astnode_call) {
	ASTNode* astnode_valueList;
	astnode_valueList = astnode_call->getChild(ASTNode_Call_ValueList);

	char* name = astnode_call->getValueStr();

	//注入参数
	t_valuelist(astnode_valueList);
	

	regManager->freeAll();

	Output::sw($ra, sp_offset+WORD_SIZE, $sp);

	Output::addi($sp, $sp, sp_offset+ WORD_SIZE+ WORD_SIZE);

	Output::jal(name);					//跳转到函数

	Output::subi($sp, $sp, sp_offset+ WORD_SIZE+ WORD_SIZE);

	Output::lw($ra, sp_offset+ WORD_SIZE, $sp);
}

void TargetCode::t_valuelist(ASTNode* astnode_valuelist) {
	int len = astnode_valuelist->getValue();

	ASTNode* astnode_value;

	int type,reg;
	/*
	if (len >= 1) {
		astnode_value = astnode_valuelist->getChild(0);
		reg = t_factor(astnode_value, type);
		Output::addi($a0, reg, 0);
		regManager->freeTmpReg(reg);
	}
	if (len >= 2) {
		astnode_value = astnode_valuelist->getChild(1);
		reg = t_factor(astnode_value, type);
		Output::addi($a1, reg, 0);
		regManager->freeTmpReg(reg);
	}
	if (len >= 3) {
		astnode_value = astnode_valuelist->getChild(2);
		reg = t_factor(astnode_value, type);
		Output::addi($a2, reg, 0);
		regManager->freeTmpReg(reg);
	}
	if (len >= 4) {
		astnode_value = astnode_valuelist->getChild(3);
		reg = t_factor(astnode_value, type);
		Output::addi($a3, reg, 0);
		regManager->freeTmpReg(reg);
	}*/
	int ids[100];
	for (int i = 0; i < len; i++) {						//存放第四个之后的参数
		astnode_value = astnode_valuelist->getChild(i);
		reg = t_factor(astnode_value, type);
		ids[i] = bindRegWithTmpMem(reg);
		//Output::sw(reg, sp_offset + WORD_SIZE * (i+2),$sp);		
		//regManager->freeTmpReg(reg);
	}
	char str[100];
	for (int i = 0; i < len; i++) {
		int reg = regManager->loadVar(my_itoa(ids[i],str));
		Output::sw(reg, sp_offset + WORD_SIZE * (i + 2), $sp);
	}
}