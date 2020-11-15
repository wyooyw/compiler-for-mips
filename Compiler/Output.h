#pragma once
#include "Word.h"
#include "Error.h"
#include <fstream>
enum EXP { EXP1, EXP2, EXP3, EXP4 };

class Output {
public:
	static void setExp(int exp);
	static void printWord(Word word);
	static void printGrammar(const char* grammar);
	static void printError(Error error);
	static void close();
	static int exp;

	//MIPSª„±‡œ‡πÿ
	static void data();
	static void text();

	static void lw( int rt, int offset, int base);
	static void sw( int rt, int offset,int base);
	static void add(int rd,int rs, int rt);
	static void addi(int rt, int rs, int imm);

	static void sub(int rd, int rs, int rt);
	static void mult(int rs, int rt);
	static void div(int rs, int rt);

	static void syscall();
	static void la(int rs, int rt);
	static void mflo(int rs);

	static void str(char* s);

	static void lui(int rs,int imm);
	static void ori(int rt, int rs, int imm);

	static void note(const char *str);
private:
};