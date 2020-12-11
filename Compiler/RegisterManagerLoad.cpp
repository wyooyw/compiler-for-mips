#include "RegisterManager.h"
//�������ص��Ĵ�����
int RegisterManager::loadVar(char* name) {
	Sign sign;
	signTable->getSign(name, sign);

	//�������мĴ�����û��
	for (int i = $a1; i <= $t9; i++) {
		if (registers[i].isVar() && registers[i].getVarId() == sign.getId()) {
			//printf("%s�ڼĴ������ж�Ӧ��", name);
			registers[i].setAllocTime(nextTime());
			return registers[i].getRegId();
		}
	}
	//printf("%s��Ҫ���ڴ���ȡ����������Ĵ���!", name);
	//��ǰ�Ĵ�����û�У������¼Ĵ���
	int regid = allocTmpReg();

	bindRegAndVar(regid, name);

	Output::lw(regid, sign.getOffset(), sign.getBase());
	return regid;
}
