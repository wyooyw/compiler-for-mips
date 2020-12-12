#include "TargetCode.h"
TargetCode::TargetCode(SignTable* para_signTable) {
	signTable = para_signTable;
	regManager = new RegisterManager();
	regManager->setSignTable(signTable);
}

int TargetCode::allocLabel() {
	return label++;
}

void TargetCode::bindRegWithTmpMem(int reg,char* name) {
	int id = tmp_mem_count;
	tmp_mem_count++;
	sp_offset+=WORD_SIZE;

	my_itoa(id, name);
	
	//printf("bindRegWithTmpMem:%s\n",name);

	signTable->addVar(0, name, 0, 2, 0);		//��ӽ����ű�

	regManager->bindRegAndVar(reg,name);	//
}

int TargetCode::bindRegWithTmpMem(int reg) {
	char name[MAX_WORD_LEN];

	int id = tmp_mem_count;
	tmp_mem_count++;
	sp_offset += WORD_SIZE;

	my_itoa(id, name);

	//printf("bindRegWithTmpMem:%s\n", name);

	signTable->addVar(0, name, 0, 2, 0);		//��ӽ����ű�

	regManager->bindRegAndVar(reg, name);	//

	return id;
}

void TargetCode::freeTmpMem(int reg) {
	tmp_mem_count--;
	sp_offset-=WORD_SIZE;
	regManager->freeRegDirectly(reg);
}

int TargetCode::allocSpMemory() {
	sp_offset += 4;
	return sp_offset;
}
int TargetCode::getOffsetOfVar(char *name) {
	return 0;
}
int TargetCode:: addStr(char* str) {
	int len = strlen(str);
	char *word = (char *)malloc(MAX_WORD_LEN * sizeof(char));
	int i = 0;
	for (str; *str; str++) {
		if ((*str) == '\\') {
			word[i++] = '\\';
		}
		word[i++] = *str;
	}
	word[i] = '\0';
	//printf("%s\n", word);
	strs.push_back(word);
	int str_offset = data_offset;
	data_offset = data_offset + len + 1;

	return str_offset;
}
void TargetCode::t_strings() {
	int len = strs.size();
	for (int i = 0; i < len; i++) {
		char* str = strs[i];
		Output::str(str);
	}
}
void TargetCode::t_begin(ASTNode* astnode_program) {
	Output::text();
	
	t_globalVarInit();

	Output::lui($t0,32752);
	Output::addi($sp, $t0, 0);
	
	t_program(astnode_program);
	
	Output::data();
	t_strings();
}

void TargetCode::t_program(ASTNode* astnode_program) {
	int len = astnode_program->getValue();
	ASTNode* astnode_func;
	char m[5] = "main";
	Output::jal(m);

	char end[10] = "wyoend";
	Output::jal(end);
	for (int i = 0; i < len; i++) {
		astnode_func = astnode_program->getChild(i);
		if (astnode_program->getChild(i)->getType() == ASTNodeType_Main) {
			printf("Main!!!!!\n");
			signTable->setCurrent(m);
			tmp_mem_count = 0;
			//��ʼ��main�еı���
			Output::label(m);

			t_mainVarInit();

			//main����
			t_main(astnode_func);

			signTable->setCurrent(0);
		}
		else {
			//printf("%s!!!!!\n", astnode_func->getValueStr());
			signTable->setCurrent(astnode_func->getValueStr());
			tmp_mem_count = 0;
			
			t_func(astnode_func);

			signTable->setCurrent(0);

		}
	}

	Output::label(end);
}
void TargetCode::t_main(ASTNode* astnode_main) {
	ASTNode* astnode_stmtlist = astnode_main->getChild(ASTNodeMain_Stmtlist);
	
	//���ط��ű�
	//�����ű��У�����Main�����ı����ͳ������ؽ����ڴ桱��
	
	

	//���������
	t_stmt_list(astnode_stmtlist);

}

