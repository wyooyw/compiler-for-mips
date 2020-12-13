#include "LoopExtendOptimizer.h"
LoopExtendOptimizer::LoopExtendOptimizer(SignTable* signTable) {
	this->signTable = signTable;
}
void LoopExtendOptimizer::optimize(ASTNode* astnode) {
	int type = astnode->getType();
	switch (type) {
	case ASTNodeType_Assign:
		checkAssign(astnode);
		break;
	case ASTNodeType_StmtList:
		for (int i = 0; i < astnode->getValue(); i++) {
			optimize(astnode->getChild(i));
		}
		break;
	case ASTNodeType_Main:

		signTable->setCurrent(astnode->getValueStr());//设置符号表环境
		//printf("%s\n", astnode->getValueStr());
		initRecord();		//记录初始化的变量到record中

		//safe = 1;

		optimize(astnode->getChild(ASTNodeMain_Stmtlist));

		break;
	case ASTNodeType_Program:
		for (int i = 0; i < astnode->getValue(); i++) {
			optimize(astnode->getChild(i));
		}
		break;
	case ASTNodeType_If:
		//optimize(astnode->getChild(ASTNode_If_Cond));
		optimize(astnode->getChild(ASTNode_If_Stmt));
		break;
	case ASTNodeType_Ifelse:
		//optimize(astnode->getChild(ASTNode_Ifelse_Cond));
		optimize(astnode->getChild(ASTNode_Ifelse_Stmt));
		optimize(astnode->getChild(ASTNode_Ifelse_ElseStmt));
		break;
	case ASTNodeType_While:
		checkWhile(astnode);
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
		signTable->setCurrent(astnode->getValueStr());//设置符号表环境

		initRecord();		//记录初始化的变量到record中

		optimize(astnode->getChild(ASTNode_Func_StmtList));
		break;
	default:
		break;

	}
}

void LoopExtendOptimizer::initRecord() {
	record.erase(record.begin(), record.end());
	list<Sign*>* signs = signTable->getFuncSigns();
	list<Sign*>::iterator iter;
	for (iter = signs->begin(); iter != signs->end(); iter++) {
		Sign* sign = *iter;
		
		//如果是常量或者是有初始值的变量(非数组)
		if (sign->getCategory() == C_CONST ||
			(sign->getCategory() == C_VAR && sign->hasInitValue() && sign->getDimen()==0)) {
			record[sign->getName()] = sign->getInitValue();
		}
	}
}


bool LoopExtendOptimizer::getDefineValue(char* name,int &value) {
	map<string, int>::iterator iter = record.find(name);
	if (iter == record.end()) {
		return false;
	}
	value = iter->second;
	return true;
}

