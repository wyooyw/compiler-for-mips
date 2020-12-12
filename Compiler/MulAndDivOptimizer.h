#pragma once
#include "ASTNode.h"
/*
³ËnÔÙ³ýn£¬ÔòÏûµô
*/
class MulAndDivOptimizer {
public:
	void optimize(ASTNode* program);
private:
	void optimize_factor_mod(ASTNode* factor);
	void optimize_factor_mul(ASTNode* factor);
};