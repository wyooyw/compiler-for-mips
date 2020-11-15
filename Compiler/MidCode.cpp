#include "MidCode.h"

void MidCode::add(MidCodeUnit* unit) {
	midcodes.push_back(unit);
}

void MidCode::add(vector<MidCodeUnit*> units) {
	int len = units.size();
	for (int i = 0; i < len; i++) {
		midcodes.push_back(units[i]);
	}
}

void MidCode::add(MidCode* para_midcode) {
	midcodes.insert(midcodes.end(), para_midcode->midcodes.begin(), para_midcode->midcodes.end());
}

void MidCode::print() {
	int len = midcodes.size();
	for (int i = 0; i < len; i++) {
		midcodes[i]->print();
	}
}