void LoopExtendOptimizer::checkAssign(ASTNode* assign) {
	if (assign->getType() != ASTNodeType_Assign) {
		return;
	}
	ASTNode* left = assign->getChild(ASTNode_Assign_Left);
	ASTNode* right = assign->getChild(ASTNode_Assign_Right);
	int value;

	if (left->getType() != ASTNodeType_Var) {
		return;
	}
	char* varname = left->getValueStr();
	if (getDefineValue(varname, value)) {
		//左端是变量，且之前初值
		if (right->getType() == ASTNodeType_Int) {
			//右端是常数
			value = right->getValue();
			record[left->getValueStr()] = value;		//更换数值
		}
		else {
			//右端不是常数
			record.erase(varname);
		}
	}
	else {
		if (right->getType() == ASTNodeType_Int) {
			//右端是常数
			value = right->getValue();
			record[left->getValueStr()] = value;		//添加数值
		}
	}
}
void LoopExtendOptimizer::checkWhile(ASTNode* astnode_while) {
	ASTNode* cond = astnode_while->getChild(ASTNode_While_Cond);
	ASTNode* stmt = astnode_while->getChild(ASTNode_While_Stmt);

	char* varname;
	int cmp,initvalue,targetvalue,step;
	int extend;

	//检查条件是否符合格式
	if (!checkWhileCond(cond, varname, cmp, targetvalue)) {
		return;
	}

	//检查当前是否为一确定的值
	if (!getDefineValue(varname, initvalue)) {
		return;
	}

	checkWhileStmt_findUpdate = 0;

	//检查while中的语句是否符合格式
	if (!checkWhileStmt(stmt, varname,step)) {
		return;
	}

	//找到了0个或多于1个的 更新循环变量的语句
	if (checkWhileStmt_findUpdate != 1) {
		return;
	}
	
	if (cmp == EQL) {
		return;
	}

	if (cmp == LEQ) {
		cmp = LSS;
		targetvalue += 1;
	}

	if (cmp == GEQ) {
		cmp = GRE;
		targetvalue -= 1;
	}

	if (cmp == LSS) {
		if (initvalue >= targetvalue)  return;
		if (step <= 0) return;
		//可
		//x=initvalue;x<targetvalue;x=x+step;
		if (((targetvalue - initvalue) % step) == 0) {
			extend = (targetvalue - initvalue) / step;
		}
		else {
			extend = (targetvalue - initvalue) / step + 1;
		}
		
	}
	else if (cmp == GRE) {
		if (initvalue <= targetvalue)  return;
		if (step >= 0) return;
		//可
		//x=initvalue;x>targetvalue;x=x-(-step);
		if (((initvalue - targetvalue) % (-step)) == 0) {
			extend = (initvalue - targetvalue) / (-step);
		}
		else {
			extend = (initvalue - targetvalue) / (-step) + 1;
		}
	}
	else if (cmp == NEQ) {
		if (initvalue == targetvalue) {
			//这里就是死循环了，不会出现
			return;
		}
		if (initvalue > targetvalue) {
			if (step >= 0) return;
			if (((initvalue - targetvalue) % (-step)) == 0) {
				extend = (initvalue - targetvalue) / (-step);
			}
			else {
				extend = (initvalue - targetvalue) / (-step) + 1;
			}
		}
		else {
			if (step <= 0) return;
			if (((targetvalue - initvalue) % step) == 0) {
				extend = (targetvalue - initvalue) / step;
			}
			else {
				extend = (targetvalue - initvalue) / step + 1;
			}
		}
	}
	else {
		return;
	}


	//可以真正开始循环展开了
	printf("loop extend!!%d\n", extend);
	ASTNode* stmtlist = new ASTNode(extend);
	stmtlist->setType(ASTNodeType_StmtList);
	stmtlist->setValue(extend);
	for (int i = 0; i < extend; i++) {
		stmtlist->setChild(i,stmt);
	}

	replace(astnode_while, stmtlist);//用后者替换前者
	return;

}
bool LoopExtendOptimizer::checkWhileCond(ASTNode* cond, char*& name, int& cmp, int& value) {
	ASTNode* left = cond->getChild(ASTNode_Condition_Left);
	ASTNode* right = cond->getChild(ASTNode_Condition_Right);
	if (left->getType() == ASTNodeType_Var && right->getType() == ASTNodeType_Int) {
		name = left->getValueStr();
		value = right->getValue();
		cmp = cond->getValue();
		return true;

	}
	else if (right->getType() == ASTNodeType_Var && left->getType() == ASTNodeType_Int) {
		name = right->getValueStr();
		value = left->getValue();
		cmp = cond->getValue();
		return true;
	}
	return false;
}
bool LoopExtendOptimizer::checkWhileStmt(ASTNode* stmt,char *varname, int& step) {
	//只能有且只有一句vanname在左的赋值语句,且使用i=i+x或i=i-x来更新。
	ASTNode* left = stmt->getChild(ASTNode_Assign_Left);
	ASTNode* right = stmt->getChild(ASTNode_Assign_Right);
	switch (stmt->getType()) {
		case ASTNodeType_Assign :

			if (left->getType() == ASTNodeType_Var &&
				strcmp(left->getValueStr(), varname) == 0) {
				checkWhileStmt_findUpdate++;
				if (checkWhileStmt_findUpdate > 1) {
					return false;
				}
				if (!checkWhileStmtAssignRight(right, varname, step)) {
					return false;
				}
			}
			break;
		case ASTNodeType_StmtList:
			for (int i = 0; i < stmt->getValue(); i++) {
				if (!checkWhileStmt(stmt->getChild(i), varname, step)) {
					return false;
				}
			}
			break;
		case ASTNodeType_While:
			return false;
			break;
		case ASTNodeType_For:
			return false;
			break;
		case ASTNodeType_If:
			return false;
			break;
		case ASTNodeType_Switch:
			return false;
			break;
		default:
			break;
	}
	return true;
}

bool LoopExtendOptimizer::checkWhileStmtAssignRight(ASTNode* factor, char* varname, int& step) {
	if (factor->getType() == ASTNodeType_Factor
		&& (factor->getValue()=='+' || factor->getValue() == '-')) {
		ASTNode* left = factor->getChild(ASTNode_Factor_Left);
		ASTNode* right = factor->getChild(ASTNode_Factor_Right);
		if (left->getType() == ASTNodeType_Var 
			&& right->getType()==ASTNodeType_Int) {
			if (factor->getValue() == '+') {
				step = right->getValue();
			}
			else {
				step = -right->getValue();
			}
			return true;
		}
	}
	return false;
}

void LoopExtendOptimizer::replace(ASTNode* astnode_old,ASTNode* astnode_new) {
	ASTNode* father = astnode_old->getFather();
	int childindex = astnode_old->getChildIndex();
	father->setChild(childindex, astnode_new);
	delete astnode_old;
}
/*void LoopExtendOptimizer::var2int(ASTNode* factor) {
	if (factor->getType() != ASTNodeType_Factor) {
		return;
	}
	ASTNode* left = factor->getChild(ASTNode_Factor_Left);
	ASTNode* right = factor->getChild(ASTNode_Factor_Right);
	if (left->getType() == ASTNodeType_Var) {
		int value;
		if (getDefineValue(left->getValueStr(), value)) {
			left->setType(ASTNodeType_Int);
			left->setValue(value);
		}
	}
	if (right->getType() == ASTNodeType_Var) {
		int value;
		if (getDefineValue(left->getValueStr(), value)) {
			left->setType(ASTNodeType_Int);
			left->setValue(value);
		}
	}
}*/