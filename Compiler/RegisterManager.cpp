#include "RegisterManager.h"
RegisterManager::RegisterManager() {
	for (int i = 0; i < 32; i++) {
		registers[i].setRegId(i);
	}
	return;
}

//申请临时寄存器
int RegisterManager::allocTmpReg() {
	//找空闲寄存器,和alloc_time最大的
	int mintime = 99999999,regid = -1;
	for (int i = $t0; i <= $t9; i++) {
		if (registers[i].isFree()) {
			regid = registers[i].getRegId();

			//配置寄存器
			registers[regid].setTemp();

			return regid;
		}
		if (registers[i].isVar() && registers[i].getAllocTime()<mintime) {
			mintime = registers[i].getAllocTime();
			regid = registers[i].getRegId();
		}
	}
	if (regid == -1) {
		printf("奇怪！没有可分配的寄存器了！！\n");
	}
	printf("借个变量寄存器%d来用！\n", regid);
	//配置寄存器
	freeReg(regid);
	registers[regid].setTemp();
	registers[regid].setAllocTime(nextTime());

	return regid;
}

//释放寄存器，写会内存
void RegisterManager::freeReg(int regid) {
	
	printf("释放寄存器%d\n",regid);
	if (registers[regid].isVar()) {
		//变量写回
		pair<int,int> addr = registers[regid].getReflect();
		int base = addr.first;
		int offset = addr.second;
		Output::sw(regid, offset, base);
		printf("sw!!\n");
	}

	registers[regid].setFree();
}

//释放临时寄存器
void RegisterManager::freeTmpReg(int regid) {
	if (registers[regid].isTemp()) {
		printf("释放临时寄存器%d\n", regid);
		registers[regid].setFree();
	}
}

void RegisterManager::freeRegDirectly(int regid) {
	registers[regid].setFree();
}

//将寄存器的值存入内存(或其对应的寄存器)
void RegisterManager::saveVar(int reg, char* name) {
	Sign sign;
	signTable->getSign(name, sign);

	for (int i = $t0; i <= $t9; i++) {
		if (registers[i].isVar() && registers[i].getVarId() == sign.getId()) {
			//写入其对应的寄存器
			printf("写入寄存器!!\n");
			Output::addi(registers[i].getRegId(), reg, 0);
			return;
		}
	}
	printf("写入内存!!\n");
	//写入内存
	if (registers[reg].isTemp()) {
		printf("把临时寄存器%d，变成变量寄存器\n",reg);
		bindRegAndVar(reg, name);
		return;
	}

	//不用loadVar是因为,loadVar还要load一下。
	int reg_var = allocTmpReg();
	bindRegAndVar(reg_var, name);
	Output::addi(reg_var, reg, 0);
	//Output::sw(reg, sign.getOffset(), sign.getBase());
}

//加载符号表
void RegisterManager::setSignTable(SignTable* table) {
	signTable = table;
}

//下一个时间
int RegisterManager::nextTime() {
	return time++;
}

void RegisterManager::bindRegAndVar(int reg, char* varname) {
	Sign sign;
	signTable->getSign(varname, sign);

	printf("bindRegAndVar:varname=%s,reg=%d,sign-id=%d,sign-offset=%d\n", varname, reg, sign.getId(), sign.getOffset());

	registers[reg].setVar();						//设置为变量类型寄存器
	registers[reg].setVarId(sign.getId());		//设置其绑定的变量id
	registers[reg].setReflect(make_pair(sign.getBase(), sign.getOffset()));//设置其映射到的内存地址
}

void RegisterManager::record() {
	for (int i = 0; i < 32; i++) {
		if (registers[i].isVar()) {
			recordRegisters[regRecordTop][i] = registers[i];
		}
		else {
			recordRegisters[regRecordTop][i].setFree();
		}
	}

	regRecordTop++;
}

void RegisterManager::freeAllAndRecover() {
	freeAll();

	recover();
}

void RegisterManager::recover() {		//回复上一次记录的寄存器
	for (int i = 0; i < 32; i++) {
		//if (registers[i].isVar()) {
		//	freeReg(i);
		//}

		if (recordRegisters[regRecordTop - 1][i].isVar()) {
			printf("recover:%d\n", i);
			//freeReg(i);
			registers[i] = recordRegisters[regRecordTop - 1][i];

			Output::lw(i, registers[i].getReflect().second, registers[i].getReflect().first);
		}
		else {
			
		}
	}
	regRecordTop--;
}

void RegisterManager::freeAll() {		//释放所有变量寄存器
	for (int i = 0; i < 32; i++) {
		if (registers[i].isVar()) {
			freeReg(i);		//写回寄存器
		}
	}
}

void RegisterManager::printReg(int regid) {
	printf("$%d,varid=%d,reflect_offset=%d\n", regid, registers[regid].getVarId()
		, registers[regid].getReflect().second);
}