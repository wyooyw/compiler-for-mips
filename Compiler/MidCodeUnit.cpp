#include "MidCodeUnit.h"
#include <stdio.h>

int MidCodeUnit::get(int name) {
	return codeunit[name];
}
void MidCodeUnit::set(int name, int value) {
	codeunit[name] = value;
}

int MidCodeUnit::getType() {
	return type;
}
void MidCodeUnit::setType(int para_type) {
	type = para_type;
}

void MidCodeUnit::print() {
	switch (type) {

	default:
		printf("other");
	}
}