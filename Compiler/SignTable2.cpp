#include "SignTable.h"
SignTable::SignTable() {
	
}

//����һ������
void SignTable::push(Sign sign) {

	//���½������ڱ���ʵ��
	if (sign.getLevel() == 1 && (sign.getCategory() == C_VAR || sign.getCategory() == C_CONST)) {
		//ȫ�ֱ���
		//Ӧ���ȸ�offsex������setOffset?
		sign.setBase($gp);
		sign.setOffset(gp_offset);
		globalVarCount++;
		if (sign.getDimen() == 1) {
			gp_offset += WORD_SIZE * sign.getDimenN();
		}
		else if (sign.getDimen() == 2) {
			gp_offset += WORD_SIZE * sign.getDimenN()*sign.getDimenM();
		}
		else {
			gp_offset += WORD_SIZE;
		}
		
	}
	else if (sign.getLevel() == 2 && (sign.getCategory() == C_VAR || sign.getCategory() == C_CONST || sign.getCategory() == C_PARA)) {
		//�ֲ�����
		sign.setBase($sp);
		mainVarCount++;
	}

	if (sign.getCategory() == C_FUNCTION) {
		funname2id[sign.getName()] = subTablesTop++;
	}

	sign.setId(signId++);

	subTables[current].addSign(sign);

	//show();
}

bool SignTable::havaSignInSameLevel(char* name, int level) {
	return subTables[current].hasName(name);
}

//1:�������Ȳ�һ��	2���������Ͳ�һ��	3����һ��	0��û��ͬ������

void SignTable::checkPara(char* name, int paralen, int paratype[], int row) {
	if (subTables[0].hasName(name)) {
		Sign* sign = subTables[0].getSign(name);
		if (paralen != sign->getF_para_len()) {
			Error::paraLenError(row);
		}
		for (int j = 0; j < paralen; j++) {
			if (paratype[j] != sign->getF_para_type(j)) {
				Error::paraTypeError(row);
			}
		}
		return;
	}
	else {
		Error::noDefError(row);
	}
}

bool SignTable::getSign(char* name, Sign& sign) {
	if (subTables[current].hasName(name)) {//�ֲ�����
		sign = *subTables[current].getSign(name);
		return true;
	}
	else if (subTables[0].hasName(name)) {//ȫ�ֱ���
		sign = *subTables[0].getSign(name);
		return true;
	}
	else {
		return false;
	}
}

int SignTable::getSignType(char* name) {
	Sign* sign;
	if (subTables[current].hasName(name)) {//�ֲ�����
		sign = subTables[current].getSign(name);
		return sign->getType();
	}
	else if (subTables[0].hasName(name)) {//ȫ�ֱ���
		sign = subTables[0].getSign(name);
		return sign->getType();
	}
	else {
		return -1;
	}
}

int SignTable::getFuncType(char* name) {
	Sign* sign;
	if (subTables[0].hasName(name)) {//ȫ�ֱ���
		sign = subTables[0].getSign(name);
		return sign->getType();
	}
	else {
		return -1;
	}
}

int SignTable::getSignCategory(char* name) {
	Sign* sign;
	if (subTables[current].hasName(name)) {//�ֲ�����
		sign = subTables[current].getSign(name);
		return sign->getCategory();
	}
	else if (subTables[0].hasName(name)) {//ȫ�ֱ���
		sign = subTables[0].getSign(name);
		return sign->getCategory();
	}
	else {
		return -1;
	}
}

int SignTable::getSignReturn(char* name) {
	Sign* sign;
	if (subTables[current].hasName(name)) {//�ֲ�����
		sign = subTables[current].getSign(name);
		return sign->getF_return();
	}
	else if (subTables[0].hasName(name)) {//ȫ�ֱ���
		sign = subTables[0].getSign(name);
		return sign->getF_return();
	}
	else {
		return -1;
	}
}

bool SignTable::havaSign(char* name) {
	return subTables[current].hasName(name) || subTables[0].hasName(name);
}

