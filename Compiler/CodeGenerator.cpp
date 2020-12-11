#include "CodeGenerator.h"
CodeGenerator::CodeGenerator(SignTable* para_signTable) {
	signTable = para_signTable;
}

//输入AST,输出MidCode
MidCode* CodeGenerator::genProgram(ASTNode* astnode_program) {
	MidCode* midcode_program = new MidCode();

	int len = astnode_program->getValue();
	ASTNode* astnode_func;
	for (int i = 0; i < len; i++) {
		astnode_func = astnode_program->getChild(i);
		if (astnode_program->getChild(i)->getType() == ASTNodeType_Main) {

			//main函数
			midcode_program->add(genMain(astnode_func));

		}
		else {

			//其他函数
			
			printf("Not exists in this experiment!");

		}
	}
	return midcode_program;
}
MidCode* CodeGenerator::genMain(ASTNode* astnode_main) {
	MidCode* midcode_main = new MidCode();
	return midcode_main;
}

//产生“语句列”的中间代码
MidCode* CodeGenerator::genStmtList(ASTNode* astnode_stmtlist) {
	MidCode* midcode_stmtlist = new MidCode();
	int len = astnode_stmtlist->getValue();
	ASTNode* stmt;
	for (int i = 0; i < len; i++) {
		stmt = astnode_stmtlist->getChild(i);
		switch (stmt->getType()) {
		case ASTNodeType_Assign:
			midcode_stmtlist->add(genAssign(stmt));
			break;
		case ASTNodeType_Print:

			break;
		case ASTNodeType_Scanf:
			break;
		case ASTNodeType_PrintExpression:
			break;
		case ASTNodeType_PrintString:
			break;
		default:
			break;
		}
	}
	return midcode_stmtlist;
}
MidCode* CodeGenerator::genAssign(ASTNode* astnode_stmtlist) {
	MidCode* midcode_assign = new MidCode();

	/*ASTNode* left = astnode_stmtlist->getChild(ASTNode_Assign_Left);
	ASTNode* right = astnode_stmtlist->getChild(ASTNode_Assign_Right);
	
	//生成右临时符号T1
	Sign* T1 = allocSign(WORD_SIZE);

	//让genFactor生成的代码，把结果放进T1里
	midcode_assign->add(genFactor(right));

	//建立 = , T1, , right的中间代码
	MidCodeUnit* unit = factory->makeAssign(T1,right);

	//将赋值代码加入中间代码
	midcode_assign->add(unit);
	*/
	return midcode_assign;
}
MidCode* CodeGenerator::genFactor(ASTNode* factor,Sign* rst) {
	MidCode* midcode_assign = new MidCode();
	/*if(factor.get)
	switch (factor->getValue()) {
	case '+':

		break;
	case '-':
		break;
	case '*':
		break;
	case '/':
		break;

	}*/
	return midcode_assign;
}

int allocNew() {
	return 0;
}
int getIdByName(char * name) {
	return 0;
}