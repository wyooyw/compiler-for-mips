
#include "SubTable.h"

//检测是否有该名
bool SubTable::hasName(char* name) {
	map<string, Sign>::iterator iter = signs.find(name);
	return iter != signs.end();
}

//获取符号
Sign* SubTable::getSign(char* name) {
	map<string, Sign>::iterator iter = signs.find(name);
	return &(iter->second);
}

//添加符号
void SubTable::addSign(Sign sign) {
	sign.setOffset(sp_offset);

	signs[sign.getName()] = sign;

	//应该先改offsex还是先setOffset?
	if (sign.getDimen() == 1) {
		sp_offset += WORD_SIZE * sign.getDimenN();
	}
	else if (sign.getDimen() == 2) {
		sp_offset += WORD_SIZE * sign.getDimenN() * sign.getDimenM();
	}
	else {
		sp_offset += WORD_SIZE;
	}
}

//获取所有符号
list<Sign*>* SubTable::getSignList() {
	list<Sign*> *l = new list<Sign*>;
	map<string, Sign>::iterator iter;
	for (iter = signs.begin(); iter != signs.end(); iter++) {
		if (iter->second.getCategory() != C_FUNCTION) {
			l->push_back(&(iter->second));
		}
	}
	return l;
}

void SubTable::print() {

	map<string, Sign>::iterator iter;
	Sign sign;
	for (iter = signs.begin(); iter != signs.end(); iter++) {
		sign = iter->second;
		printf("\tname=%s \t type=%d \t dimen = %d \t", sign.getName(),sign.getType(),sign.getDimen());
		if (sign.getDimen() == 1) {
			printf("dimenN=%d \n", sign.getDimenN());
		}
		else if (sign.getDimen() == 2) {
			printf("dimenN=%d dimenM=%d\n", sign.getDimenN(), sign.getDimenM());
		}
		else {
			printf("\n");
		}
		printf("\t初值：");
		//初值
		if (sign.hasInitValue()) {
			if (sign.getDimen() == 0) {
				printf("%d\n", sign.getInitValue());
			}
			else if (sign.getDimen() == 1) {
				int dimen_n = sign.getDimenN();
				printf("{");
				for (int i = 0; i < dimen_n; i++) {
					printf("%d,",sign.getInitValue(i));
				}
				printf("}\n");
			}
			else {
				int dimen_n = sign.getDimenN();
				int dimen_m = sign.getDimenM();
				printf("{");
				for (int i = 0; i < dimen_n; i++) {
					printf("{");
					for (int j = 0; j < dimen_m; j++) {
						printf("%d,", sign.getInitValue(i,j));
					}
					printf("},\n");
				}
				printf("}\n");
			}
		}
		else {
			printf("无\n");
		}
		
	}
}
int SubTable::getSpOffset() {
	return sp_offset;
}

int SubTable::getId() {
	return id;
}
void SubTable::setId(int id) {
	this->id = id;
}