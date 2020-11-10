#include "Output.h"

const string OUT_PATH = "./output.txt";
ofstream fout(OUT_PATH);
enum EXP { EXP1, EXP2, EXP3, EXP4 };
int Output::exp;
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