#include "Output.h"
int Output::exp;
const string OUT_PATH = "./mips.txt";
ofstream fout(OUT_PATH);
void Output::setExp(int e) {
	exp = e;
}
void Output::printWord(Word word) {
	if (exp == EXP1 || exp == EXP2) {
		fout << WORD_TYPE_STR[word.getType()] << " " << word.getWord() << endl;
	}
}
void Output::printGrammar(const char* grammar) {
	if (exp == EXP2) {
		fout << grammar << endl;
	}
}
void Output::close() {
	fout.close();
}
void Output::printError(Error error) {
	if (exp == EXP3) {
		fout << error.getRow() << " " << error.getType() << endl;
	}
}

//»ã±àÏà¹Ø
void Output::data() {
	if (exp != EXP4) return;
	fout << ".data" << endl;
}
void Output::text() {
	if (exp != EXP4) return;
	fout << ".text" << endl;
}
void Output::str(char* s) {
	if (exp != EXP4) return;
	static int index = 0;
	index++;
	fout << "STR" << index << ": .asciiz \"" << s << "\"" << endl;
}
void Output::lw(int rt, int offset, int base) {
	if (exp != EXP4) return;
	fout << "lw " << "$" << rt << "," << offset << "($" << base << ")" << endl;
}
void Output::sw(int rt, int offset, int base) {
	if (exp != EXP4) return;
	fout << "sw " << "$" << rt << "," << offset << "($" << base << ")" << endl;
}
void Output::add(int rd, int rs, int rt) {
	if (exp != EXP4) return;
	fout << "add $" << rd << ",$" << rs << ",$" << rt << endl;
}
void Output::addi(int rt, int rs, int imm) {
	if (exp != EXP4) return;
	fout << "addi $" << rt << ",$" << rs << "," << imm << endl;
}

void Output::sub(int rd, int rs, int rt) {
	if (exp != EXP4) return;
	fout << "sub $" << rd << ",$" << rs << ",$" << rt << endl;
}
void Output::mult(int rs, int rt) {
	if (exp != EXP4) return;
	fout << "mult $" << rs << ",$" << rt << endl;
}
void Output::div(int rs, int rt) {
	if (exp != EXP4) return;
	fout << "div $" << rs << ",$" << rt << endl;
}

void Output::syscall() {
	if (exp != EXP4) return;
	fout << "syscall" << endl;
}
void Output::la(int rs, int rt) {
	if (exp != EXP4) return;
	fout << "la $" << rs << ",$" << rt << endl;
}
void Output::mflo(int rs) {
	if (exp != EXP4) return;
	fout << "mflo $" << rs << endl;
}
void Output::lui(int rs, int imm) {
	if (exp != EXP4) return;
	fout << "lui $" << rs << ","<<imm<< endl;
}
void Output::ori(int rt, int rs, int imm) {
	if (exp != EXP4) return;
	fout << "ori $" << rt << ",$"<<rs<<"," << imm << endl;
}

void Output::note(const char* str) {
	if (exp != EXP4) return;
	//fout << "#" << str << endl;
}