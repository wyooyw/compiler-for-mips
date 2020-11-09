#pragma once
#include "Word.h"
#include "Error.h"
#include <fstream>
class Output {
public:
	static void setExp(int exp);
	static void printWord(Word word);
	static void printGrammar(const char* grammar);
	static void printError(Error error);
	static void close();
	static int exp;
};