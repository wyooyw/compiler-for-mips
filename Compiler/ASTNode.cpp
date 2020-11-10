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

string ASTNode::getValueStr() {
	return value_s;
}
void ASTNode::setValueStr(char* n) {
	value_s = n;
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
		printf("Char--'%c\n'",value);
		break;
	case ASTNodeType_Factor:
		printf("Factor--%c\n",value);
		child[ASTNode_Factor_Left]->print();
		child[ASTNode_Factor_Right]->print();
		break;
	default:
		printf("other,type=%d\n",type);

	}
}