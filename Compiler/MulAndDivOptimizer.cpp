#include "MulAndDivOptimizer.h"
void MulAndDivOptimizer::optimize(ASTNode* program) {
	
}
void MulAndDivOptimizer::optimize_factor(ASTNode* factor) {
	int v_right = 0;
	int v_left_right = 0;
	if (factor->getValue() == '*') {
		ASTNode* right = factor->getChild(ASTNode_Factor_Right);
		if (right->getType() == ASTNodeType_Int) {
			ASTNode* left = factor->getChild(ASTNode_Factor_Left);
			if (left->getType() == ASTNodeType_Factor &&
				left->getValue() == '/') {
				ASTNode* left_right = left->getChild(ASTNode_Factor_Right);
				if (left_right->getType() == ASTNodeType_Int &&
					right->getValue()== left_right->getValue()) {
					//可以优化



				}

			}
		}
		
	}
}