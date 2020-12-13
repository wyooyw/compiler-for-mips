#include "TargetCode.h"
//返回值：结果所存的寄存器id
int TargetCode::t_factor(ASTNode* astnode_factor, int& type) {
	if (astnode_factor->getType() == ASTNodeType_Int) {
		//叶子 若出现比2^16大的数，应先用lui
		type = INTTK;

		int value = astnode_factor->getValue();

		int regid = regManager->allocTmpReg();

		if (value >= (1 << 16)) {
			Output::lui(regid, value >> 16);
			Output::addi(regid, regid, value & 0x0000ffff);
		}
		else {
			Output::addi(regid, $zero, value);
		}


		return regid;
	}
	else if (astnode_factor->getType() == ASTNodeType_Char) {
		//叶子
		type = CHARTK;
		int value = astnode_factor->getValue();

		int regid = regManager->allocTmpReg();

		Output::addi(regid, $zero, value);

		return regid;
	}
	else if (astnode_factor->getType() == ASTNodeType_Var) {
		//叶子
		char* name = astnode_factor->getValueStr();

		int reg_var = regManager->loadVar(name);

		type = signTable->getSignType(name);

		int reg_tmp = regManager->allocTmpReg();

		Output::addi(reg_tmp, reg_var, 0);

		return reg_tmp;
	}
	else if (astnode_factor->getType() == ASTNodeType_Call) {
		//函数调用
		char* name = astnode_factor->getValueStr();

		t_func_call(astnode_factor);

		type = signTable->getFuncType(name);

		int regid = regManager->allocTmpReg();

		Output::addi(regid, $v0, 0);

		return regid;
	}
	else if (astnode_factor->getType() == ASTNodeType_Arr) {	//一维数组
		char* name = astnode_factor->getValueStr();
		Sign sign;
		signTable->getSign(name, sign);
		int arr_addr = sign.getOffset();

		int reg = t_factor(astnode_factor->getChild(ASTNode_Arr_Express), type);
		//int reg_arr = regManager->allocTmpReg();

		Output::sll(reg, reg, 2);			//倒腾地址

		Output::addi(reg, reg, arr_addr);

		Output::add(reg, reg, sign.getBase());

		Output::lw(reg, 0, reg);			//把地址里的数拿出来

		regManager->freeTmpReg(reg);

		type = sign.getType();

		return reg;
	}
	else if (astnode_factor->getType() == ASTNodeType_Arr2) {	//二维数组
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

		Output::addi(reg_arr, $zero, m);				//倒腾地址

		Output::mult(reg_index_n, reg_arr);

		Output::mflo(reg_index_n);

		Output::add(reg_index_n, reg_index_n, reg_index_m);

		Output::sll(reg_index_n, reg_index_n, 2);

		Output::addi(reg_index_n, reg_index_n, arr_addr);	//加上数组头相对于sp寄存器的地址

		Output::add(reg_index_n, reg_index_n, sign.getBase());			//加上sp寄存器的地址

		Output::lw(reg_index_n, 0, reg_index_n);			//把地址里的数拿出来

		regManager->freeTmpReg(reg_arr);
		regManager->freeTmpReg(reg_index_m);

		type = sign.getType();

		regManager->unbindRegAndVarDirectly(reg_index_n);

		return reg_index_n;
	}
	else {
		//非叶子
		ASTNode* left_child = astnode_factor->getChild(ASTNode_Factor_Left);
		ASTNode* right_child = astnode_factor->getChild(ASTNode_Factor_Right);
		char sign = astnode_factor->getValue();

		if (right_child->getType() == ASTNodeType_Int && 
			(sign == '+' || sign=='-' || sign == '<' || sign == '>')) {
			int t;
			int left_regid = t_factor(left_child, t);
			int right_value = right_child->getValue();

			char sign = astnode_factor->getValue();
			switch (sign) {
			case '+':
				Output::addi(left_regid, left_regid, right_value);
				break;
			case '-':
				Output::subi(left_regid, left_regid, right_value);
				break;
			case '<':
				Output::sll(left_regid, left_regid, right_value);
				break;
			case '>':
				Output::srl(left_regid, left_regid, right_value);
				break;
			default:
				break;
			}

			return left_regid;
		}
		else {

			int t;
			int left_regid = t_factor(left_child, t);
			char left_tmp_mem[10];
			bindRegWithTmpMem(left_regid, left_tmp_mem);
			//printf("name:%s\n", left_tmp_mem);

			int right_regid = t_factor(right_child, t);
			//int rst_regid = regManager->allocTmpReg();
			type = INTTK;
			left_regid = regManager->loadVar(left_tmp_mem);

			switch (sign) {
			case '+':
				Output::add(left_regid, left_regid, right_regid);
				break;
			case '-':
				Output::sub(left_regid, left_regid, right_regid);
				break;
			case '*':
				Output::mult(left_regid, right_regid);
				Output::mflo(left_regid);
				break;
			case '/':
				Output::div(left_regid, right_regid);
				Output::mflo(left_regid);
				break;
			case '%':
				Output::div(left_regid, right_regid);
				Output::mfhi(left_regid);
				break;
			case '<':
				Output::sllv(left_regid, left_regid, right_regid);
				break;
			case '>':
				Output::srlv(left_regid, left_regid, right_regid);
				break;
			default:
				break;
			}

			regManager->freeTmpReg(right_regid);
			regManager->unbindRegAndVarDirectly(left_regid);

			return left_regid;
		}
	}
}