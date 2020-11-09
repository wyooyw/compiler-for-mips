#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "Error.h"
#include "Word.h"
#include "Output.h"

#ifndef LEXICAL_ANALYSER_H
#define LEXICAL_ANALYSER_H

class LexicalAnalyser {
public:
	LexicalAnalyser();
	bool readWord(Word& word);
	bool tryReadWord(Word& word, int l);
	int getRow();
	//Word& getTryReadWord(int i);
private:
	char* buffer;
	long buffer_size;
	char* buf_ptr;
	Word tryWord[10];
	int row = 1;
	map<string, WORD_TYPE> keyword_table;
	map<char, WORD_TYPE> sign_table;
	void readfile();
	void init();
	void initKeywordTable();
	void initSignTable();
	void passSpace(bool recordrow);
	bool isDigital(char c);
	bool isLetter(char c);
	bool isChar(char c);
	bool isPlus(char c);
	bool isMul(char c);
	bool isValidInString(char c);
	bool nextWord(Word& word,bool recordrow);
};
#endif