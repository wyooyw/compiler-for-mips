#include "ASTNode.h"
#pragma once
class ASTNodeFactory {
public:
	
	ASTNode* makeASTNodeInt(int value);
	ASTNode* makeASTNodeChar(char c);
	ASTNode* makeASTNodeVar(char* name);

	ASTNode* makeASTNodeFactor(int sign, ASTNode* left, ASTNode* right);
};