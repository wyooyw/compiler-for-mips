#pragma once
#include "ASTNode.h"
#include "SignTable.h"
class FunctionInclineOptimizer {
public:
	FunctionInclineOptimizer(SignTable* signTable);
	void optimize(ASTNode* program);
	bool checkFunc(ASTNode* func);
	bool checkFuncReturn(ASTNode* ret);
private:
	SignTable* signTable;
	int findReturn = 0;
};