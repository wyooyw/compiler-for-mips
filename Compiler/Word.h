#include "Error.h"
#include <string>
#include <string.h>

#define MAX_WORD_LEN 200

#ifndef WORD_H
#define WORD_H
using namespace std;
enum WORD_TYPE {
	IDENFR, INTCON, CHARCON, STRCON, CONSTTK,
	INTTK, CHARTK, VOIDTK, MAINTK, IFTK,
	ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK,
	FORTK, SCANFTK, PRINTFTK, RETURNTK, PLUS,
	MINU, MULT, DIV, LSS, LEQ,
	GRE, EQL, GEQ, NEQ, COLON,
	ASSIGN, SEMICN, COMMA, LPARENT, RPARENT,
	LBRACK, RBRACK, LBRACE, RBRACE
};
const string WORD_TYPE_STR[] = {
	"IDENFR", "INTCON", "CHARCON", "STRCON", "CONSTTK",
		"INTTK", "CHARTK", "VOIDTK", "MAINTK", "IFTK",
		"ELSETK", "SWITCHTK", "CASETK", "DEFAULTTK", "WHILETK",
		"FORTK", "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS","MINU",
		"MULT", "DIV", "LSS", "LEQ", "GRE",
		"EQL", "GEQ", "NEQ", "COLON", "ASSIGN",
		"SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACK",
		"RBRACK", "LBRACE", "RBRACE"
};
class Word { 
public:
	char* getWord();
	void setWord(char* str);
	WORD_TYPE getType();
	void setType(const WORD_TYPE t);
	Error getError();
	void setError(Error err);
	char* getSmallword();
private:
	char word[MAX_WORD_LEN];
	char smallword[MAX_WORD_LEN];
	WORD_TYPE type;
	Error error;
};


#endif