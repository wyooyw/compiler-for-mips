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
	fout << "addu $" << rd << ",$" << rs << ",$" << rt << endl;
}
void Output::addi(int rt, int rs, int imm) {
	if (exp != EXP4) return;
	fout << "addiu $" << rt << ",$" << rs << "," << imm << endl;
}

void Output::sub(int rd, int rs, int rt) {
	if (exp != EXP4) return;
	fout << "subu $" << rd << ",$" << rs << ",$" << rt << endl;
}

void Output::subi(int rt, int rs, int imm) {
	if (exp != EXP4) return;
	fout << "subiu $" << rt << ",$" << rs << "," << imm << endl;
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
void Output::mfhi(int rs) {
	if (exp != EXP4) return;
	fout << "mfhi $" << rs << endl;
}
void Output::lui(int rs, int imm) {
	if (exp != EXP4) return;
	fout << "lui $" << rs << ","<<imm<< endl;
}
void Output::ori(int rt, int rs, int imm) {
	if (exp != EXP4) return;
	fout << "ori $" << rt << ",$"<<rs<<"," << imm << endl;
}

void Output::beq(int rs, int rt, int label) {
	if (exp != EXP4) return;
	fout << "beq $" << rs << ",$" << rt << "," << "label" << label << endl;
}
void Output::bgez(int rs, int label) {
	if (exp != EXP4) return;
	fout << "bgez $" << rs << "," << "label" << label << endl;
}
void Output::bgtz(int rs, int label) {
	if (exp != EXP4) return;
	fout << "bgtz $" << rs << "," << "label" << label << endl;
}
void Output::blez(int rs, int label) {
	if (exp != EXP4) return;
	fout << "blez $" << rs << "," << "label" << label << endl;
}
void Output::bltz(int rs, int label) {
	if (exp != EXP4) return;
	fout << "bltz $" << rs << "," << "label" << label << endl;
}
void Output::bne(int rs, int rt, int label) {
	if (exp != EXP4) return;
	fout << "bne $" << rs << ",$" << rt << "," << "label" << label << endl;
}

void Output::j(int label) {
	if (exp != EXP4) return;
	fout << "j label" << label << endl;
}

void Output::jr(int rs) {
	if (exp != EXP4) return;
	fout << "jr $" << rs << endl;
}

void Output::jal(int label) {
	if (exp != EXP4) return;
	fout << "jal label" << label << endl;
}

void Output::jal(char* label) {
	if (exp != EXP4) return;
	fout << "jal f_" << label << endl;
}

void Output::label(int l) {
	if (exp != EXP4) return;
	fout << "label" << l << ":" << endl;
}

void Output::label(char* name) {
	if (exp != EXP4) return;
	fout << "f_" << name << ":" << endl;
}


void Output::note(const char* str) {
	if (exp != EXP4) return;
	fout << "#" << str << endl;
}


void Output::sll(int rd, int rt, int s) {
	if (exp != EXP4) return;
	fout << "sll $" << rd << ",$" << rt << ","<< s << endl;
}

void Output::srl(int rd, int rt, int s) {
	if (exp != EXP4) return;
	fout << "srl $" << rd << ",$" << rt << "," << s << endl;
}
void Output::sllv(int rd, int rt, int rs) {
	if (exp != EXP4) return;
	fout << "sllv $" << rd << ",$" << rt << ",$" << rs << endl;
}

void Output::srlv(int rd, int rt, int rs) {
	if (exp != EXP4) return;
	fout << "srav $" << rd << ",$" << rt << ",$" << rs << endl;
}