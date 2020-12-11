#include "TargetCode.h"

//���
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

//IF���
void TargetCode::t_if(ASTNode* astnode_if) {
	ASTNode* cond = astnode_if->getChild(ASTNode_If_Cond);
	ASTNode* stmt = astnode_if->getChild(ASTNode_If_Stmt);

	int label = t_cond(cond);

	regManager->record();		//ִ��ifǰ���浱ǰ�Ĵ�������

	t_stmt(stmt);
	//Output::note("before recover");
	regManager->freeAllAndRecover();		//�ظ�֮ǰ�Ĵ�����������ֹ�������ϲ�����
	//Output::note("after recover");
	Output::label(label);
}

//IF_ELSE���
void TargetCode::t_ifelse(ASTNode* astnode_ifelse) {
	ASTNode* cond = astnode_ifelse->getChild(ASTNode_Ifelse_Cond);
	ASTNode* stmt = astnode_ifelse->getChild(ASTNode_Ifelse_Stmt);
	ASTNode* stmt_else = astnode_ifelse->getChild(ASTNode_Ifelse_ElseStmt);

	int label1 = t_cond(cond);

	regManager->record();		//ִ��ifǰ���浱ǰ�Ĵ�������

	t_stmt(stmt);

	regManager->freeAllAndRecover();		//�ظ�֮ǰ�Ĵ�����������ֹ�������ϲ�����

	int label2 = allocLabel();
	Output::j(label2);

	Output::label(label1);

	regManager->record();		//ִ��ifǰ���浱ǰ�Ĵ�������

	t_stmt(stmt_else);

	regManager->freeAllAndRecover();		//�ظ�֮ǰ�Ĵ�����������ֹ�������ϲ�����

	Output::label(label2);
}

//SWITCH���
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

	for (int i = 0; i < case_len; i++) {				//�����жϸ���ת���δ�
		astnode_case = astnode_caselist->getChild(i);
		labels[i] = allocLabel();
		Output::addi(reg_const, $zero, astnode_case->getValue());
		Output::beq(reg_expr, reg_const, labels[i]);
	}
	regManager->freeTmpReg(reg_expr);
	regManager->freeTmpReg(reg_const);

	Output::j(label_default);	//����default

	regManager->record();						//��¼����

	for (int i = 0; i < case_len; i++) {			//ÿһ��case����
		astnode_case = astnode_caselist->getChild(i);
		Output::label(labels[i]);					//��ڱ�ǩ

		regManager->record();		//ִ��ifǰ���浱ǰ�Ĵ�������

		t_stmt(astnode_case->getChild(ASTNode_Case_Stmt));						//���

		regManager->freeAllAndRecover();		//

		Output::j(label_end);						//����switch
	}

	//default����
	Output::label(label_default);
	t_stmt(astnode_default);

	regManager->freeAllAndRecover();						//�ָ�����

	//switch����
	Output::label(label_end);
}

//WHILE���
void TargetCode::t_while(ASTNode* astnode_while) {
	ASTNode* cond = astnode_while->getChild(ASTNode_While_Cond);
	ASTNode* stmt = astnode_while->getChild(ASTNode_While_Stmt);

	Output::note("WHILE");

	int label1 = allocLabel();
	Output::label(label1);		//��ѭ��һ�Σ����صĵط�

	regManager->record();

	regManager->record();

	int label2 = t_cond(cond);	//���ѭ������

	//

	t_stmt(stmt);				//ִ��ѭ�����

	regManager->freeAllAndRecover();

	Output::j(label1);			//��������

	Output::label(label2);		//ѭ������ʧ�ܣ�����ѭ��

	regManager->freeAllDirectly();
	regManager->recover();


}

//FOR���
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

	printf("���ͷ�9֮ǰ");
	regManager->printReg(9);
	
	Output::note("freeAll֮ǰ");

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
		//printf("�з���ֵ");
		astnode_expres = astnode_ret->getChild(ASTNode_Return_Expression);
		int type;
		int reg = t_factor(astnode_expres,type);
		Output::addi($v0, reg, 0);
		regManager->freeTmpReg(reg);
	}
	else {
		//printf("û�з���ֵ");
	}

	regManager->freeAll();

	Output::jr($ra);
}