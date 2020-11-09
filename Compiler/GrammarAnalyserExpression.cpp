#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserExpression.cpp
	包含<表达式>、<因子>、<项>
*/

//表达式
void GrammarAnalyser::g_expression(int &type) {
	
	if (word.getType() == PLUS || word.getType() == MINU) {
		getWord();
	}
	g_term(type);

	while (true) {
		if (tryWord(1) &&
			(tryword.getType() == PLUS || tryword.getType() == MINU)) {
			getWord();
			g_plus();

			getWord();
			g_term(type);

			type = INTTK;
		}
		else {
			break;
		}
	}
	Output::printGrammar("<表达式>");
}

//项
void GrammarAnalyser::g_term(int &type) {
	g_factor(type);

	while (true) {
		if (tryWord(1) &&
			(tryword.getType() == MULT || tryword.getType() == DIV)) {
			getWord();
			g_mul();

			getWord();
			g_factor(type);

			type = INTTK;
		}
		else {
			break;
		}
	}
	Output::printGrammar("<项>");
}

//因子
void GrammarAnalyser::g_factor(int &type) {
	
	if (word.getType() == LPARENT) {
		getWord();
		g_expression(type);

		type = INTTK;
		//可能缺失的右括号
		tryWord(1);
		if (tryword.getType() != RPARENT) Error::parentError(getRow());
		else getWord();
	}
	else if (word.getType() == CHARCON) {
		type = CHARTK;
	}
	else if (word.getType() == PLUS || word.getType() == MINU || word.getType() == INTCON) {
		
		g_int();
		type = INTTK;
	}
	else if (word.getType() == IDENFR && 
		tryWord(1) && tryword.getType() == LPARENT) {//函数
		//有引用的标识符，内含判断该标识符是否已在符号表中创建
		g_call_iden();

		type = signTable.getSignType(word.getSmallword());

		g_func_ret_call();
	}
	else if (word.getType() == IDENFR) {
		//有引用的标识符，内含判断该标识符是否已在符号表中创建
		g_call_iden();

		type = signTable.getSignType(word.getSmallword());
		int indexType;
		tryWord(1);
		if (tryword.getType() == LBRACK) {
			getWord();
			getWord();
			g_expression(indexType);

			//数组下标不是整型
			if (indexType != INTTK) Error::indexTypeError(getRow());

			//可能缺失的右中括号
			tryWord(1);
			if (tryword.getType() != RBRACK)  Error::brackError(getRow());
			else getWord();
		}

		tryWord(1);
		if (tryword.getType() == LBRACK) {
			getWord();
			getWord();
			g_expression(indexType);

			//数组下标不是整型
			if (indexType != INTTK) Error::indexTypeError(getRow());

			//可能缺失的右中括号
			tryWord(1);
			if (tryword.getType() != RBRACK)  Error::brackError(getRow());
			else getWord();
		}

	}
	

	Output::printGrammar("<因子>");
}