//����ֵ���������ļĴ���id
int TargetCode::t_factor(ASTNode* astnode_factor,int &type) {
	if (astnode_factor->getType() == ASTNodeType_Int) {
		//Ҷ�� �����ֱ�2^16�������Ӧ����lui
		type = INTTK;

		int value = astnode_factor->getValue();

		int regid = regManager->allocTmpReg();

		if (value >= (1 << 16)) {
			Output::lui(regid, value >> 16);
			Output::addi(regid, regid, value&0x0000ffff);
		}
		else {
			Output::addi(regid, $zero, value);
		}
		

		return regid;
	}
	else if (astnode_factor->getType() == ASTNodeType_Char) {
		//Ҷ��
		type = CHARTK;
		int value = astnode_factor->getValue();

		int regid = regManager->allocTmpReg();

		Output::addi(regid, $zero, value);

		return regid;
	}
	else if (astnode_factor->getType() == ASTNodeType_Var) {
		//Ҷ��
		char* name = astnode_factor->getValueStr();

		int reg_var = regManager->loadVar(name); 

		type = signTable->getSignType(name);

		int reg_tmp = regManager->allocTmpReg();

		Output::addi(reg_tmp, reg_var, 0);

		return reg_tmp;
	}
	else if (astnode_factor->getType() == ASTNodeType_Call) {
		//��������
		char* name = astnode_factor->getValueStr();
		
		t_func_call(astnode_factor);

		type = signTable->getFuncType(name);

		int regid = regManager->allocTmpReg();

		Output::addi(regid, $v0, 0);

		return regid;
	}
	else if (astnode_factor->getType() == ASTNodeType_Arr) {	//һά����
		char* name = astnode_factor->getValueStr();
		Sign sign;
		signTable->getSign(name, sign);
		int arr_addr = sign.getOffset();
		
		int reg_index = t_factor(astnode_factor->getChild(ASTNode_Arr_Express),type);
		int reg_arr = regManager->allocTmpReg();

		Output::sll(reg_arr,reg_index, 2);			//���ڵ�ַ

		Output::addi(reg_arr, reg_arr, arr_addr);

		Output::add(reg_arr, reg_arr, sign.getBase());

		Output::lw(reg_arr, 0, reg_arr);			//�ѵ�ַ������ó���
		
		regManager->freeTmpReg(reg_index);

		type = sign.getType();
	
		return reg_arr;
	}
	else if (astnode_factor->getType() == ASTNodeType_Arr2) {	//��ά����
		char* name = astnode_factor->getValueStr();
		Sign sign;
		signTable->getSign(name, sign);
		int arr_addr = sign.getOffset();
		int n = sign.getDimenN();
		int m = sign.getDimenM();

		int reg_index_n = t_factor(astnode_factor->getChild(ASTNode_Arr2_Express1), type);
		char tmp_mem[10];
		bindRegWithTmpMem(reg_index_n, tmp_mem);
		//printf("name:%s\n", tmp_mem);
		
		int reg_index_m = t_factor(astnode_factor->getChild(ASTNode_Arr2_Express2), type);

		int reg_arr = regManager->allocTmpReg();
		reg_index_n = regManager->loadVar(tmp_mem);

		Output::addi(reg_arr, $zero, m);				//���ڵ�ַ

		Output::mult(reg_index_n, reg_arr);

		Output::mflo(reg_index_n);

		Output::add(reg_index_n, reg_index_n, reg_index_m);

		Output::sll(reg_index_n, reg_index_n, 2);			

		Output::addi(reg_index_n, reg_index_n, arr_addr);	//��������ͷ�����sp�Ĵ����ĵ�ַ

		Output::add(reg_index_n, reg_index_n, sign.getBase());			//����sp�Ĵ����ĵ�ַ

		Output::lw(reg_index_n, 0, reg_index_n);			//�ѵ�ַ������ó���

		regManager->freeTmpReg(reg_arr);
		regManager->freeTmpReg(reg_index_m);

		type = sign.getType();

		return reg_index_n;
	}
	else {
		//��Ҷ��
		ASTNode* left_child = astnode_factor->getChild(ASTNode_Factor_Left);
		ASTNode* right_child = astnode_factor->getChild(ASTNode_Factor_Right);

		int t;
		int left_regid = t_factor(left_child,t);
		char left_tmp_mem[10];
		bindRegWithTmpMem(left_regid, left_tmp_mem);
		//printf("name:%s\n", left_tmp_mem);

		int right_regid = t_factor(right_child,t);
		int rst_regid = regManager->allocTmpReg();
		type = INTTK;
		left_regid = regManager->loadVar(left_tmp_mem);

		char sign = astnode_factor->getValue();
		switch (sign) {
		case '+':
			Output::add(rst_regid, left_regid, right_regid);
			break;
		case '-':
			Output::sub(rst_regid, left_regid, right_regid);
			break;
		case '*':
			Output::mult(left_regid, right_regid);
			Output::mflo(rst_regid);
			break;
		case '/':
			Output::div(left_regid, right_regid);
			Output::mflo(rst_regid);
			break;
		case '%':
			Output::div(left_regid, right_regid);
			Output::mfhi(rst_regid);
			break;
		case '<':
			Output::sllv(rst_regid, left_regid, right_regid);
			break;
		case '>':
			Output::srlv(rst_regid, left_regid, right_regid);
			break;
		default:
			break;
		}

		regManager->freeTmpReg(left_regid);
		regManager->freeTmpReg(right_regid);

		return rst_regid;
	}
}
void TargetCode::t_assign(ASTNode* astnode_assign) {
	ASTNode* left = astnode_assign->getChild(ASTNode_Assign_Left);
	ASTNode* right = astnode_assign->getChild(ASTNode_Assign_Right);

	Output::note("#assign:");
	Output::note(left->getValueStr());

	int type;
	int reg = t_factor(right, type);
	char* varname = left->getValueStr();
	Sign sign;
	signTable->getSign(varname,sign);
	int dimen = sign.getDimen();
	int dimen_n = sign.getDimenN();
	int dimen_m = sign.getDimenM();

	if (dimen == 0) {
		regManager->saveVar(reg, varname);//�浽�ڴ���Ӧ�ļĴ�����

		regManager->freeTmpReg(reg);		//��reg����ʱ�Ĵ��������ͷš�

		return;
	}
	else{
		int id = bindRegWithTmpMem(reg);

		int reg_addr = t_arr(left, type);

		char str[20];
		reg = regManager->loadVar(my_itoa(id,str));

		Output::sw(reg, 0, reg_addr);

		regManager->freeTmpReg(reg);
		regManager->freeTmpReg(reg_addr);

		return;
	}
}
void TargetCode::t_printstring(ASTNode* astnode_print) {

	//printf("\n\nPRINT STRING!!!!!\n\n");

	ASTNode* astnode_str = astnode_print->getChild(ASTNode_Print_String);
	char* str = astnode_str->getValueStr();//�ַ�������

	//�ַ�������
	int str_offset = addStr(str);


	int reg = regManager->allocTmpReg();

	Output::lui(reg,0x1001);
	Output::ori($a0,reg, str_offset);
	//Output::la($a0, str_offset);
	Output::addi($v0, $zero, 4);
	Output::syscall();

	//����
	Output::addi($a0,$zero, '\n');
	Output::addi($v0, $zero, 11);
	Output::syscall();
}
void TargetCode::t_printexpression(ASTNode* astnode_print) {
	//printf("\n\nPRINT VAR!!!!!\n\n");

	ASTNode* astnode_factor = astnode_print->getChild(ASTNode_Print_Expression);
	int type;
	int reg = t_factor(astnode_factor,type);	//��type
	type = astnode_print->getValue();			//��type
	bool isChar = (type==CHARTK);

	//����/�ַ�����
	
	Output::addi($a0, reg,0);
	Output::addi($v0, $zero, isChar?11:1);
	Output::syscall();

	//����
	Output::addi($a0, $zero, '\n');
	Output::addi($v0, $zero, 11);
	Output::syscall();
}
void TargetCode::t_print(ASTNode* astnode_print) {
	//�ַ�������
	ASTNode* astnode_str = astnode_print->getChild(ASTNode_Print_String);
	char* str = astnode_str->getValueStr();//�ַ�������

	//�ַ�������
	int str_offset = addStr(str);

	int reg = regManager->allocTmpReg();
	Output::lui(reg, 0x1001);
	Output::ori($a0, reg, str_offset);
	//Output::la($a0, str_offset);
	Output::addi($v0, $zero, 4);
	Output::syscall();

	regManager->freeTmpReg(reg);		//�ͷżĴ���

	ASTNode* astnode_factor = astnode_print->getChild(ASTNode_Print_Expression);
	int type;
	int reg2 = t_factor(astnode_factor,type);	//��type
	type = astnode_print->getValue();			//��type
	bool isChar = (type==CHARTK);

	//���ʽ����
	//����/�ַ�����
	Output::addi($a0, reg2, 0);
	Output::addi($v0, $zero, isChar ? 11 : 1);
	Output::syscall();

	//����
	Output::addi($a0, $zero, '\n');
	Output::addi($v0, $zero, 11);
	Output::syscall();

	regManager->freeTmpReg(reg2);			//�ͷżĴ���
}
void TargetCode::t_scanf(ASTNode* astnode_scanf) {
	ASTNode* astnode_var = astnode_scanf->getChild(ASTNode_Scanf_Var);
	char* name = astnode_var->getValueStr();
	Sign var;
	if (signTable->getSign(name, var)) {
		if (var.getType() == CHARTK) {
			Output::addi($v0, $zero, 12);//�����ַ�
		}
		else {
			Output::addi($v0, $zero, 5);//��������
		}
		Output::syscall();

		regManager->saveVar($v0, name);
	}
};

