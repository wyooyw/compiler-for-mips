#include "ASTNode.h"

ASTNode::ASTNode() {
	child = (ASTNode**)malloc(3 * sizeof(ASTNode*));
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
	default:
		printf("other,type=%d\n",type);

	}
}