#include "Sign.h"
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