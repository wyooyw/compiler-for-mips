#pragma once
#include "ASTNode.h"
/*
³ËnÔÙ³ın£¬ÔòÏûµô
*/
class MulAndDivOptimizer {
public:
	void optimize(ASTNode* program);
private:
	void optimize_factor(ASTNode* factor);
};