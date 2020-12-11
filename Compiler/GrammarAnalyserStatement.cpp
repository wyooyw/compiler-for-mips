#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserStatement.cpp
	包含
*/

//语句
void GrammarAnalyser::g_statement(ASTNode* &statement) {
	if (word.getType() == WHILETK || word.getType() == FORTK) {
		g_loop_statement(statement);
	}
	else if (word.getType() == IFTK) {
		g_cond_statement(statement);
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
		g_switch_statement(statement);
	}
	else if (word.getType() == SEMICN) {
		//空语句
		//getWord();
		statement = factory->makeASTNodeNull();
	}
	else if (word.getType() == RETURNTK) {
		//返回语句
		g_return_statement(statement);

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
				g_func_ret_call(statement);
			}
			else {
				g_func_no_ret_call(statement);
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
	ASTNode* astnode_express1 = 0;
	ASTNode* astnode_express2 = 0;
	char name[MAX_WORD_LEN];

	//if (word.getType() != IDENFR) goError();
	//有引用的标识符，内含判断该标识符是否已在符号表中创建
	g_call_iden(astnode_var);
	//printf("\n---%s---\n", astnode_var->getValueStr());
	if (signTable->getSignCategory(word.getSmallword()) == C_CONST) Error::changeConstError(getRow());

	strcpy(name, word.getSmallword());

	int dimen = 0;
	int indexType;
	int valueType;
	getWord();
	if (word.getType() == LBRACK) {
		dimen = 1;

		getWord();
		g_expression(astnode_express1,indexType);
		
		//数组下标不是整型
		if (indexType != INTTK) Error::indexTypeError(getRow());

		//  ]
		tryWord(1);
		if (tryword.getType() != RBRACK)  Error::brackError(getRow()); 
		else getWord();

		getWord();
		if (word.getType() == LBRACK) {
			dimen = 2;

			getWord();
			g_expression(astnode_express2,indexType);

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

	if (dimen == 1) {
		astnode_var = factory->makeASTNodeArr(name, astnode_express1);
	}
	else if (dimen == 2) {
		astnode_var = factory->makeASTNodeArr2(name, astnode_express1,astnode_express2);
	}
	
	getWord();
	g_expression(astnode_expression,valueType);

	astnode_assign = factory->makeASTNodeAssign(astnode_var, astnode_expression);
		
	Output::printGrammar("<赋值语句>");
}
//条件(old)
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
//条件
void GrammarAnalyser::g_cond(ASTNode* &astnode_cond) {
	ASTNode* astnode_left_expression;
	ASTNode* astnode_right_expression;
	int relation;

	int valueType;
	g_expression(astnode_left_expression,valueType);

	if (valueType == CHARTK) Error::condTypeError(getRow());

	getWord();
	g_relation(relation);

	getWord();
	g_expression(astnode_right_expression,valueType);

	if (valueType == CHARTK) Error::condTypeError(getRow());

	astnode_cond = factory->makeASTNodeCondition(relation, astnode_left_expression, astnode_right_expression);

	Output::printGrammar("<条件>");
}

//条件语句
void GrammarAnalyser::g_cond_statement(ASTNode* &astnode_if) {
	ASTNode* astnode_stmt_if;
	ASTNode* astnode_stmt_else;
	ASTNode* astnode_condition;

	if (word.getType() != IFTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	getWord();
	g_cond(astnode_condition);

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	g_statement(astnode_stmt_if);

	if (tryWord(1) && tryword.getType() == ELSETK) {
		getWord();
		getWord();
		g_statement(astnode_stmt_else);
		astnode_if = factory->makeASTNodeIfelse(astnode_condition, astnode_stmt_if, astnode_stmt_else);
	}
	else {
		astnode_if = factory->makeASTNodeIf(astnode_condition, astnode_stmt_if);
	}
	

	Output::printGrammar("<条件语句>");
}

//步长(old)
void GrammarAnalyser::g_loop_step() {
	g_unsigned_int();
	Output::printGrammar("<步长>");
}

//步长
void GrammarAnalyser::g_loop_step(ASTNode* &astnode_int) {
	int value;
	g_unsigned_int(value);
	astnode_int = factory->makeASTNodeInt(value);
	Output::printGrammar("<步长>");
}

//循环语句
void GrammarAnalyser::g_loop_statement(ASTNode* &astnode_loop) {
	ASTNode* astnode_just_temporary;
	if (word.getType() == WHILETK) {
		ASTNode* astnode_condition;
		ASTNode* astnode_stmt;

		getWord();
		if (word.getType() != LPARENT) goError();

		getWord();
		g_cond(astnode_condition);

		//可能缺失的右括号
		tryWord(1);
		if (tryword.getType() != RPARENT) Error::parentError(getRow());
		else getWord();

		getWord();
		g_statement(astnode_stmt);

		astnode_loop = factory->makeASTNodeWhile(astnode_condition, astnode_stmt);
	}
	else if (word.getType() == FORTK) {
		ASTNode* astnode_init;
		ASTNode* astnode_init_left;
		ASTNode* astnode_init_right;
		ASTNode* astnode_condition;
		ASTNode* astnode_update;
		ASTNode* astnode_update_left;
		ASTNode* astnode_update_right;
		ASTNode* astnode_update_right_iden;
		ASTNode* astnode_update_right_step;
		ASTNode* astnode_stmt;
		int sign;

		int t;

		getWord();
		if (word.getType() != LPARENT) goError();

		getWord();
		//if (word.getType() != IDENFR) goError();
		g_call_iden(astnode_init_left);

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		g_expression(astnode_init_right,t);

		astnode_init = factory->makeASTNodeAssign(astnode_init_left,astnode_init_right);

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();
		

		getWord();
		g_cond(astnode_condition);

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();

		getWord();
		//if (word.getType() != IDENFR) goError();
		g_call_iden(astnode_update_left);

		getWord();
		if (word.getType() != ASSIGN) goError();

		getWord();
		g_call_iden(astnode_update_right_iden);
		//if (word.getType() != IDENFR) goError();

		getWord();
		if (word.getType() != PLUS && word.getType() != MINU) goError();
		sign = word.getType();
	
		getWord();
		g_loop_step(astnode_update_right_step);

		astnode_update_right = factory->makeASTNodeFactor(sign, astnode_update_right_iden, astnode_update_right_step);

		astnode_update = factory->makeASTNodeAssign(astnode_update_left,astnode_update_right);

		//可能缺失的右括号
		tryWord(1);
		if (tryword.getType() != RPARENT) Error::parentError(getRow());
		else getWord();

		getWord();
		g_statement(astnode_stmt);

		astnode_loop = factory->makeASTNodeFor(astnode_init, astnode_condition, astnode_update, astnode_stmt);
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
	if (signTable->getSignCategory(word.getSmallword()) == C_CONST) Error::changeConstError(getRow());
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
void GrammarAnalyser::g_return_statement(ASTNode*& astnode_return) {
	ASTNode* astnode_expression = 0;
	int type;

	if (word.getType() != RETURNTK) goError();

	tryWord(1);
	if (tryword.getType() == LPARENT) {
		getWord();

		tryWord(1);
		if (tryword.getType() == RPARENT) {
			getWord();
			signTable->checkReturn(100, getRow());
		}
		else if (tryword.getType() == SEMICN) {
			//缺失右括号
			signTable->checkReturn(100, getRow());
		}
		else {
			getWord();
			g_expression(astnode_expression,type);

			signTable->checkReturn(type, getRow());

			//可能缺失的右括号
			tryWord(1);
			if (tryword.getType() != RPARENT) Error::parentError(getRow());
			else getWord();
		}

		astnode_return = factory->makeASTNodeReturn(astnode_expression);
	}
	else {
		signTable->checkReturn(VOIDTK,getRow());

		astnode_return = factory->makeASTNodeReturn();
	}

	//fout << "<返回语句>" << endl;
	Output::printGrammar("<返回语句>");
}

//情况语句
void GrammarAnalyser::g_switch_statement(ASTNode*& asnnode_switch) {
	ASTNode* expression = 0;
	ASTNode* caselist = 0;
	ASTNode* astnode_default = 0;

	int type;

	if (word.getType() != SWITCHTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	getWord();
	g_expression(expression,type);

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	if (word.getType() != LBRACE) goError();

	getWord();
	g_switch_table(caselist,type);

	//可能缺失的default
	getWord();
	if (word.getType() != DEFAULTTK) Error::missDefaultError(getRow());
	else {
		g_default(astnode_default);
		getWord();
	}
	
	if (word.getType() != RBRACE) goError();

	asnnode_switch = factory->makeASTNodeSwitch(expression, caselist, astnode_default);

	//Output::printGrammar("<情况语句>" << endl;
	Output::printGrammar("<情况语句>");
}

//情况表
void GrammarAnalyser::g_switch_table(ASTNode*& asnnode_caselist,int type) {
	vector<ASTNode*> cases;

	ASTNode* atnode_case;
	g_switch_sub_statement(atnode_case,type);
	cases.push_back(atnode_case);

	while (true) {
		tryWord(1);
		if (tryword.getType() != CASETK) {
			break;
		}

		getWord();
		g_switch_sub_statement(atnode_case,type);
		cases.push_back(atnode_case);
	}

	asnnode_caselist = factory->makeASTNodeCaselist(cases);

	//fout << "<情况表>" << endl;
	Output::printGrammar("<情况表>");
}

//情况子语句
void GrammarAnalyser::g_switch_sub_statement(ASTNode*& asnnode_case,int type) {
	ASTNode* astnode_stmt;
	int value;
	int valueType;
	if (word.getType() != CASETK) goError();

	getWord();
	g_const(valueType,value);

	if (type != valueType) Error::constTypeError(getRow());

	getWord();
	if (word.getType() != COLON) goError();

	getWord();
	g_statement(astnode_stmt);

	asnnode_case = factory->makeASTNodeCase(value, astnode_stmt);

	//fout << "<情况子语句>" << endl;
	Output::printGrammar("<情况子语句>");
}

//缺省
void GrammarAnalyser::g_default(ASTNode*& asnnode_default) {

	if (word.getType() != DEFAULTTK) goError();

	getWord();
	if (word.getType() != COLON) goError();

	getWord();
	g_statement(asnnode_default);

	Output::printGrammar("<缺省>");
}

//值参数表
void GrammarAnalyser::g_value_table(int &paralen,int paratype[],ASTNode*& astnode_valuelist) {
	vector<ASTNode*> expressions;
	ASTNode* astnode_expre;
	int len = 0;
	int type;
	tryWord(1);
	if (tryword.getType() == SEMICN) {
		Error::parentError(getRow());
	}else if (tryword.getType() != RPARENT) {
		getWord();
		g_expression(astnode_expre,type);
		paratype[len++] = type;
		expressions.push_back(astnode_expre);

		while (true) {
			tryWord(1);
			if (tryword.getType() != COMMA) {
				break;
			}
			getWord();

			getWord();
			g_expression(astnode_expre,type);

			paratype[len++] = type;

			expressions.push_back(astnode_expre);
		}
	}
	paralen = len;

	astnode_valuelist = factory->makeASTNodeValueList(expressions);

	//fout << "<值参数表>" << endl;
	Output::printGrammar("<值参数表>");
}

//有返回值函数调用语句
void GrammarAnalyser::g_func_ret_call(ASTNode*& astnode_call) {
	ASTNode* astnode_valuelist;
	char name[MAX_WORD_LEN];
	int paralen;
	int paratype[50];
	//if (word.getType() != IDENFR) goError();
	g_call_iden();
	strcpy(name, word.getSmallword());

	if (!hasReturnValue(word.getWord())) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	g_value_table(paralen, paratype, astnode_valuelist);

	//符号表检查参数
	signTable->checkPara(name, paralen, paratype, getRow());

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	astnode_call = factory->makeASTNodeCall(name,astnode_valuelist);

	//fout << "<有返回值函数调用语句>" << endl;
	Output::printGrammar("<有返回值函数调用语句>");
}

//无返回值函数调用
void GrammarAnalyser::g_func_no_ret_call(ASTNode*& astnode_call) {
	ASTNode* astnode_valuelist;

	char name[MAX_WORD_LEN];
	int paralen;
	int paratype[50];
	//if (word.getType() != IDENFR) goError();
	g_call_iden();
	strcpy(name, word.getSmallword());

	if (hasReturnValue(word.getWord())) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	g_value_table(paralen, paratype, astnode_valuelist);

	//符号表检查参数
	signTable->checkPara(name, paralen, paratype, getRow());

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	astnode_call = factory->makeASTNodeCall(name,astnode_valuelist);

	//fout << "<无返回值函数调用语句>" << endl;
	Output::printGrammar("<无返回值函数调用语句>");
}