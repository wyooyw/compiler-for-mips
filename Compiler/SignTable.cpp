
#include "SignTable.h"

/*
	符号表要重构！！！
*/
/*
SignTable::SignTable() {
	signs = (Sign*)malloc(50000 * sizeof(Sign));
}

//插入一个符号
void SignTable::push(Sign sign) {

	//以下仅适用于本次实验
	if (sign.getLevel() == 1 && (sign.getCategory()== C_VAR || sign.getCategory() == C_CONST)) {
		//全局变量
		gp_offset += WORD_SIZE;
		sign.setOffset(gp_offset);
		globalVarCount++;
	}
	else if (sign.getLevel() == 2 && (sign.getCategory() == C_VAR || sign.getCategory() == C_CONST)) {
		//局部变量
		sp_offset += WORD_SIZE;
		sign.setOffset(sp_offset);
		if (mainVarCount == 0) {
			mainVarBegin = signs + top;
		}
		mainVarCount++;
	}

	signs[top] = sign;
	valid[top] = 1;
	top++;
}

//“删除”当前level最高的所有符号}
void SignTable::popTopLevel(int level) {
	int i = top-1;
	while (i>=0 && signs[i].getLevel() == level) {
		valid[i] = 0;
		i--;
	}
}

//通过名称查找当前最近的符号
bool SignTable::getSignRecent(char* name,Sign &sign) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0 ) {
		if (valid[i]==1 && strcmp(signs[i].getName(), name) == 0) {
			flag = true;
			break;
		}
		i--;
	}
	if (flag) {
		sign = signs[i];
	}
	return flag;
}

bool SignTable::havaSignInSameLevel(char* name, int level) {
	int i = top - 1;
	bool flag = false;
	
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0 && signs[i].getLevel()==level) {
			flag = true;
			break;
		}
		i--;
	}
	return flag;
	//return true;
}

bool SignTable::havaSign(char* name) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0) {
			flag = true;
			break;
		}
		i--;
	}
	return flag;
}
*/
//void SignTable::show() {
	/*printf("SignTable:\n");
	printf("name\tcateg\ttype\tlevel\tplen\tptype\n");
	for (int i = 0; i < top; i++) {
		if (valid[i] == 0) {
			continue;
		}
		printf("%s\t",signs[i].getName());
		switch (signs[i].getCategory()) {
		case C_VAR:
			printf("var\t");
			break;
		case C_FUNCTION:
			printf("func\t");
			break;
		case  C_CONST:
			printf("const\t");
			break;
		default:
			printf("OTHER\t");
			break;
		}
		switch (signs[i].getType()) {
		case INTTK:
			printf("int\t");
			break;
		case CHARTK:
			printf("char\t");
			break;
		default:
			printf("OTHER\t");
			break;
		}
		//printf("%s\t", WORD_TYPE_STR[signs[i].getType()]);
		printf("%d\t",signs[i].getLevel());
		if (signs[i].getCategory() == C_FUNCTION) {
			printf("%d\t", signs[i].getF_para_len());
			for (int j = 0; j < signs[i].getF_para_len(); j++) {
				printf("%d\t", signs[i].getF_para_type(j));
			}
		}
		printf("\n");
	}*/
//}
/*
void SignTable::addVar(int type, char* name, int dimen, int level,int init) {
	Sign sign;
	sign.setCategory(C_VAR);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setLevel(level);
	sign.setType(type);
	sign.setInitValue(init);
	push(sign);
	show();
}
void SignTable::addConst(int type, char* name, int dimen, int level,int init) {
	Sign sign;
	sign.setCategory(C_CONST);
	sign.setName(name);
	sign.setDimen(dimen);
	sign.setLevel(level);
	sign.setType(type);
	sign.setInitValue(init);
	push(sign);
	show();
}

//type:函数返回值类型，可选INTTK,CHARTK,VOIDTK	
//name:名称	
//level:层级

void SignTable::addFunc(int type, char* name, int level) {
	
	Sign sign;
	sign.setCategory(C_FUNCTION);
	sign.setName(name);
	sign.setDimen(0);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
	show();
}

//name:名称	level:层级	name和level确定一个符号(函数)
//paralen：参数个数	paratype[]:每个参数的类型
void SignTable::refillFunc(char* name, int level, int paralen, int paratype[]) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0 && signs[i].getLevel() == level) {
			flag = true;
			break;
		}
		i--;
	}
	if (flag) {
		signs[i].setF_para_len(paralen);
		for (int j = 0; j < paralen; j++) {
			signs[i].setF_para_type(paratype[j], j);
		}
	}
	//printf("refill:%d %d\n",flag,paralen);
	show();
}

//1:参数长度不一致	2：参数类型不一致	3：都一致	0：没有同名函数

void SignTable::checkPara(char* name,int paralen,int paratype[],int row) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0) {
			flag = true;
			break;
		}
		i--;
	}
	if (flag) {
		if (paralen != signs[i].getF_para_len()) {
			Error::paraLenError(row);
		}
		for (int j = 0; j < paralen; j++) {
			if (paratype[j] != signs[i].getF_para_type(j)) {
				Error::paraTypeError(row);
			}
		}
		return;
	}
	else {
		//printf("aa\n");
		Error::noDefError(row);
	}
}

void SignTable::addPara(int type, char* name, int level) {
	Sign sign;
	sign.setCategory(C_PARA);
	sign.setName(name);
	sign.setLevel(level);
	sign.setType(type);
	push(sign);
	show();
}

int SignTable::getSignType(char* name) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0) {
			return signs[i].getType();
			break;
		}
		i--;
	}
	return -1;
}

int SignTable::getSignCategory(char* name) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0) {
			return signs[i].getCategory();
			break;
		}
		i--;
	}
	return -1;
}
void SignTable::checkReturn( int retType,int row) {
	Sign sign;
	//通过名称查找当前最近的符号
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && signs[i].getCategory() == C_FUNCTION) {
			flag = true;
			break;
		}
		i--;
	}
	if (flag) {
		signs[i].setF_return(retType);
		int type = signs[i].getType();
		if (type == VOIDTK && retType != VOIDTK) {
			Error::haveReturnError(row);
		}
		else if (type != retType) {
			Error::returnError(row);
		}
	}
}

int SignTable::getSignReturn(char* name) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (valid[i] == 1 && strcmp(signs[i].getName(), name) == 0) {
			return signs[i].getF_return();
			break;
		}
		i--;
	}
	return -1;
}



bool SignTable::getSign(char* name, Sign& sign) {
	int i = top - 1;
	bool flag = false;
	while (i >= 0) {
		if (strcmp(signs[i].getName(), name) == 0) {
			flag = true;
			break;
		}
		i--;
	}
	if (flag) {
		sign = signs[i];
	}
	return flag;
}

int SignTable::getGpOffset() {
	return gp_offset;
}
int SignTable::getSpOffset() {
	return sp_offset;
}

void SignTable::getGlobalSigns(Sign*& begin, int& len) {
	begin = signs;
	len = globalVarCount;
}
void SignTable::getMainSigns(Sign*& begin, int& len) {
	begin = mainVarBegin;
	len = mainVarCount;
}*/