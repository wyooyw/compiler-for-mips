#include "RegisterManager.h"
//变量加载到寄存器中
int RegisterManager::loadVar(char* name) {
	Sign sign;
	signTable->getSign(name, sign);

	//先找现有寄存器有没有
	for (int i = $a1; i <= $t9; i++) {
		if (registers[i].isVar() && registers[i].getVarId() == sign.getId()) {
			//printf("%s在寄存器里有对应！", name);
			registers[i].setAllocTime(nextTime());
			return registers[i].getRegId();
		}
	}
	//printf("%s需要从内存里取出来，分配寄存器!", name);
	//当前寄存器里没有，申请新寄存器
	int regid = allocTmpReg();

	bindRegAndVar(regid, name);

	Output::lw(regid, sign.getOffset(), sign.getBase());
	return regid;
}
