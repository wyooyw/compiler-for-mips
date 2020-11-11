#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserStatement.cpp
	包含
*/

//语句
void GrammarAnalyser::g_statement(ASTNode* &statement) {
	if (word.getType() == WHILETK || word.getType() == FORTK) {
		g_loop_statement();
	}
	else if (word.getType() == IFTK) {
		g_cond_statement();
	}
	else if (word.getType() == PRINTFTK) {
		//读语句
		g_printf_statement(statement);

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();
	}
	else if (word.getType() == SCANFTK) {
		//写语句
		g_scanf_statement(statement);

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();
	}
	else if (word.getType() == SWITCHTK) {
		//情况语句
		g_switch_statement();
	}
	else if (word.getType() == SEMICN) {
		//空语句
		//getWord();
	}
	else if (word.getType() == RETURNTK) {
		//返回语句
		g_return_statement();

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();
	}
	else if (word.getType() == LBRACE) {
		//getWord();
		g_statement_list(statement);

		getWord();
		if (word.getType() != RBRACE) goError();

	}
	else if (word.getType() == IDENFR) {
		
		//有返回值函数语句、无返回值函数语句 或 赋值语句
		if (tryWord(1) && tryword.getType()==LPARENT) {
			if (hasReturnValue(word.getWord())) {
				g_func_ret_call();
			}
			else {
				g_func_no_ret_call();
			}

			//可能缺失的分号
			tryWord(1);
			if (tryword.getType() != SEMICN) Error::semiError(getRow());
			else getWord();
		}
		else {
			g_assign_statement(statement);

			//可能缺失的分号
			tryWord(1);
			if (tryword.getType() != SEMICN) Error::semiError(getRow());
			else getWord();
		}
	}

	Output::printGrammar("<语句>");
}

//赋值语句
void GrammarAnalyser::g_assign_statement(ASTNode* &astnode_assign) {
	ASTNodeFactory* factory = new ASTNodeFactory();
	ASTNode* astnode_expression;
	ASTNode* astnode_var;

	//if (word.getType() != IDENFR) goError();
	//有引用的标识符，内含判断该标识符是否已在符号表中创建
	g_call_iden(astnode_var);
	//printf("\n---%s---\n", astnode_var->getValueStr());
	if (signTable.getSignCategory(word.getSmallword()) == C_CONST) Error::changeConstError(getRow());

	int indexType;
	int valueType;
	getWord();
	if (word.getType() == LBRACK) {
		getWord();
		g_expression(indexType);
		
		//数组下标不是整型
		if (indexType != INTTK) Error::indexTypeError(getRow());

		//  ]
		tryWord(1);
		if (tryword.getType() != RBRACK)  Error::brackError(getRow()); 
		else getWord();

		getWord();
		if (word.getType() == LBRACK) {
			getWord();
			g_expression(indexType);

			//数组下标不是整型
			if (indexType != INTTK) Error::indexTypeError(getRow());

			//  ]
			tryWord(1);
			if (tryword.getType() != RBRACK)  Error::brackError(getRow());
			else getWord();

			getWord();
		}
	}
	if (word.getType() != ASSIGN) goError();
	
	getWord();
	g_expression(astnode_expression,valueType);

	astnode_assign = factory->makeASTNodeAssign(astnode_var, astnode_expression);
		
	Output::printGrammar("<赋值语句>");
}
//条件
void GrammarAnalyser::g_cond() {
	int valueType;
	g_expression(valueType);

	if (valueType == CHARTK) Error::condTypeError(getRow());

	getWord();
	g_relation();

	getWord();
	g_expression(valueType);

	if (valueType == CHARTK) Error::condTypeError(getRow());

	Output::printGrammar("<条件>");
}

