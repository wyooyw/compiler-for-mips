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

		while (true) {
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


		}
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
	int type = word.getType();
	if (word.getType() == INTTK) {
		getWord();
		if (word.getType() != IDENFR)  goError();

		//添加进符号表
		if (signTable.havaSignInSameLevel(word.getSmallword(), level)) Error::reDefError(getRow());
		else signTable.addConst(INTTK, word.getSmallword(), 0, level);

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		g_int();

		while (true) {

			if (tryWord(1) && tryword.getType() == COMMA) {
				getWord();

				getWord();
				if (word.getType() != IDENFR) goError();

				//添加进符号表
				if (signTable.havaSignInSameLevel(word.getSmallword(), level)) Error::reDefError(getRow());
				else signTable.addConst(INTTK, word.getSmallword(), 0, level);

				getWord();
				if (word.getType() != ASSIGN) goError();

				getWord();
				if(word.getType()==CHARCON) Error::constTypeError(getRow());
				g_int();


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

		//添加进符号表
		if (signTable.havaSignInSameLevel(word.getSmallword(), level)) Error::reDefError(getRow());
		else signTable.addConst(CHARTK, word.getSmallword(), 0, level);

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		if (word.getType() != CHARCON) goError();

		while (true) {
			if (tryWord(1) && tryword.getType() == COMMA) {
				getWord();

				getWord();
				if (word.getType() != IDENFR) goError();

				//添加进符号表
				if (signTable.havaSignInSameLevel(word.getSmallword(), level)) Error::reDefError(getRow());
				else signTable.addConst(CHARTK, word.getSmallword(), 0, level);
	
				getWord();
				if (word.getType() != ASSIGN) goError();

				getWord();
				if (word.getType() != CHARCON) Error::constTypeError(getRow());

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

