#include "FunctionInclineOptimizer.h"
FunctionInclineOptimizer::FunctionInclineOptimizer(SignTable* signTable) {
	this->signTable = signTable;
}
bool FunctionInclineOptimizer::checkFunc(ASTNode* astnode) {
	if (findReturn >= 1) {
		return false;
	}

	int type = astnode->getType();
	switch (type) {
	case ASTNodeType_StmtList:
		for (int i = 0; i < astnode->getValue(); i++) {
			if (!checkFunc(astnode->getChild(i))) {
				return false;
			}
		}
		break;
	case ASTNodeType_Main:
		if (!checkFunc(astnode->getChild(ASTNodeMain_Stmtlist))) {
			return false;
		}
		break;
	case ASTNodeType_If:
		if (!checkFunc(astnode->getChild(ASTNode_If_Stmt))) {
			return false;
		}
		break;
	case ASTNodeType_Ifelse:
		if (!checkFunc(astnode->getChild(ASTNode_Ifelse_Stmt))) {
			return false;
		}
		if (!checkFunc(astnode->getChild(ASTNode_Ifelse_ElseStmt))) {
			return false;
		}
		break;
	case ASTNodeType_While:
		if (!checkFunc(astnode->getChild(ASTNode_While_Stmt))) {
			return false;
		}
		break;
	case ASTNodeType_For:
		if (!checkFunc(astnode->getChild(ASTNode_For_Stmt))) {
			return false;
		}
		break;
	case ASTNodeType_Switch:
		if (!checkFunc(astnode->getChild(ASTNode_Switch_Caselist))) {
			return false;
		}
		if (!checkFunc(astnode->getChild(ASTNode_Switch_Default))) {
			return false;
		}
		break;
	case ASTNodeType_Caselist:
		for (int i = 0; i < astnode->getValue(); i++) {
			if (!checkFunc(astnode->getChild(i))) {
				return false;
			}
		}
		break;
	case ASTNodeType_Case:
		if (!checkFunc(astnode->getChild(ASTNode_Case_Stmt))) {
			return false;
		}
		break;
	case ASTNodeType_Func:
		if (!checkFunc(astnode->getChild(ASTNode_Func_StmtList))) {
			return false;
		}
		break;
	case ASTNodeType_Return:
		findReturn++;
		break;
	default:
		break;
	}
	return true;
}