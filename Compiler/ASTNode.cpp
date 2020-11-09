#include "ASTNode.h"

int ASTNode::getType() {
	return type;
}
void ASTNode::setType(int t) {
	type = t;
}

string ASTNode::getName() {
	return name;
}
void ASTNode::setName(char* n) {
	name = n;
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
	printf("%d",type);
	switch (type) {
	default:

	}
}