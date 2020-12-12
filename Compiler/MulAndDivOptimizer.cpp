#include "MulAndDivOptimizer.h"
void MulAndDivOptimizer::optimize(ASTNode* astnode) {
	int type = astnode->getType();
	switch (type) {
	case ASTNodeType_Factor:
		optimize_factor_mod(astnode);
		optimize_factor_mul(astnode);
		optimize(astnode->getChild(ASTNode_Factor_Left));
		optimize(astnode->getChild(ASTNode_Factor_Right));
		break;
	case ASTNodeType_Assign:
		optimize(astnode->getChild(ASTNode_Assign_Left));
		optimize(astnode->getChild(ASTNode_Assign_Right));
		break;
	case ASTNodeType_Print:
		optimize(astnode->getChild(ASTNode_Print_Expression));
		break;
	case ASTNodeType_PrintExpression:
		optimize(astnode->getChild(ASTNode_Print_Expression));
		break;
	case ASTNodeType_StmtList:
		for (int i = 0; i < astnode->getValue(); i++) {
			optimize(astnode->getChild(i));
		}
		break;
	case ASTNodeType_Main:
		optimize(astnode->getChild(ASTNodeMain_Stmtlist));
		break;
	case ASTNodeType_Program:
		for (int i = 0; i < astnode->getValue(); i++) {
			optimize(astnode->getChild(i));
		}
		break;
	case ASTNodeType_If:
		optimize(astnode->getChild(ASTNode_If_Cond));
		optimize(astnode->getChild(ASTNode_If_Stmt));
		break;
	case ASTNodeType_Ifelse:
		optimize(astnode->getChild(ASTNode_Ifelse_Cond));
		optimize(astnode->getChild(ASTNode_Ifelse_Stmt));
		optimize(astnode->getChild(ASTNode_Ifelse_ElseStmt));
		break;
	case ASTNodeType_Condition:
		optimize(astnode->getChild(ASTNode_Condition_Left));
		optimize(astnode->getChild(ASTNode_Condition_Right));
		break;
	case ASTNodeType_While:
		optimize(astnode->getChild(ASTNode_While_Cond));
		optimize(astnode->getChild(ASTNode_While_Stmt));
		break;
	case ASTNodeType_For:
		optimize(astnode->getChild(ASTNode_For_Init));
		optimize(astnode->getChild(ASTNode_For_Cond));
		optimize(astnode->getChild(ASTNode_For_Update));
		optimize(astnode->getChild(ASTNode_For_Stmt));
		break;
	case ASTNodeType_Switch:
		optimize(astnode->getChild(ASTNode_Switch_Expression));
		optimize(astnode->getChild(ASTNode_Switch_Caselist));

		optimize(astnode->getChild(ASTNode_Switch_Default));
		break;
	case ASTNodeType_Caselist:
		for (int i = 0; i < astnode->getValue(); i++) {
			optimize(astnode->getChild(i));
		}
		break;
	case ASTNodeType_Case:
		optimize(astnode->getChild(ASTNode_Case_Stmt));
		break;
	case ASTNodeType_Func:
		optimize(astnode->getChild(ASTNode_Func_StmtList));
		break;
	case ASTNodeType_Return:
		if (astnode->getValue() == 1) {
			optimize(astnode->getChild(ASTNode_Return_Expression));
		}
		break;
	case ASTNodeType_Call:
		optimize(astnode->getChild(ASTNode_Call_ValueList));
		break;
	case ASTNodeType_ValueList:
		for (int i = 0; i < astnode->getValue(); i++) {
			optimize(astnode->getChild(i));
		}
		break;
	case ASTNodeType_Arr:
		optimize(astnode->getChild(ASTNode_Arr_Express));
		break;
	case ASTNodeType_Arr2:
		optimize(astnode->getChild(ASTNode_Arr2_Express1));
		optimize(astnode->getChild(ASTNode_Arr2_Express2));
		break;
	default:
		break;

	}
}
void MulAndDivOptimizer::optimize_factor_mod(ASTNode* factor) {
	if (factor->getType() != ASTNodeType_Factor) {
		return;
	}
	int v_right = 0;
	int v_left_right = 0;
	if (factor->getValue() == '-') {
		ASTNode* left = factor->getChild(ASTNode_Factor_Left);
		int left_type = left->getType();
		if (left_type == ASTNodeType_Var) {
			char* left_name = left->getValueStr();//左节点名字
			ASTNode* right = factor->getChild(ASTNode_Factor_Right);
			if (right->getType() == ASTNodeType_Factor &&
				right->getValue()=='*') {
				ASTNode* right_left = right->getChild(ASTNode_Factor_Left);
				ASTNode* right_right = right->getChild(ASTNode_Factor_Right);
				int right_right_value = right_right->getValue();

				if (right_left->getType() == ASTNodeType_Factor &&
					right_left->getValue() == '/') {
					ASTNode* right_left_left = right_left->getChild(ASTNode_Factor_Left);
					ASTNode* right_left_right = right_left->getChild(ASTNode_Factor_Right);
					if (right_left_left->getType() == ASTNodeType_Var);
					char* right_left_left_name = right_left_left->getValueStr();
					int right_left_right_value = right_left_right->getValue();
					if (strcmp(left_name, right_left_left_name) == 0 &&
						right_right_value == right_left_right_value) {
						//可以优化
						printf("yeah!\n");
						factor->setValue('%');
						factor->setChild(ASTNode_Factor_Right, right_right);
						right_right->setFather(factor);
						right_right->setChildIndex(ASTNode_Factor_Right);



					}



				}


			}



		}
	}
}
void MulAndDivOptimizer::optimize_factor_mul(ASTNode* factor) {
	if (factor->getType() != ASTNodeType_Factor) {
		return;
	}
	if (factor->getValue() == '*') {
		ASTNode* right = factor->getChild(ASTNode_Factor_Right);
		if (right->getType() == ASTNodeType_Int) {
			printf("optimize!\n");
			int value = right->getValue();
			if (value == 2) {
				factor->setValue('<');//左移
				right->setValue(1);
			}
		}
	}
	else if (factor->getValue() == '/') {
		ASTNode* right = factor->getChild(ASTNode_Factor_Right);
		if (right->getType() == ASTNodeType_Int) {
			printf("optimize!\n");
			int value = right->getValue();
			if (value == 2) {
				factor->setValue('>');//右移
				right->setValue(1);
			}
		}
	}
}