//条件语句
void GrammarAnalyser::g_cond_statement() {
	ASTNode* astnode_just_temporary;

	if (word.getType() != IFTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	getWord();
	g_cond();

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	g_statement(astnode_just_temporary);

	if (tryWord(1) && tryword.getType() == ELSETK) {
		getWord();
		getWord();
		g_statement(astnode_just_temporary);
	}

	Output::printGrammar("<条件语句>");
}

//步长
void GrammarAnalyser::g_loop_step() {
	g_unsigned_int();
	Output::printGrammar("<步长>");
}

//循环语句
void GrammarAnalyser::g_loop_statement() {
	ASTNode* astnode_just_temporary;
	if (word.getType() == WHILETK) {
		getWord();
		if (word.getType() != LPARENT) goError();

		getWord();
		g_cond();

		//可能缺失的右括号
		tryWord(1);
		if (tryword.getType() != RPARENT) Error::parentError(getRow());
		else getWord();

		getWord();
		g_statement(astnode_just_temporary);
	}
	else if (word.getType() == FORTK) {
		int t;

		getWord();
		if (word.getType() != LPARENT) goError();

		getWord();
		//if (word.getType() != IDENFR) goError();
		g_call_iden();

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		g_expression(t);

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();
		

		getWord();
		g_cond();

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();

		getWord();
		//if (word.getType() != IDENFR) goError();
		g_call_iden();

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		g_call_iden();
		//if (word.getType() != IDENFR) goError();

		getWord();
		if (word.getType() != PLUS && word.getType() != MINU) goError();
	
		getWord();
		g_loop_step();

		//可能缺失的右括号
		tryWord(1);
		if (tryword.getType() != RPARENT) Error::parentError(getRow());
		else getWord();

		getWord();
		g_statement(astnode_just_temporary);
	}
	else {
		goError();
	}

	Output::printGrammar("<循环语句>");
}

//语句列
void GrammarAnalyser::g_statement_list(ASTNode* &astnode_statement_list){
	ASTNode* astnode_statement;
	vector<ASTNode*> vec_statements;
	tryWord(1);
	if (tryword.getType() != RBRACE) {
		getWord();

		while (true) {
			g_statement(astnode_statement);
			vec_statements.push_back(astnode_statement);

			tryWord(1);
			if (tryword.getType() == RBRACE) {
				break;
			}
			else {
				getWord();
			}
		}
	}
	astnode_statement_list = factory->makeASTNodeStamentList(vec_statements);
	Output::printGrammar("<语句列>");
}

//读语句
void GrammarAnalyser::g_scanf_statement(ASTNode* &astnode_sacnf) {
	ASTNode* astnode_var;

	if (word.getType() != SCANFTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	getWord();
	g_call_iden(astnode_var);
	if (signTable.getSignCategory(word.getSmallword()) == C_CONST) Error::changeConstError(getRow());
	//if (word.getType() != IDENFR) goError();

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	astnode_sacnf = factory->makeASTNodeScanf(astnode_var);

	Output::printGrammar("<读语句>");
}

//写语句
void GrammarAnalyser::g_printf_statement(ASTNode* &asnnode_print) {
	ASTNode* astnode_expression;
	char str[MAX_WORD_LEN];

	int type;

	if (word.getType() != PRINTFTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	getWord();
	if (word.getType() == STRCON) {
		g_string(str);

		tryWord(1);
		if (tryword.getType() == COMMA) {
			getWord();

			getWord();
			g_expression(astnode_expression,type);
			asnnode_print = factory->makeASTNodePrint(str,astnode_expression);
		}
		else {
			asnnode_print = factory->makeASTNodePrint(str);
		}
	}
	else {
		g_expression(astnode_expression,type);
		asnnode_print = factory->makeASTNodePrint(astnode_expression);
	}

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	

	Output::printGrammar("<写语句>");

}

//返回语句
void GrammarAnalyser::g_return_statement() {
	int type;

	if (word.getType() != RETURNTK) goError();

	tryWord(1);
	if (tryword.getType() == LPARENT) {
		getWord();

		tryWord(1);
		if (tryword.getType() == RPARENT) {
			getWord();
			signTable.checkReturn(100, getRow());
		}
		else if (tryword.getType() == SEMICN) {
			//缺失右括号
			signTable.checkReturn(100, getRow());
		}
		else {
			getWord();
			g_expression(type);

			signTable.checkReturn(type, getRow());

			//可能缺失的右括号
			tryWord(1);
			if (tryword.getType() != RPARENT) Error::parentError(getRow());
			else getWord();
		}
	}
	else {
		signTable.checkReturn(VOIDTK,getRow());
	}

	//fout << "<返回语句>" << endl;
	Output::printGrammar("<返回语句>");
}

//情况语句
void GrammarAnalyser::g_switch_statement() {
	int type;

	if (word.getType() != SWITCHTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	getWord();
	g_expression(type);

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	if (word.getType() != LBRACE) goError();

	getWord();
	g_switch_table(type);

	//可能缺失的default
	getWord();
	if (word.getType() != DEFAULTTK) Error::missDefaultError(getRow());
	else {
		g_default();
		getWord();
	}
	
	if (word.getType() != RBRACE) goError();

	//Output::printGrammar("<情况语句>" << endl;
	Output::printGrammar("<情况语句>");
}

//情况表
void GrammarAnalyser::g_switch_table(int type) {
	g_switch_sub_statement(type);

	while (true) {
		tryWord(1);
		if (tryword.getType() != CASETK) {
			break;
		}

		getWord();
		g_switch_sub_statement(type);
	}

	//fout << "<情况表>" << endl;
	Output::printGrammar("<情况表>");
}

//情况子语句
void GrammarAnalyser::g_switch_sub_statement(int type) {
	ASTNode* astnode_just_temporary;
	int valueType;
	if (word.getType() != CASETK) goError();

	getWord();
	g_const(valueType);

	if (type != valueType) Error::constTypeError(getRow());

	getWord();
	if (word.getType() != COLON) goError();

	getWord();
	g_statement(astnode_just_temporary);

	//fout << "<情况子语句>" << endl;
	Output::printGrammar("<情况子语句>");
}

//缺省
void GrammarAnalyser::g_default() {
	ASTNode* astnode_just_temporary;

	if (word.getType() != DEFAULTTK) goError();

	getWord();
	if (word.getType() != COLON) goError();

	getWord();
	g_statement(astnode_just_temporary);

	Output::printGrammar("<缺省>");
}

//值参数表
void GrammarAnalyser::g_value_table(int &paralen,int paratype[]) {
	int len = 0;
	int type;
	tryWord(1);
	if (tryword.getType() == SEMICN) {
		Error::parentError(getRow());
	}else if (tryword.getType() != RPARENT) {
		getWord();
		g_expression(type);
		paratype[len++] = type;
		while (true) {
			tryWord(1);
			if (tryword.getType() != COMMA) {
				break;
			}
			getWord();

			getWord();
			g_expression(type);

			paratype[len++] = type;
		}
	}
	paralen = len;

	//fout << "<值参数表>" << endl;
	Output::printGrammar("<值参数表>");
}

//有返回值函数调用语句
void GrammarAnalyser::g_func_ret_call() {
	char name[MAX_WORD_LEN];
	int paralen;
	int paratype[50];
	//if (word.getType() != IDENFR) goError();
	g_call_iden();
	strcpy(name, word.getSmallword());
	
	if (!hasReturnValue(word.getWord())) goError();
	
	getWord();
	if (word.getType() != LPARENT) goError();

	g_value_table(paralen,paratype);

	//符号表检查参数
	signTable.checkPara(name, paralen, paratype,getRow());

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	//fout << "<有返回值函数调用语句>" << endl;
	Output::printGrammar("<有返回值函数调用语句>");
}

//无返回值函数调用
void GrammarAnalyser::g_func_no_ret_call() {
	char name[MAX_WORD_LEN];
	int paralen;
	int paratype[50];
	//if (word.getType() != IDENFR) goError();
	g_call_iden();
	strcpy(name, word.getSmallword());

	if (hasReturnValue(word.getWord())) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	g_value_table(paralen, paratype);

	//符号表检查参数
	signTable.checkPara(name, paralen, paratype, getRow());

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	//fout << "<无返回值函数调用语句>" << endl;
	Output::printGrammar("<无返回值函数调用语句>");
}