#include "TargetCode.h"
void TargetCode::t_globalVarInit() {
	gp_offset = signTable->getGpOffset();

	Sign* sign;
	int len;
	list<Sign*> *signs = signTable->getGlobalSigns();
	list<Sign*>::iterator iter;
	for (iter = signs->begin(); iter != signs->end(); iter++) {
		sign = *iter;
		if (!sign->hasInitValue()) continue;		//如果没有初值，直接跳过
		int dimen = sign->getDimen();

		if (dimen == 0) {							//初始化普通变量
			//printf("\n%s ,%d", sign->getName(), sign->getOffset());
			int value = sign->getInitValue();
			int offset = sign->getOffset();
			Output::addi($t0, $zero, value);
			Output::sw($t0, offset, $gp);
		}
		else if (dimen == 1) {						//初始化一维数组
			for (int i = 0; i < sign->getDimenN(); i++) {

				int value = sign->getInitValue(i);
				int offset = sign->getOffset() + i * WORD_SIZE;
				Output::addi($t0, $zero, value);
				Output::sw($t0, offset, $gp);
				//printf("\n%s[%d] ,%d", sign->getName(), i, offset);
			}
		}
		else if (dimen == 2) {						//初始化二维数组
			for (int i = 0; i < sign->getDimenN(); i++) {
				for (int j = 0; j < sign->getDimenM(); j++) {
					int value = sign->getInitValue(i, j);
					int offset = sign->getOffset() + (i * sign->getDimenM() + j) * WORD_SIZE;
					Output::addi($t0, $zero, value);
					Output::sw($t0, offset, $gp);
					//printf("\n%s[%d][%d] ,%d", sign->getName(), i, j, offset);
				}
			}
		}
		else {
			//printf("未知维度？！");
		}
	}
}
void TargetCode::t_mainVarInit() {
	sp_offset = signTable->getSpOffset();
	//printf("main's sp offset is %d\n", sp_offset);
	Sign* sign;
	int len;
	list<Sign*> *signs = signTable->getMainSigns();
	list<Sign*>::iterator iter;
	for (iter = signs->begin(); iter != signs->end(); iter++) {
		sign = *iter;
		if (!sign->hasInitValue()) continue;		//如果没有初值，直接跳过

		int dimen = sign->getDimen();
		if (dimen == 0) {							//初始化普通变量
			//printf("\n%s ,%d", sign->getName(), sign->getOffset());
			int value = sign->getInitValue();
			int offset = sign->getOffset();
			Output::addi($t0, $zero, value);
			Output::sw($t0, offset, $sp);
		}
		else if (dimen == 1) {						//初始化一维数组
			for (int i = 0; i < sign->getDimenN(); i++) {

				int value = sign->getInitValue(i);
				int offset = sign->getOffset() + i * WORD_SIZE;
				Output::addi($t0, $zero, value);
				Output::sw($t0, offset, $sp);
				//printf("\n%s[%d] ,%d", sign->getName(), i, offset);
			}
		}
		else if (dimen == 2) {						//初始化二维数组
			for (int i = 0; i < sign->getDimenN(); i++) {
				for (int j = 0; j < sign->getDimenM(); j++) {
					int value = sign->getInitValue(i, j);
					int offset = sign->getOffset() + (i * sign->getDimenM() + j) * WORD_SIZE;
					Output::addi($t0, $zero, value);
					Output::sw($t0, offset, $sp);
					//printf("\n%s[%d][%d] ,%d", sign->getName(), i, j, offset);
				}
			}
		}
		else {
			//printf("未知维度？！");
		}

	}
}