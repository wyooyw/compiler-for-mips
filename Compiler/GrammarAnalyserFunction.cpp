#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserFunction.cpp
	包含<有返回值函数定义>、<无返回值函数定义>、<参数表>
*/

//参数表 paralen:参数个数	paratype：参数类型数组
void GrammarAnalyser::g_para_table(int &paralen,int paratype[]) {
	int len = 0;
	int type;
	char name[MAX_WORD_LEN];
	tryWord(1);
	//有可能消失的右小括号(无参数时)
	if (tryword.getType() == LBRACE) {
		Error::parentError(getRow());
	}
	if (tryword.getType() != RPARENT) {
		
		getWord();
		g_type_iden(type);

		//更新综合属性
		paratype[len++] = type;

		getWord();
		if (word.getType() != IDENFR) goError();
		strcpy(name, word.getSmallword());

		//向符号表中添加参数
		if (signTable.havaSignInSameLevel(name, level)) Error::reDefError(getRow());
		else signTable.addPara(type, name, level);

		while (true) {
			tryWord(1);

			if (tryword.getType() != COMMA) {
				break;
			}
			getWord();

			getWord();
			g_type_iden(type);

			//更新综合属性
			paratype[len++] = type;

			getWord();
			if (word.getType() != IDENFR) goError();
			strcpy(name, word.getSmallword());

			//向符号表中添加参数
			if (signTable.havaSignInSameLevel(name, level)) Error::reDefError(getRow());
			else signTable.addPara(type, name, level);
		}
	}
	paralen = len;
	Output::printGrammar("<参数表>");

}

//有返回值函数定义
void GrammarAnalyser::g_func_ret_def() {
	char name[MAX_WORD_LEN];
	int type;
	int paralen;
	int paratype[101];
	bool writeInSignTableSuccess = false;
	g_declare_head(true,type,name);

	//向符号表中添加函数
	if (signTable.havaSignInSameLevel(name, level)) Error::reDefError(getRow());
	else {
		signTable.addFunc(type, name, level);
		writeInSignTableSuccess = true;
	}

	//进入函数，层级加一
	level++;

	getWord();
	if (word.getType() != LPARENT) goError();

	g_para_table(paralen,paratype);

	//向符号表中添加函数
	if (writeInSignTableSuccess) signTable.refillFunc(name, level - 1, paralen, paratype);

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	if (word.getType() != LBRACE) goError();

	g_combine_statement();

	//从符号表中去除本函数的符号
	signTable.popTopLevel(level);

	getWord();
	if (word.getType() != RBRACE) goError();

	int retType = signTable.getSignReturn(name);
	printf("\n------%d------\n", retType);
	if (retType == 0)  Error::returnError(getRow());

	//退出函数，层级减一
	level--;

	Output::printGrammar("<有返回值函数定义>");
}

//无返回值函数定义
void GrammarAnalyser::g_func_no_ret_def() {
	char name[MAX_WORD_LEN];
	int type = VOIDTK;
	int paralen;
	int paratype[101];
	bool writeInSignTableSuccess = false;

	if (word.getType() != VOIDTK) goError();

	getWord();
	if (word.getType() != IDENFR) goError();
	strcpy(name,word.getSmallword());

	getWord();
	if (word.getType() != LPARENT) goError();

	//向符号表中添加函数
	if (signTable.havaSignInSameLevel(name, level)) Error::reDefError(getRow());
	else {
		signTable.addFunc(type, name, level);
		writeInSignTableSuccess = true;
	}

	//进入函数，层级加一
	level++;

	g_para_table(paralen,paratype);
	
	//向符号表回填函数的参数信息
	if (writeInSignTableSuccess) signTable.refillFunc(name, level - 1, paralen, paratype);

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	if (word.getType() != LBRACE) goError();

	g_combine_statement();

	//从符号表中去除本函数的符号
	signTable.popTopLevel(level);

	level--;

	getWord();
	if (word.getType() != RBRACE) goError();

	Output::printGrammar("<无返回值函数定义>");
}

//主函数
void GrammarAnalyser::g_main(ASTNode*& astnode_main) {
	ASTNode* astnode_stmt_list;
	char name[MAX_WORD_LEN];
	strcpy(name, "main");
	int type = VOIDTK;

	if (word.getType() != VOIDTK) goError();

	getWord();
	if (word.getType() != MAINTK) goError();

	getWord();
	if (word.getType() != LPARENT) goError();

	//可能缺失的右括号
	tryWord(1);
	if (tryword.getType() != RPARENT) Error::parentError(getRow());
	else getWord();

	getWord();
	if (word.getType() != LBRACE) goError();

	//向符号表中添加函数
	if (signTable.havaSignInSameLevel(name, level)) Error::reDefError(getRow());
	else {
		signTable.addFunc(type, name, level);
	}

	//进入函数，层级加一
	level++;

	g_combine_statement(astnode_stmt_list);

	astnode_main = factory->makeASTNodeMain(astnode_stmt_list);

	//从符号表中去除本函数的符号
	signTable.popTopLevel(level);

	level--;

	getWord();
	if (word.getType() != RBRACE) goError();

	Output::printGrammar("<主函数>");
}

//复合语句
void GrammarAnalyser::g_combine_statement(ASTNode* &astnode_statement_list) {
	tryWord(1);
	if (tryword.getType() == CONSTTK) {
		getWord();
		g_const_declare();
		
		tryWord(1);
		if (tryword.getType() == CHARTK || tryword.getType() == INTTK) {
			getWord();
			g_var_declare();
			tryWord(1);
		}
	}else if (tryword.getType() == CHARTK || tryword.getType() == INTTK) {
		
		getWord();
		g_var_declare();
		tryWord(1);
	}
	g_statement_list(astnode_statement_list);
	
		
	Output::printGrammar("<复合语句>");
}

//复合语句（old）
void GrammarAnalyser::g_combine_statement() {
	ASTNode* astnode_statement_list;
	tryWord(1);
	if (tryword.getType() == CONSTTK) {
		getWord();
		g_const_declare();

		tryWord(1);
		if (tryword.getType() == CHARTK || tryword.getType() == INTTK) {
			getWord();
			g_var_declare();
			tryWord(1);
		}
	}
	else if (tryword.getType() == CHARTK || tryword.getType() == INTTK) {

		getWord();
		g_var_declare();
		tryWord(1);
	}
	g_statement_list(astnode_statement_list);


	Output::printGrammar("<复合语句>");
}