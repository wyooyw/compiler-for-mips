#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserConstDeclare.cpp
	包含<常量说明>、<常量定义>
*/

//常量说明
void GrammarAnalyser::g_const_declare() {
	if (word.getType() == CONSTTK) {
		getWord();
		g_const_def();

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();

		/*while (true) {
			if (tryWord(1) && tryword.getType() == CONSTTK) {
				getWord();

				getWord();
				g_const_def();

				//可能缺失的分号
				tryWord(1);
				if (tryword.getType() != SEMICN) Error::semiError(getRow());
				else getWord();

			}
			else {
				Output::printGrammar("<常量说明>");
				return;
			}


		}*/
	}
	else {
		Output::printGrammar("a");
		goError();
	}
}

//常量定义
void GrammarAnalyser::g_const_def() {
	char name[MAX_WORD_LEN];
	int valueType;
	int value;
	int type = word.getType();
	if (word.getType() == INTTK) {
		getWord();
		if (word.getType() != IDENFR)  goError();

		strcpy(name, word.getSmallword());//保存名字

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		g_int(value);

		//添加进符号表
		if (signTable->havaSignInSameLevel(name, level)) Error::reDefError(getRow());
		else signTable->addConst(INTTK, name, 0, level,value);
		
		while (true) {

			if (tryWord(1) && tryword.getType() == COMMA) {
				getWord();

				getWord();
				if (word.getType() != IDENFR) goError();

				strcpy(name, word.getSmallword());//保存名字

				getWord();
				if (word.getType() != ASSIGN) goError();

				getWord();
				if(word.getType()==CHARCON) Error::constTypeError(getRow());
				g_int(value);

				//添加进符号表
				if (signTable->havaSignInSameLevel(name, level)) Error::reDefError(getRow());
				else signTable->addConst(INTTK, name, 0, level,value);
			}
			else {
				Output::printGrammar("<常量定义>");
				return;
			}
		}
	}
	else if (word.getType() == CHARTK) {
		getWord();
		if (word.getType() != IDENFR) goError();

		strcpy(name,word.getSmallword());

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		if (word.getType() != CHARCON) goError();

		value = word.getWord()[0];

		//添加进符号表
		if (signTable->havaSignInSameLevel(name, level)) Error::reDefError(getRow());
		else signTable->addConst(CHARTK, name, 0, level,value);
		
		while (true) {
			if (tryWord(1) && tryword.getType() == COMMA) {
				getWord();

				getWord();
				if (word.getType() != IDENFR) goError();

				strcpy(name, word.getSmallword());
	
				getWord();
				if (word.getType() != ASSIGN) goError();

				getWord();
				if (word.getType() != CHARCON) Error::constTypeError(getRow());
				value = word.getWord()[0];

				//添加进符号表
				if (signTable->havaSignInSameLevel(name, level)) Error::reDefError(getRow());
				else signTable->addConst(CHARTK, name, 0, level,value);

			}
			else {
				Output::printGrammar("<常量定义>");
				return;
			}
		}
	}
	else {
		goError();
	}
	getWord();
}

