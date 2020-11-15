#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserExpression.cpp
	包含<表达式>、<因子>、<项>
*/

//表达式
void GrammarAnalyser::g_expression(ASTNode*& expression,int &type) {
	ASTNodeFactory* factory = new ASTNodeFactory();
	//ASTNode* expression;
	ASTNode* term;
	int sign;
	if (word.getType() == PLUS || word.getType() == MINU) {
		sign = word.getType();
		getWord();
		g_term(term,type);
		expression = factory->makeASTNodeFactor(sign,factory->makeASTNodeInt(0), term);
	}
	else {
		g_term(term,type);
		expression = term;
	}
	

	while (true) {
		if (tryWord(1) &&
			(tryword.getType() == PLUS || tryword.getType() == MINU)) {
			getWord();
			g_plus();
			sign = word.getType();

			getWord();
			g_term(term,type);

			
			type = INTTK;
			expression = factory->makeASTNodeFactor(sign,expression, term);
		}
		else {
			break;
		}
	}
	//expression->print();
	Output::printGrammar("<表达式>");
}

//表达式(old)
void GrammarAnalyser::g_expression(int& type) {
	ASTNodeFactory* factory = new ASTNodeFactory();
	ASTNode* expression;
	ASTNode* term;
	int sign;
	if (word.getType() == PLUS || word.getType() == MINU) {
		sign = word.getType();
		getWord();
		g_term(term, type);
		expression = factory->makeASTNodeFactor(sign, factory->makeASTNodeInt(0), term);
	}
	else {
		g_term(term, type);
		expression = term;
	}


	while (true) {
		if (tryWord(1) &&
			(tryword.getType() == PLUS || tryword.getType() == MINU)) {
			getWord();
			g_plus();
			sign = word.getType();

			getWord();
			g_term(term, type);


			type = INTTK;
			expression = factory->makeASTNodeFactor(sign, expression, term);
		}
		else {
			break;
		}
	}
	//expression->print();
	Output::printGrammar("<表达式>");
}

//项
void GrammarAnalyser::g_term(ASTNode* &term,int &type) {
	ASTNodeFactory* factory = new ASTNodeFactory();
	ASTNode* factor;
	int sign;
	g_factor(factor,type);

	term = factor;

	while (true) {
		if (tryWord(1) &&
			(tryword.getType() == MULT || tryword.getType() == DIV)) {
			getWord();
			g_mul();
			sign = word.getType();

			getWord();
			g_factor(factor,type);

			type = INTTK;

			term = factory->makeASTNodeFactor(sign, term, factor);
		}
		else {
			break;
		}
	}
	Output::printGrammar("<项>");
}

//因子
void GrammarAnalyser::g_factor(ASTNode* &factor,int &type) {
	ASTNodeFactory* factory = new ASTNodeFactory();
	//ASTNode* node;
	int value;
	if (word.getType() == LPARENT) {
		getWord();
		g_expression(factor,type);

		type = INTTK;
		//可能缺失的右括号
		tryWord(1);
		if (tryword.getType() != RPARENT) Error::parentError(getRow());
		else getWord();
	}
	else if (word.getType() == CHARCON) {
		type = CHARTK;
		factor = factory->makeASTNodeChar(word.getWord()[0]);
	}
	else if (word.getType() == PLUS || word.getType() == MINU || word.getType() == INTCON) {
		g_int(value);
		type = INTTK;

		factor = factory->makeASTNodeInt(value);
	}
	else if (word.getType() == IDENFR && 
		tryWord(1) && tryword.getType() == LPARENT) {//函数
		//有引用的标识符，内含判断该标识符是否已在符号表中创建
		g_call_iden();

		type = signTable->getSignType(word.getSmallword());

		g_func_ret_call();
	}
	else if (word.getType() == IDENFR) {
		//有引用的标识符，内含判断该标识符是否已在符号表中创建
		g_call_iden(factor);

		type = signTable->getSignType(word.getSmallword());
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