#include "TargetCode.h"
//返回的寄存器里是数组该项的地址
int TargetCode::t_arr(ASTNode *astnode_arr,int &type) {
	char* name = astnode_arr->getValueStr();
	Sign sign;
	signTable->getSign(name, sign);
	int arr_addr = sign.getOffset();
	int dimen = sign.getDimen();
	int n = sign.getDimenN();
	int m = sign.getDimenM();

	if (dimen == 1) {
		int reg_index = t_factor(astnode_arr->getChild(ASTNode_Arr_Express), type);
		int reg_arr = regManager->allocTmpReg();

		Output::sll(reg_arr, reg_index, 2);			//倒腾地址

		Output::addi(reg_arr, reg_arr, arr_addr);

		Output::add(reg_arr, reg_arr, sign.getBase());

		regManager->freeTmpReg(reg_index);

		type = sign.getType();

		return reg_arr;
	}
	else if (dimen == 2) {
		int reg_index_n = t_factor(astnode_arr->getChild(ASTNode_Arr2_Express1), type);
		char tmp_mem[10];
		bindRegWithTmpMem(reg_index_n, tmp_mem);
		//printf("name:%s\n", tmp_mem);

		int reg_index_m = t_factor(astnode_arr->getChild(ASTNode_Arr2_Express2), type);

		int reg_arr = regManager->allocTmpReg();
		reg_index_n = regManager->loadVar(tmp_mem);

		Output::addi(reg_arr, $zero, m);				//倒腾地址

		Output::mult(reg_index_n, reg_arr);

		Output::mflo(reg_index_n);

		Output::add(reg_index_n, reg_index_n, reg_index_m);

		Output::sll(reg_index_n, reg_index_n, 2);

		Output::addi(reg_index_n, reg_index_n, arr_addr);	//加上数组头相对于sp寄存器的地址

		Output::add(reg_index_n, reg_index_n, sign.getBase());			//加上sp寄存器的地址

		regManager->freeTmpReg(reg_arr);
		regManager->freeTmpReg(reg_index_m);

		type = sign.getType();

		return reg_index_n;
	}
	return -1;
}