void SignTable::checkReturn(int retType, int row) {
	if (subTables[0].hasName(currentName)) {
		Sign* sign = subTables[0].getSign(currentName);
		sign->setF_return(retType);
		int type = sign->getType();

		if (type == VOIDTK && retType != VOIDTK) {
			Error::haveReturnError(row);
		}
		else if (type != retType) {
			Error::returnError(row);
		}
	}
}

void SignTable::addVar(int type, char* name, int dimen, int level, int init) {
	Sign sign;
	sign.setCategory(C_VAR);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setLevel(level);
	sign.setType(type);
	sign.setInitValue(init);
	push(sign);
}

void SignTable::addConst(int type, char* name, int dimen, int level, int init) {
	Sign sign;
	sign.setCategory(C_CONST);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setLevel(level);
	sign.setType(type);
	sign.setInitValue(init);
	push(sign);
}
//�޳�ʼ������
void SignTable::addVar(int type, char* name, int dimen, int level) {
	Sign sign;
	sign.setCategory(C_VAR);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
}
//�޳�ʼ������
void SignTable::addConst(int type, char* name, int dimen, int level) {
	Sign sign;
	sign.setCategory(C_CONST);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
}
//�г�ʼ���������
void SignTable::addVar(int type, char* name, int dimen, int dimen_n, int dimen_m, int level, vector<int> init) {
	Sign sign;
	sign.setCategory(C_VAR);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setDimenN(dimen_n);
	sign.setDimenM(dimen_m);
	sign.setLevel(level);
	sign.setType(type);
	sign.setInitValue(init);
	push(sign);
}

//�޳�ʼ���������
void SignTable::addVar(int type, char* name, int dimen, int dimen_n, int dimen_m, int level) {
	Sign sign;
	sign.setCategory(C_VAR);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setDimenN(dimen_n);
	sign.setDimenM(dimen_m);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
}

//type:��������ֵ���ͣ���ѡINTTK,CHARTK,VOIDTK	
//name:����	
//level:�㼶

void SignTable::addFunc(int type, char* name, int level) {

	Sign sign;
	sign.setCategory(C_FUNCTION);
	sign.setName(name);
	sign.setDimen(0);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
	//show();
}

//name:����	level:�㼶	name��levelȷ��һ������(����)
//paralen����������	paratype[]:ÿ������������
void SignTable::refillFunc(char* name, int level, int paralen, int paratype[]) {
	if (subTables[0].hasName(name)) {
		Sign* sign = subTables[0].getSign(name);
		sign->setF_para_len(paralen);
		for (int j = 0; j < paralen; j++) {
			sign->setF_para_type(paratype[j], j);
		}
	}
	//printf("refill:%d %d\n",flag,paralen);
	//show();
}

void SignTable::addPara(int type, char* name, int level) {
	Sign sign;
	sign.setCategory(C_PARA);
	sign.setName(name);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
	//show();
}

list<Sign*>* SignTable::getGlobalSigns() {
	list<Sign*> *l = subTables[0].getSignList();
	return l;
}
list<Sign*>* SignTable::getMainSigns() {
	char m[MAX_WORD_LEN];
	strcpy(m, "main");
	list<Sign*> *l = subTables[funname2id[m]].getSignList();
	return l;
}
list<Sign*>* SignTable::getFuncSigns() {
	list<Sign*> *l = subTables[current].getSignList();
	return l;
}

void SignTable::setCurrent(char* funcName) {
	strcpy(currentName, funcName);
	current = funname2id[currentName];
}

void SignTable::setCurrent(int cur) {
	current = cur;
}

int SignTable::getGpOffset() {
	return gp_offset;
}
int SignTable::getSpOffset() {
	return subTables[current].getSpOffset();
}

void SignTable::showMainId() {
	char m[MAX_WORD_LEN];
	strcpy(m, "main");
	printf("showMainId:%d\n", funname2id[m]);
}

void SignTable::show() {
	for (int i = 0; i < subTablesTop; i++) {
		printf("%d:\n", i);
		subTables[i].print();
	}
}