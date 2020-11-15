#include "TargetCode.h"
TargetCode::TargetCode(SignTable* para_signTable) {
	signTable = para_signTable;
}
int TargetCode::allocSpMemory() {
	sp_offset += 4;
	return sp_offset;
}
int TargetCode::getOffsetOfVar(char *name) {
	return 0;
}
void TargetCode::t_signs_global() {
	gp_offset = signTable->getGpOffset();
}
void TargetCode::t_signs_main() {
	sp_offset = signTable->getSpOffset();
}
int TargetCode::loadVar(int regis, char* name) {
	Sign var;
	if (signTable->getSign(name,var)) {
		if (var.getLevel() == 1) {
			//全局变量
			Output::lw(regis, var.getOffset(), $gp);
		}
		else {
			//局部变量
			Output::lw(regis, var.getOffset(), $sp);
		}
	}
	if (var.getType() == CHARTK) {
		return CHARTK;
	}
	else {
		return INTTK;
	}
}
void TargetCode::saveVar(int regis, char* name) {
	Sign var;
	if (signTable->getSign(name, var)) {
		if (var.getLevel() == 1) {
			//全局变量
			Output::sw(regis, var.getOffset(), $gp);
		}
		else {
			//局部变量
			Output::sw(regis, var.getOffset(), $sp);
		}
	}
	else {
		printf("没有找到变量！！%s\n",name);
	}
}
int TargetCode:: addStr(char* str) {
	int len = strlen(str);
	strs.push_back(str);
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
	
	t_program(astnode_program);
	
	Output::data();
	t_strings();
}
void TargetCode::t_globalVarInit() {
	gp_offset = signTable->getGpOffset();

	Sign* begin;
	int len;
	signTable->getGlobalSigns(begin,len);
	for (int i = 0; i < len; i++, begin++) {
		int value = begin->getInitValue();
		int offset = begin->getOffset();
		Output::addi($t0, $zero, value);
		Output::sw($t0, offset, $gp);
	}
}
void TargetCode::t_mainVarInit() {
	sp_offset = signTable->getSpOffset();

	Sign* begin;
	int len;
	signTable->getMainSigns(begin, len);
	for (int i = 0; i < len; i++, begin++) {
		int value = begin->getInitValue();
		int offset = begin->getOffset();
		Output::addi($t0, $zero, value);
		Output::sw($t0, offset, $sp);
	}
}
void TargetCode::t_program(ASTNode* astnode_program) {
	int len = astnode_program->getValue();
	ASTNode* astnode_func;
	for (int i = 0; i < len; i++) {
		astnode_func = astnode_program->getChild(i);
		if (astnode_program->getChild(i)->getType() == ASTNodeType_Main) {
			//初始化main中的变量
			t_mainVarInit();

			//main函数
			t_main(astnode_func);

		}
		else {

			//其他函数
			printf("Not exists in this experiment!");

		}
	}
}
void TargetCode::t_main(ASTNode* astnode_main) {
	ASTNode* astnode_stmtlist = astnode_main->getChild(ASTNodeMain_Stmtlist);
	
	//加载符号表
	//将符号表中，属于Main函数的变量和常量加载进“内存”。
	
	

	//解析语句列
	t_stmt_list(astnode_stmtlist);

}
void TargetCode::t_stmt_list(ASTNode* astnode_stmtlist) {
	int len = astnode_stmtlist->getValue();
	ASTNode* astnode_stmt;
	for (int i = 0; i < len; i++) {
		astnode_stmt = astnode_stmtlist->getChild(i);
		switch (astnode_stmt->getType()) {
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
		default:
			break;
		}

	}
}
int TargetCode::t_factor(ASTNode* astnode_factor,int &type) {
	Output::note("");
	if (astnode_factor->getType() == ASTNodeType_Int) {
		//叶子 若出现比2^16大的数，应先用lui
		type = INTTK;
		int value = astnode_factor->getValue();
		int rst_offset = allocSpMemory();
		Output::addi($t0, $zero, value);
		Output::sw($t0, rst_offset, $sp);
		return rst_offset;
	}
	else if (astnode_factor->getType() == ASTNodeType_Char) {
		//叶子
		type = CHARTK;
		int value = astnode_factor->getValue();
		int rst_offset = allocSpMemory();
		Output::addi($t0, $zero, value);
		Output::sw($t0, rst_offset, $sp);
		return rst_offset;
	}
	else if (astnode_factor->getType() == ASTNodeType_Var) {
		//叶子
		char* name = astnode_factor->getValueStr();
		//int var_offset = getOffsetOfVar(name);
		int rst_offset = allocSpMemory();
		//Output::lw($t0, var_offset,$sp);
		type = loadVar($t0, name);
		Output::sw($t0, rst_offset, $sp);
		return rst_offset;
	}
	else {
		//非叶子
		ASTNode* left_child = astnode_factor->getChild(ASTNode_Factor_Left);
		ASTNode* right_child = astnode_factor->getChild(ASTNode_Factor_Right);

		int t;
		int left_offset = t_factor(left_child,t);
		int right_offset = t_factor(right_child,t);
		type = INTTK;

		Output::lw($t0, left_offset, $sp);	//lw $t0,left_offset($sp)
		Output::lw($t1, right_offset, $sp);	//lw $t1,right_offset($sp)

		char sign = astnode_factor->getValue();
		switch (sign) {
		case '+':
			Output::add($t2, $t0, $t1);
			break;
		case '-':
			Output::sub($t2, $t0, $t1);
			break;
		case '*':
			Output::mult($t0, $t1);
			Output::mflo($t2);
			break;
		case '/':
			Output::div($t0, $t1);
			Output::mflo($t2);
			break;
		default:
			break;
		}

		int rst_offset = allocSpMemory();
		Output::sw($t2, rst_offset,$sp);	//sw $t2,rst_offset($sp)
		return rst_offset;
	}
}
void TargetCode::t_assign(ASTNode* astnode_assign) {
	ASTNode* left = astnode_assign->getChild(ASTNode_Assign_Left);
	ASTNode* right = astnode_assign->getChild(ASTNode_Assign_Right);

	int type;
	int rst_offset = t_factor(right, type);
	char* varname = left->getValueStr();
	
	Output::note("赋值语句");
	Output::lw($t0, rst_offset,$sp);
	saveVar($t0, varname);
}
void TargetCode::t_printstring(ASTNode* astnode_print) {

	ASTNode* astnode_str = astnode_print->getChild(ASTNode_Print_String);
	char* str = astnode_str->getValueStr();//字符串内容

	//字符串内容
	int str_offset = addStr(str);

	Output::note("打印字符串");

	Output::lui($t0,0x1001);
	Output::ori($a0,$t0, str_offset);
	//Output::la($a0, str_offset);
	Output::addi($v0, $zero, 4);
	Output::syscall();

	//换行
	Output::addi($a0,$zero, '\n');
	Output::addi($v0, $zero, 11);
	Output::syscall();
}
void TargetCode::t_printexpression(ASTNode* astnode_print) {
	ASTNode* astnode_factor = astnode_print->getChild(ASTNode_Print_Expression);
	int type;
	int rst_offset = t_factor(astnode_factor,type);
	bool isChar = (type==CHARTK);

	Output::note("打印表达式");
	//数字/字符内容
	
	Output::lw($a0, rst_offset,$sp);
	Output::addi($v0, $zero, isChar?11:1);
	Output::syscall();

	//换行
	Output::addi($a0, $zero, '\n');
	Output::addi($v0, $zero, 11);
	Output::syscall();
}
void TargetCode::t_print(ASTNode* astnode_print) {
	//字符串部分
	ASTNode* astnode_str = astnode_print->getChild(ASTNode_Print_String);
	char* str = astnode_str->getValueStr();//字符串内容

	Output::note("打印-打印字符串部分");
	//字符串内容
	int str_offset = addStr(str);
	Output::lui($t0, 0x1001);
	Output::ori($a0, $t0, str_offset);
	//Output::la($a0, str_offset);
	Output::addi($v0, $zero, 4);
	Output::syscall();

	ASTNode* astnode_factor = astnode_print->getChild(ASTNode_Print_Expression);
	int type;
	int rst_offset = t_factor(astnode_factor,type);
	bool isChar = type==CHARTK;

	//表达式部分
	Output::note("打印-打印表达式部分");
	//数字/字符内容
	Output::lw($a0, rst_offset, $sp);
	Output::addi($v0, $zero, isChar ? 11 : 1);
	Output::syscall();

	//换行
	Output::addi($a0, $zero, '\n');
	Output::addi($v0, $zero, 11);
	Output::syscall();
}
void TargetCode::t_scanf(ASTNode* astnode_scanf) {
	ASTNode* astnode_var = astnode_scanf->getChild(ASTNode_Scanf_Var);
	char* name = astnode_var->getValueStr();
	Sign var;
	if (signTable->getSign(name, var)) {
		if (var.getType() == CHARTK) {
			Output::addi($v0, $zero, 12);//输入字符
		}
		else {
			Output::addi($v0, $zero, 5);//输入数字
		}
		Output::syscall();
		saveVar($v0, name);
	}
};