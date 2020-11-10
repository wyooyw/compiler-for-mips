#include "ASTNode.h"
#include "Word.h"
#pragma once
class ASTNodeFactory {
public:
	
	ASTNode* makeASTNodeInt(int value);
	ASTNode* makeASTNodeChar(char c);
	ASTNode* makeASTNodeVar(char* name);

	ASTNode* makeASTNodeFactor(int sign, ASTNode* left, ASTNode* right);

	ASTNode* makeASTNodeAssign(ASTNode* left, ASTNode* right);
	ASTNode* makeASTNodePrint(ASTNode* expression);
	ASTNode* makeASTNodePrint(char* str);
	ASTNode* makeASTNodePrint(char* str, ASTNode* expression);
	ASTNode* makeASTNodeScanf(ASTNode* var);

};