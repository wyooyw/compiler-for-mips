#include "RegisterManager.h"
RegisterManager::RegisterManager() {
	for (int i = 0; i < 32; i++) {
		registers[i].setRegId(i);
	}
	return;
}

//������ʱ�Ĵ���
int RegisterManager::allocTmpReg() {
	//�ҿ��мĴ���,��alloc_time����
	int mintime = 99999999,regid = -1;
	for (int i = $t0; i <= $t9; i++) {
		if (registers[i].isFree()) {
			regid = registers[i].getRegId();

			//���üĴ���
			registers[regid].setTemp();

			return regid;
		}
		if (registers[i].isVar() && registers[i].getAllocTime()<mintime) {
			mintime = registers[i].getAllocTime();
			regid = registers[i].getRegId();
		}
	}
	if (regid == -1) {
		printf("��֣�û�пɷ���ļĴ����ˣ���\n");
	}
	printf("��������Ĵ���%d���ã�\n", regid);
	//���üĴ���
	freeReg(regid);
	registers[regid].setTemp();
	registers[regid].setAllocTime(nextTime());

	return regid;
}

//�ͷżĴ�����д���ڴ�
void RegisterManager::freeReg(int regid) {
	
	printf("�ͷżĴ���%d\n",regid);
	if (registers[regid].isVar()) {
		//����д��
		pair<int,int> addr = registers[regid].getReflect();
		int base = addr.first;
		int offset = addr.second;
		Output::sw(regid, offset, base);
		printf("sw!!\n");
	}

	registers[regid].setFree();
}

//�ͷ���ʱ�Ĵ���
void RegisterManager::freeTmpReg(int regid) {
	if (registers[regid].isTemp()) {
		printf("�ͷ���ʱ�Ĵ���%d\n", regid);
		registers[regid].setFree();
	}
}

void RegisterManager::freeRegDirectly(int regid) {
	registers[regid].setFree();
}

//���Ĵ�����ֵ�����ڴ�(�����Ӧ�ļĴ���)
void RegisterManager::saveVar(int reg, char* name) {
	Sign sign;
	signTable->getSign(name, sign);

	for (int i = $t0; i <= $t9; i++) {
		if (registers[i].isVar() && registers[i].getVarId() == sign.getId()) {
			//д�����Ӧ�ļĴ���
			printf("д��Ĵ���!!\n");
			Output::addi(registers[i].getRegId(), reg, 0);
			return;
		}
	}
	printf("д���ڴ�!!\n");
	//д���ڴ�
	if (registers[reg].isTemp()) {
		printf("����ʱ�Ĵ���%d����ɱ����Ĵ���\n",reg);
		bindRegAndVar(reg, name);
		return;
	}

	//����loadVar����Ϊ,loadVar��Ҫloadһ�¡�
	int reg_var = allocTmpReg();
	bindRegAndVar(reg_var, name);
	Output::addi(reg_var, reg, 0);
	//Output::sw(reg, sign.getOffset(), sign.getBase());
}

//���ط��ű�
void RegisterManager::setSignTable(SignTable* table) {
	signTable = table;
}

//��һ��ʱ��
int RegisterManager::nextTime() {
	return time++;
}

void RegisterManager::bindRegAndVar(int reg, char* varname) {
	Sign sign;
	signTable->getSign(varname, sign);

	printf("bindRegAndVar:varname=%s,reg=%d,sign-id=%d,sign-offset=%d\n", varname, reg, sign.getId(), sign.getOffset());

	registers[reg].setVar();						//����Ϊ�������ͼĴ���
	registers[reg].setVarId(sign.getId());		//������󶨵ı���id
	registers[reg].setReflect(make_pair(sign.getBase(), sign.getOffset()));//������ӳ�䵽���ڴ��ַ
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

void RegisterManager::recover() {		//�ظ���һ�μ�¼�ļĴ���
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

void RegisterManager::freeAll() {		//�ͷ����б����Ĵ���
	for (int i = 0; i < 32; i++) {
		if (registers[i].isVar()) {
			freeReg(i);		//д�ؼĴ���
		}
	}
}

void RegisterManager::printReg(int regid) {
	printf("$%d,varid=%d,reflect_offset=%d\n", regid, registers[regid].getVarId()
		, registers[regid].getReflect().second);
}