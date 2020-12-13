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
	for (int i = $a1; i <= $t9; i++) {
		if (registers[i].isFree()) {
			regid = registers[i].getRegId();

			//���üĴ���
			registers[regid].setTemp();

			return regid;
		}
		if (registers[i].isVar() && registers[i].getAllocTime()<mintime) {
			mintime = registers[i].getAllocTime();
			regid = registers[i].getRegId();
			if (regid == -1) {
				/*while (1) {
					printf("helloworld\n");
				}*/
			}
		}
	}
	if (regid == -1) {
		printf("��֣�û�пɷ���ļĴ����ˣ���\n");
	}
	//printf("��������Ĵ���%d���ã�\n", regid);
	//���üĴ���
	freeReg(regid);
	registers[regid].setTemp();
	registers[regid].setAllocTime(nextTime());
	registers[regid].setChange(false);

	return regid;
}

//�ͷżĴ�����д���ڴ�
void RegisterManager::freeReg(int regid) {
	
	//printf("�ͷżĴ���%d\n",regid);
	if (registers[regid].isVar() && registers[regid].isChange()) {//
		//����д��
		pair<int,int> addr = registers[regid].getReflect();
		int base = addr.first;
		int offset = addr.second;
		Output::sw(regid, offset, base);
		//printf("sw!!\n");
	}

	registers[regid].setFree();
	registers[regid].setChange(false);
}

//�ͷ���ʱ�Ĵ���
void RegisterManager::freeTmpReg(int regid) {
	if (registers[regid].isTemp()) {
		//printf("�ͷ���ʱ�Ĵ���%d\n", regid);
		registers[regid].setFree();
		registers[regid].setChange(false);
	}
}

void RegisterManager::freeRegDirectly(int regid) {
	registers[regid].setFree();
	registers[regid].setChange(false);
}

//���Ĵ�����ֵ�����ڴ�(�����Ӧ�ļĴ���)
void RegisterManager::saveVar(int reg, char* name) {
	Sign sign;
	signTable->getSign(name, sign);

	for (int i = $a1; i <= $t9; i++) {
		if (registers[i].isVar() && registers[i].getVarId() == sign.getId()) {
			//д�����Ӧ�ļĴ���
			//printf("д��Ĵ���!!\n");
			Output::addi(registers[i].getRegId(), reg, 0);
			registers[i].setChange(true);
			return;
		}
	}
	//printf("д���ڴ�!!\n");
	//д���ڴ�
	if (registers[reg].isTemp()) {
		//printf("����ʱ�Ĵ���%d����ɱ����Ĵ���\n",reg);
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

	//printf("bindRegAndVar:varname=%s,reg=%d,sign-id=%d,sign-offset=%d\n", varname, reg, sign.getId(), sign.getOffset());

	registers[reg].setVar();						//����Ϊ�������ͼĴ���
	registers[reg].setVarId(sign.getId());		//������󶨵ı���id
	registers[reg].setReflect(make_pair(sign.getBase(), sign.getOffset()));//������ӳ�䵽���ڴ��ַ
	registers[reg].setAllocTime(nextTime());
	registers[reg].setChange(true);
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
	if (regRecordTop >= MAX_REG_RECORD_TIMES-1) {
		
	}
}

void RegisterManager::freeAllAndRecover() {
	freeAll();

	recover();
}

void RegisterManager::recover() {		//�ظ���һ�μ�¼�ļĴ���
	//printf("recover-top:%d\n", regRecordTop);
	for (int i = 0; i < 32; i++) {
		//if (registers[i].isVar()) {
		//	freeReg(i);
		//}

		if (recordRegisters[regRecordTop - 1][i].isVar()) {
			//printf("recover:%d\n", i);
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

void RegisterManager::freeAllDirectly() {		//�ͷ����б����Ĵ���
	for (int i = 0; i < 32; i++) {
		if (registers[i].isVar()) {
			freeRegDirectly(i);		//д�ؼĴ���
		}
	}
}

void RegisterManager::printReg(int regid) {
	//printf("$%d,varid=%d,reflect_offset=%d\n", regid, registers[regid].getVarId()
	//	, registers[regid].getReflect().second);
}

void RegisterManager::change(int reg) {
	registers[reg].setChange(true);
}

void RegisterManager::unbindRegAndVarDirectly(int regid) {
	registers[regid].setTemp();
}