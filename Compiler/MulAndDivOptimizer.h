#pragma once
#include "ASTNode.h"
/*
��n�ٳ�n��������
*/
class MulAndDivOptimizer {
public:
	void optimize(ASTNode* program);
private:
	void optimize_factor(ASTNode* factor);
};