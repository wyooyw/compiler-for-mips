#include "Register.h"

bool Register::isFree() {
	return type == 0;
}
bool Register::isTemp() {
	return type == 1;
}
bool Register::isVar() {
	return type == 2;
}
void Register::setFree() {
	type = 0;
}
void Register::setTemp() {
	type = 1;
}
void Register::setVar() {
	type = 2;
}
int Register::getRegId() {
	return regId;
}
void Register::setRegId(int id) {
	regId = id;
}
//base,offset
pair<int, int> Register::getReflect() {
	return make_pair(reflect_base, reflect_offset);
}
void Register::setReflect(pair<int, int> addr) {
	reflect_base = addr.first;
	reflect_offset = addr.second;
}
int Register::getAllocTime() {
	return alloc_time;
}
void Register::setAllocTime(int time) {
	alloc_time = time;
}

int Register::getVarId() {
	return var_id;
}
void Register::setVarId(int id) {
	var_id = id;
}
int Register::getArrN() {
	return arr_n;
}
void Register::setArrN(int n) {
	arr_n = n;
}
int Register::getArrM() {
	return arr_m;
}
void Register::setArrM(int m) {
	arr_m = m;
}

bool Register::isChange() {
	return change;
}
void Register::setChange(bool change) {
	this->change = change;
}