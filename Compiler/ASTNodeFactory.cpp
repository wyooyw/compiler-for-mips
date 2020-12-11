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

ASTNode* ASTNodeFactory::makeASTNodeCondition(int sign, ASTNode* left, ASTNode* right) {
	ASTNode* cond = new ASTNode();
	cond->setType(ASTNodeType_Condition);
	cond->setValue(sign);
	cond->setChild(ASTNode_Condition_Left, left);
	cond->setChild(ASTNode_Condition_Right,right);
	return cond;
}
ASTNode* ASTNodeFactory::makeASTNodeIf(ASTNode* condition, ASTNode* stmt) {
	ASTNode* astnode_if = new ASTNode();
	astnode_if->setType(ASTNodeType_If);
	astnode_if->setChild(ASTNode_If_Cond, condition);
	astnode_if->setChild(ASTNode_If_Stmt, stmt);
	return astnode_if;
}
ASTNode* ASTNodeFactory::makeASTNodeIfelse(ASTNode* condition, ASTNode* stmt, ASTNode* elsestmt) {
	ASTNode* astnode_ifelse = new ASTNode();
	astnode_ifelse->setType(ASTNodeType_Ifelse);
	astnode_ifelse->setChild(ASTNode_Ifelse_Cond, condition);
	astnode_ifelse->setChild(ASTNode_Ifelse_Stmt, stmt);
	astnode_ifelse->setChild(ASTNode_Ifelse_ElseStmt, elsestmt);

	return astnode_ifelse;
}
ASTNode* ASTNodeFactory::makeASTNodeWhile(ASTNode* condition, ASTNode* stmt) {
	ASTNode* astnode_while = new ASTNode();
	astnode_while->setType(ASTNodeType_While);
	astnode_while->setChild(ASTNode_While_Cond, condition);
	astnode_while->setChild(ASTNode_While_Stmt, stmt);
	return astnode_while;
}
ASTNode* ASTNodeFactory::makeASTNodeSwitch(ASTNode* expression, ASTNode* caselist,ASTNode* astnode_default) {
	ASTNode* astnode_switch = new ASTNode();
	astnode_switch->setType(ASTNodeType_Switch);
	astnode_switch->setChild(ASTNode_Switch_Expression, expression);
	astnode_switch->setChild(ASTNode_Switch_Caselist, caselist);
	astnode_switch->setChild(ASTNode_Switch_Default, astnode_default);
	return astnode_switch;
}
ASTNode* ASTNodeFactory::makeASTNodeCaselist(vector<ASTNode*> cases) {
	int len = cases.size();
	ASTNode* astnode_caselist = new ASTNode(len+1);
	astnode_caselist->setValue(len);
	astnode_caselist->setType(ASTNodeType_Caselist);
	for (int i = 0; i < len; i++) {
		astnode_caselist->setChild(i, cases[i]);
	}
	return astnode_caselist;
}
ASTNode* ASTNodeFactory::makeASTNodeCase(int constvalue, ASTNode* stmt) {
	ASTNode* astnode_case = new ASTNode();
	astnode_case->setType(ASTNodeType_Case);
	astnode_case->setValue(constvalue);
	astnode_case->setChild(ASTNode_Case_Stmt, stmt);
	return astnode_case;
}

ASTNode* ASTNodeFactory::makeASTNodeFor(ASTNode* init, ASTNode* cond, ASTNode* update, ASTNode* stmt) {
	ASTNode* astnode_for = new ASTNode(4);
	astnode_for->setType(ASTNodeType_For);
	astnode_for->setChild(ASTNode_For_Init, init);
	astnode_for->setChild(ASTNode_For_Cond, cond);
	astnode_for->setChild(ASTNode_For_Update, update);
	astnode_for->setChild(ASTNode_For_Stmt, stmt);
	return astnode_for;
}

ASTNode* ASTNodeFactory::makeASTNodeFunc(char* name, ASTNode* paraListm, ASTNode* stmtList) {
	ASTNode* astnode_func = new ASTNode();
	astnode_func->setType(ASTNodeType_Func);
	astnode_func->setValueStr(name);
	astnode_func->setChild(ASTNode_Func_ParaList, paraListm);
	astnode_func->setChild(ASTNode_Func_StmtList, stmtList);
	return astnode_func;
}
ASTNode* ASTNodeFactory::makeASTNodeParaList(vector<ASTNode*> paras) {
	int len = paras.size();

	ASTNode* astnode_paralist = new ASTNode(len+1);
	astnode_paralist->setValue(len);
	astnode_paralist->setType(ASTNodeType_ParaList);
	for (int i = 0; i < len; i++) {
		astnode_paralist->setChild(i, paras[i]);
	}
	return astnode_paralist;
	
}
ASTNode* ASTNodeFactory::makeASTNodePara(char* name) {
	ASTNode* para = new ASTNode();
	para->setType(ASTNodeType_Para);
	para->setValueStr(name);
	return para;
};

//返回语句
ASTNode* ASTNodeFactory::makeASTNodeReturn(ASTNode* expression) {
	ASTNode* ret = new ASTNode();
	ret->setType(ASTNodeType_Return);
	ret->setValue(1);
	ret->setChild(ASTNode_Return_Expression,expression);
	return ret;
}
ASTNode* ASTNodeFactory::makeASTNodeReturn() {
	ASTNode* ret = new ASTNode();
	ret->setType(ASTNodeType_Return);
	ret->setValue(0);
	return ret;
}

ASTNode* ASTNodeFactory::makeASTNodeCall(char* name,ASTNode* valuelist) {
	ASTNode* call = new ASTNode();
	call->setType(ASTNodeType_Call);
	call->setValueStr(name);
	call->setChild(ASTNode_Call_ValueList, valuelist);
	return call;
}

ASTNode* ASTNodeFactory::makeASTNodeValueList(vector<ASTNode*> expressions) {
	int len = expressions.size();
	ASTNode* val_list = new ASTNode(len + 1);
	val_list->setType(ASTNodeType_ValueList);
	val_list->setValue(len);
	for (int i = 0; i < len; i++) {
		val_list->setChild(i, expressions[i]);
	}
	return val_list;
}

//一维数组和二维数组
ASTNode* ASTNodeFactory::makeASTNodeArr(char* name, ASTNode* expression) {
	ASTNode* arr = new ASTNode();
	arr->setType(ASTNodeType_Arr);
	arr->setValueStr(name);
	arr->setChild(ASTNode_Arr_Express, expression);
	return arr;
}
ASTNode* ASTNodeFactory::makeASTNodeArr2(char* name, ASTNode* expression1, ASTNode* expression2) {
	ASTNode* arr = new ASTNode();
	arr->setType(ASTNodeType_Arr2);
	arr->setValueStr(name);
	arr->setChild(ASTNode_Arr2_Express1, expression1);
	arr->setChild(ASTNode_Arr2_Express2, expression2);
	return arr;
}

ASTNode* ASTNodeFactory::makeASTNodeNull() {
	ASTNode* astnode_null = new ASTNode();
	astnode_null->setType(ASTNodeType_Null);
	return astnode_null;
}