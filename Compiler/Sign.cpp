#include "Sign.h"
int Sign::getId() {
	return id;
}
void Sign::setId(int i) {
	id = i;
}

void Sign::setName(char* n) {
	strcpy(name, n);
}
char* Sign::getName() {
	return name;
}
void Sign::setLevel(int l) {
	level = l;
}
int Sign::getLevel() {
	return level;
}
void Sign::setCategory(int l) {
	category = l;
}
int Sign::getCategory() {
	return category;
}
void Sign::setType(int l) {
	type = l;
}
int Sign::getType() {
	return type;
}
void Sign::setDimen(int l) {
	dimen = l;
}
int Sign::getDimen() {
	return dimen;
}
void Sign::setDimenN(int d) {
	dimen_n = d;
}
int Sign::getDimenN() {
	return dimen_n;
}
void Sign::setDimenM(int d) {
	dimen_m = d;
}
int Sign::getDimenM() {
	return dimen_m;
}
void Sign::setF_para_len(int l) {
	f_para_len = l;
}
int Sign::getF_para_len() {
	return f_para_len;
}
void Sign::setF_para_type(int l, int i) {
	f_para_type[i] = l;
}
int Sign::getF_para_type(int i) {
	return f_para_type[i];
}
void Sign::setF_return(int r) {
	f_return = r;
}
int Sign::getF_return() {
	return f_return;
}
int Sign::getOffset() {
	return offset;
}
void Sign::setOffset(int para_offset) {
	offset = para_offset;
}

int Sign::getBase() {
	return base;
}
void Sign::setBase(int para_base) {
	base = para_base;
}

int Sign::getInitValue() {
	return initValue[0];
}
void Sign::setInitValue(int para_initvalue) {
	if (initValue.size() == 0) {
		initValue.push_back(para_initvalue);
	}
	else {
		initValue[0] = para_initvalue;
	}
	has_init_value = true;
}
//一维数组
int Sign::getInitValue(int n) {
	if (n >= dimen_n) {
		return -1;
	}
	return initValue[n];
}
//二维数组
int Sign::getInitValue(int n, int m) {
	if (n >= dimen_n) {
		return -1;
	}
	if (m >= dimen_m) {
		return -1;
	}
	return initValue[n * dimen_m + m];
}
//数组
void Sign::setInitValue(vector<int> arr) {
	initValue = arr;
	has_init_value = true;
}

bool Sign::hasInitValue() {
	return has_init_value;
}