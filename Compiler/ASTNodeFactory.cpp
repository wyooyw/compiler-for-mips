#include "ASTNodeFactory.h"

ASTNode* ASTNodeFactory::makeASTNodeFactor(int sign, ASTNode* left,ASTNode *right) {
	int s;
	switch (sign) {
	case PLUS:
		s = '+';
		break;
	case MINU:
		s = '-';
		break;
	case MULT:
		s = '*';
		break;
	case DIV:
		s = '/';
		break;
	default:
		s = '!';
	}

	ASTNode* factor = new ASTNode();
	factor->setType(ASTNodeType_Factor);
	factor->setValue(s);
	factor->setChild(ASTNode_Factor_Left, left);
	factor->setChild(ASTNode_Factor_Right,right);
	return factor;
}

ASTNode* ASTNodeFactory::makeASTNodeInt(int value) {
	ASTNode* integer = new ASTNode();
	integer->setType(ASTNodeType_Int);
	integer->setValue( value);
	return integer;
}

ASTNode* ASTNodeFactory::makeASTNodeChar(char value) {
	ASTNode* character = new ASTNode();
	character->setType(ASTNodeType_Char);
	character->setValue(value);
	return character;
}

ASTNode* ASTNodeFactory::makeASTNodeVar(char* name) {
	ASTNode* idenfr = new ASTNode();
	idenfr->setType(ASTNodeType_Var);
	idenfr->setValueStr(name);
	return idenfr;
}

ASTNode* ASTNodeFactory::makeASTNodeAssign(ASTNode* left, ASTNode* right) {
	ASTNode* assign = new ASTNode();
	assign->setType(ASTNodeType_Assign);
	assign->setChild(ASTNode_Assign_Left, left);
	assign->setChild(ASTNode_Assign_Right, right);
	return assign;
}
ASTNode* ASTNodeFactory::makeASTNodePrint(ASTNode* expression) {
	ASTNode* print = new ASTNode();
	print->setType(ASTNodeType_PrintExpression);
	print->setChild(ASTNode_Print_Expression, expression);
	return print;
}
ASTNode* ASTNodeFactory::makeASTNodePrint(char* s) {
	ASTNode* str = new ASTNode();
	str->setType(ASTNodeType_Str);
	str->setValueStr(s);

	ASTNode* print = new ASTNode();
	print->setType(ASTNodeType_PrintString);
	print->setChild(ASTNode_Print_String, str);
	return print;
}
ASTNode* ASTNodeFactory::makeASTNodePrint(char* s, ASTNode* expression) {
	ASTNode* str = new ASTNode();
	str->setType(ASTNodeType_Str);
	str->setValueStr(s);

	ASTNode* print = new ASTNode();
	print->setType(ASTNodeType_Print);
	print->setChild(ASTNode_Print_String, str);
	print->setChild(ASTNode_Print_Expression, expression);
	return print;
}
ASTNode* ASTNodeFactory::makeASTNodeScanf(ASTNode* var) {
	ASTNode* scan = new ASTNode();
	scan->setType(ASTNodeType_Scanf);
	scan->setChild(ASTNode_Scanf_Var, var);
	return scan;
}

ASTNode* ASTNodeFactory::makeASTNodeStamentList(vector<ASTNode*> vec_statements) {
	int len = vec_statements.size();
	ASTNode* stmt_list = new ASTNode(len+1);
	stmt_list->setValue(len);
	stmt_list->setType(ASTNodeType_StmtList);
	
	for (int i = 0; i < len; i++) {
		stmt_list->setChild(i, vec_statements[i]);
	}
	return stmt_list;
}

ASTNode* ASTNodeFactory::makeASTNodeMain(ASTNode* stmt_list) {
	ASTNode* main = new ASTNode();
	main->setType(ASTNodeType_Main);
	main->setChild(ASTNodeMain_Stmtlist, stmt_list);
	return main;
}

ASTNode* ASTNodeFactory::makeASTNodeProgram(vector<ASTNode*> functions) {
	int len = functions.size();
	ASTNode* program = new ASTNode(len + 1);
	program->setValue(len);
	program->setType(ASTNodeType_Program);

	for (int i = 0; i < len; i++) {
		program->setChild(i, functions[i]);
	}
	return program;
}