#include "TargetCode.h"

//语句
void TargetCode::t_stmt(ASTNode* astnode_stmt) {
	switch (astnode_stmt->getType()) {
	case ASTNodeType_StmtList:
		t_stmt_list(astnode_stmt);
		break;
	case ASTNodeType_Assign:
		t_assign(astnode_stmt);
		break;
	case ASTNodeType_Print:
		t_print(astnode_stmt);
		break;
	case ASTNodeType_PrintString:
		t_printstring(astnode_stmt);
		break;
	case ASTNodeType_PrintExpression:
		t_printexpression(astnode_stmt);
		break;
	case ASTNodeType_Scanf:
		t_scanf(astnode_stmt);
		break;
	case ASTNodeType_If:
		t_if(astnode_stmt);
		break;
	case ASTNodeType_Ifelse:
		t_ifelse(astnode_stmt);
		break;
	case ASTNodeType_While:
		t_while(astnode_stmt);
		break;
	case ASTNodeType_For:
		t_for(astnode_stmt);
		break;
	case ASTNodeType_Switch:
		t_switch(astnode_stmt);
		break;
	case ASTNodeType_Call:
		t_func_call(astnode_stmt);
		break;
	case ASTNodeType_Return:
		t_ret(astnode_stmt);
		break;
	default:
		break;
	}
}

void TargetCode::t_stmt_list(ASTNode* astnode_stmtlist) {
	int len = astnode_stmtlist->getValue();
	ASTNode* astnode_stmt;
	for (int i = 0; i < len; i++) {
		astnode_stmt = astnode_stmtlist->getChild(i);
		t_stmt(astnode_stmt);
	}
}

//IF语句
void TargetCode::t_if(ASTNode* astnode_if) {
	ASTNode* cond = astnode_if->getChild(ASTNode_If_Cond);
	ASTNode* stmt = astnode_if->getChild(ASTNode_If_Stmt);

	int label = t_cond(cond);

	regManager->record();		//执行if前保存当前寄存器环境

	t_stmt(stmt);
	//Output::note("before recover");
	regManager->freeAllAndRecover();		//回复之前寄存器环境，防止数据流合并出错
	//Output::note("after recover");
	Output::label(label);
}

//IF_ELSE语句
void TargetCode::t_ifelse(ASTNode* astnode_ifelse) {
	ASTNode* cond = astnode_ifelse->getChild(ASTNode_Ifelse_Cond);
	ASTNode* stmt = astnode_ifelse->getChild(ASTNode_Ifelse_Stmt);
	ASTNode* stmt_else = astnode_ifelse->getChild(ASTNode_Ifelse_ElseStmt);

	int label1 = t_cond(cond);

	regManager->record();		//执行if前保存当前寄存器环境

	t_stmt(stmt);

	regManager->freeAllAndRecover();		//回复之前寄存器环境，防止数据流合并出错

	int label2 = allocLabel();
	Output::j(label2);

	Output::label(label1);

	regManager->record();		//执行if前保存当前寄存器环境

	t_stmt(stmt_else);

	regManager->freeAllAndRecover();		//回复之前寄存器环境，防止数据流合并出错

	Output::label(label2);
}

//SWITCH语句
void TargetCode::t_switch(ASTNode* astnode_switch) {
	ASTNode* astnode_expr = astnode_switch->getChild(ASTNode_Switch_Expression);
	ASTNode* astnode_caselist = astnode_switch->getChild(ASTNode_Switch_Caselist);
	ASTNode* astnode_default = astnode_switch->getChild(ASTNode_Switch_Default);

	Output::note("SWITCH");

	int type;
	int reg_expr = t_factor(astnode_expr,type);
	int reg_const = regManager->allocTmpReg();
	
	int case_len = astnode_caselist->getValue();

	ASTNode* astnode_case;
	int labels[MAX_CASE_NUM];
	int label_end = allocLabel(),
		label_default = allocLabel();

	for (int i = 0; i < case_len; i++) {				//集体判断该跳转到何处
		astnode_case = astnode_caselist->getChild(i);
		labels[i] = allocLabel();
		Output::addi(reg_const, $zero, astnode_case->getValue());
		Output::beq(reg_expr, reg_const, labels[i]);
	}
	regManager->freeTmpReg(reg_expr);
	regManager->freeTmpReg(reg_const);

	Output::j(label_default);	//跳到default

	regManager->record();						//记录环境

	for (int i = 0; i < case_len; i++) {			//每一个case语句块
		astnode_case = astnode_caselist->getChild(i);
		Output::label(labels[i]);					//入口标签

		regManager->record();		//执行if前保存当前寄存器环境

		t_stmt(astnode_case->getChild(ASTNode_Case_Stmt));						//语句

		regManager->freeAllAndRecover();		//

		Output::j(label_end);						//跳出switch
	}

	//default语句块
	Output::label(label_default);
	t_stmt(astnode_default);

	regManager->freeAllAndRecover();						//恢复环境

	//switch结束
	Output::label(label_end);
}

//WHILE语句
void TargetCode::t_while(ASTNode* astnode_while) {
	ASTNode* cond = astnode_while->getChild(ASTNode_While_Cond);
	ASTNode* stmt = astnode_while->getChild(ASTNode_While_Stmt);

	Output::note("WHILE");

	int label1 = allocLabel();
	Output::label(label1);		//再循环一次，跳回的地方

	regManager->record();

	regManager->record();

	int label2 = t_cond(cond);	//检测循环条件

	//

	t_stmt(stmt);				//执行循环语句

	regManager->freeAllAndRecover();

	Output::j(label1);			//跳回上面

	Output::label(label2);		//循环条件失败，调出循环

	regManager->freeAllDirectly();
	regManager->recover();


}

//FOR语句
void TargetCode::t_for(ASTNode* astnode_for) {
	ASTNode* init = astnode_for->getChild(ASTNode_For_Init);
	ASTNode* cond = astnode_for->getChild(ASTNode_For_Cond);
	ASTNode* update = astnode_for->getChild(ASTNode_For_Update);
	ASTNode* stmt = astnode_for->getChild(ASTNode_For_Stmt);

	Output::note("FOR");

	t_assign(init);

	int label1 = allocLabel();

	Output::label(label1);

	regManager->record();

	regManager->record();

	int label2 = t_cond(cond);

	//regManager->record();

	t_stmt(stmt);

	t_assign(update);

	printf("在释放9之前");
	regManager->printReg(9);
	
	Output::note("freeAll之前");

	regManager->freeAllAndRecover();

	Output::j(label1);

	Output::label(label2);

	//regManager->freeAllAndRecover();
	regManager->freeAllDirectly();
	regManager->recover();
}

void TargetCode::t_ret(ASTNode* astnode_ret) {
	ASTNode* astnode_expres;
	int has_ret = astnode_ret->getValue();
	if (has_ret) {
		//printf("有返回值");
		astnode_expres = astnode_ret->getChild(ASTNode_Return_Expression);
		int type;
		int reg = t_factor(astnode_expres,type);
		Output::addi($v0, reg, 0);
		regManager->freeTmpReg(reg);
	}
	else {
		//printf("没有返回值");
	}

	regManager->freeAll();

	Output::jr($ra);
}