#include "ASTNode.h"

ASTNode::ASTNode() {
	child = (ASTNode**)malloc(5 * sizeof(ASTNode*));
}

ASTNode::ASTNode(int childlen) {
	child = (ASTNode**)malloc(childlen*sizeof(ASTNode*));
}

int ASTNode::getType() {
	return type;
}
void ASTNode::setType(int t) {
	type = t;
}

char* ASTNode::getValueStr() {
	return value_s;
}
void ASTNode::setValueStr(char* n) {
	strcpy(value_s,n);
}

ASTNode* ASTNode::getChild(int index) {
	return child[index];
}
void ASTNode::setChild(int index, ASTNode* ch) {
	child[index] = ch;
}

int ASTNode::getValue() {
	return value;
}
void ASTNode::setValue(int val) {
	value=val;
}

void ASTNode::print() {
	switch (type) {
	case ASTNodeType_Int:
		printf("Int--%d\n",value);
		break;
	case ASTNodeType_Char:
		printf("Char--'%c'\n",value);
		break;
	case ASTNodeType_Str:
		printf("String--%s\n", value_s);
		break;
	case ASTNodeType_Var:
		printf("Var--%s\n", value_s);
		break;
	case ASTNodeType_Factor:
		printf("Factor--%c\n",value);
		//printf("%lld\n", child[ASTNode_Factor_Left]);
		child[ASTNode_Factor_Left]->print();
		child[ASTNode_Factor_Right]->print();
		break;
	case ASTNodeType_Assign:
		printf("Assign\n");
		child[ASTNode_Assign_Left]->print();
		child[ASTNode_Assign_Right]->print();
		break;
	case ASTNodeType_Print:
		printf("Print\n");
		child[ASTNode_Print_String]->print();
		child[ASTNode_Print_Expression]->print();
		break;
	case ASTNodeType_PrintString:
		printf("PrintString\n");
		child[ASTNode_Print_String]->print();
		break;
	case ASTNodeType_PrintExpression:
		printf("PrintExpression\n");
		child[ASTNode_Print_Expression]->print();
		break;
	case ASTNodeType_Scanf:
		printf("Scanf\n");
		child[ASTNode_Scanf_Var]->print();
		break;
	case ASTNodeType_StmtList:
		printf("StmtList--%d\n",value);
		for (int i = 0; i < value; i++) {
			child[i]->print();
		}
		break;
	case ASTNodeType_Main:
		printf("Main\n");
		child[ASTNodeMain_Stmtlist]->print();
		break;
	case ASTNodeType_Program:
		printf("Program\n");
		for (int i = 0; i < value; i++) {
			child[i]->print();
		}
		break;
	case ASTNodeType_If:
		printf("If\n");
		child[ASTNode_If_Cond]->print();
		child[ASTNode_If_Stmt]->print();
		break;
	case ASTNodeType_Ifelse:
		printf("If_else\n");
		child[ASTNode_Ifelse_Cond]->print();
		child[ASTNode_Ifelse_Stmt]->print();
		child[ASTNode_Ifelse_ElseStmt]->print();
		break;
	case ASTNodeType_Condition:
		printf("Condition:");
		if (value == LSS) printf("<\n");
		else if (value == LEQ) printf("<=\n");
		else if (value == GRE) printf(">\n");
		else if (value == EQL) printf("==\n");
		else if (value == GEQ) printf(">=\n");
		else printf("!=\n");

		child[ASTNode_Condition_Left]->print();
		child[ASTNode_Condition_Right]->print();
		break;
	case ASTNodeType_While:
		printf("While\n");
		child[ASTNode_While_Cond]->print();
		child[ASTNode_While_Stmt]->print();
		break;
	case ASTNodeType_For:
		printf("For\n");
		child[ASTNode_For_Init]->print();
		child[ASTNode_For_Cond]->print();
		child[ASTNode_For_Update]->print();
		child[ASTNode_For_Stmt]->print();
		break;
	case ASTNodeType_Switch:
		printf("Switch\n");
		child[ASTNode_Switch_Expression]->print();
		child[ASTNode_Switch_Caselist]->print();

		printf("Default\n");
		child[ASTNode_Switch_Default]->print();
		break;
	case ASTNodeType_Caselist:
		printf("Caselist\n");
		for (int i = 0; i < getValue(); i++) {
			child[i]->print();
		}
		break;
	case ASTNodeType_Case:
		printf("Case %d\n",getValue());
		child[ASTNode_Case_Stmt]->print();
		break;
	case ASTNodeType_Func:
		printf("Func--%s\n", getValueStr());
		child[ASTNode_Func_ParaList]->print();
		child[ASTNode_Func_StmtList]->print();
		break;
	case ASTNodeType_ParaList:
		printf("ParaList--num:%d\n", getValue());
		for (int i = 0; i < value; i++) {
			child[i]->print();
		}
		break;
	case ASTNodeType_Para:
		printf("Para--%s\n", value_s);
		break;
	case ASTNodeType_Return:
		printf("Return--%d\n", value);
		if (value == 1) {
			child[ASTNode_Return_Expression]->print();
		}
		break;
	case ASTNodeType_Call:
		printf("Call\n");
		child[ASTNode_Call_ValueList]->print();
		break;
	case ASTNodeType_ValueList:
		printf("ValueList--%d\n",value);
		for (int i = 0; i < value; i++) {
			child[i]->print();
		}
		break;
	case ASTNodeType_Arr:
		printf("Arr--%s\n", value_s);
		child[ASTNode_Arr_Express]->print();
		break;
	case ASTNodeType_Arr2:
		printf("Arr2--%s\n", value_s);
		child[ASTNode_Arr2_Express1]->print();
		child[ASTNode_Arr2_Express2]->print();
		break;
	default:
		printf("other,type=%d\n",type);

	}
}