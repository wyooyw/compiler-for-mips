#include "ASTNodeFactory.h"

ASTNode* ASTNodeFactory::makeASTNodeFactor(int sign, ASTNode* left,ASTNode *right) {
	ASTNode* factor = new ASTNode();
	factor->setType(ASTNodeType_Factor);
	factor->setValue(sign);
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
	idenfr->setName(name);
	return idenfr;
}