//����label id����Ĵ������ҼĴ���������
int TargetCode::t_cond(ASTNode* astnode_cond) {
	ASTNode* left = astnode_cond->getChild(ASTNode_Condition_Left);
	ASTNode* right = astnode_cond->getChild(ASTNode_Condition_Right);
	int sign = astnode_cond->getValue();
	int type;
	int reg1 = t_factor(left,type);
	int id = bindRegWithTmpMem(reg1);
	char str[20];
	int reg2 = t_factor(right, type);
	reg1 = regManager->loadVar(my_itoa(id,str));
	int label = allocLabel();
	if (sign == EQL) {
		//�ж���ȣ���������ȣ���Ҫ��ת
		Output::bne(reg1, reg2,label);
		regManager->freeTmpReg(reg1);		//���뼰ʱ�ͷ���ʱ�Ĵ���
		regManager->freeTmpReg(reg2);		//��Ȼ��ʱ�Ĵ�����ֱ���������Ⱳ��Ҳ�ò���
	}
	else if (sign == NEQ) {
		//�жϲ��ȣ��������ȣ���Ҫ��ת
		Output::beq(reg1, reg2, label);
		regManager->freeTmpReg(reg1);
		regManager->freeTmpReg(reg2);
	}
	else {
		//������ʱ�Ĵ���
		int reg = regManager->allocTmpReg();

		Output::sub(reg,reg1,reg2);
		regManager->freeTmpReg(reg1);
		regManager->freeTmpReg(reg2);

		if (sign == LSS) {
			//�ж�С�ڣ���������ڵ��ڣ���Ҫ��ת
			Output::bgez(reg, label);
			regManager->freeTmpReg(reg);
		}
		else if (sign == LEQ) {
			//�ж�С�ڵ��ڣ���������ڣ���Ҫ��ת
			Output::bgtz(reg, label);
			regManager->freeTmpReg(reg);
		}
		else if (sign == GRE) {
			//�жϴ��ڣ������С�ڵ��ڣ���Ҫ��ת
			Output::blez(reg, label);
			regManager->freeTmpReg(reg);
		}
		else if (sign == GEQ) {
			//�жϴ��ڵ��ڣ������С�ڣ���Ҫ��ת
			Output::bltz(reg, label);
			regManager->freeTmpReg(reg);
		}
		else {
			//printf("�����˴���ıȽϷ��ţ�����%c,����ascii����\n", sign, sign);
		}

	}
	

	return label;
}