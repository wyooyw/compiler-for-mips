#include "GrammarAnalyser.h"
using namespace std;

/*
	GrammarAnalyserVarDeclare.cpp
	包含<变量说明>、<变量定义>、<变量定义及初始化>、<变量定义无初始化>
	<二维数组初始化>、<一维数组初始化>
*/

//变量说明
void GrammarAnalyser::g_var_declare() {
	g_var_def();

	//可能缺失的分号
	tryWord(1);
	if (tryword.getType() != SEMICN) Error::semiError(getRow());
	else getWord();
	while (true) {
		if (!tryWord(1)) {
			break;
		}
		if (tryWord(1) && tryword.getType() != CHARTK
						&& tryword.getType() != INTTK) {
			break;
		}
		if (tryWord(3) && tryword.getType() == LPARENT) {
			break;
		}
		getWord();
		g_var_def();

		//可能缺失的分号
		tryWord(1);
		if (tryword.getType() != SEMICN) Error::semiError(getRow());
		else getWord();
	}
	Output::printGrammar("<变量说明>");
}

//变量定义
void GrammarAnalyser::g_var_def() {
	int type, dimen = 0, n = 0, m = 0;
	char name[MAX_WORD_LEN];
	g_type_iden(type);

	getWord();
	if (word.getType() != IDENFR) goError();

	//保存变量名称
	strcpy(name, word.getSmallword());

	//第一个中括号
	if (tryWord(1) && tryword.getType() == LBRACK) {
		getWord();

		dimen = 1;

		getWord();
		g_unsigned_int(n);//上次写到这里

		//可能缺失的右中括号
		tryWord(1);
		if (tryword.getType() != RBRACK)  Error::brackError(getRow());
		else getWord();


		//第二个中括号
		if (tryWord(1) && tryword.getType() == LBRACK) {
			getWord();

			dimen = 2;

			getWord();
			g_unsigned_int(m);

			//可能缺失的右中括号
			tryWord(1);
			if (tryword.getType() != RBRACK)  Error::brackError(getRow());
			else getWord();


		}

	}

	tryWord(1);
	if (tryword.getType() == ASSIGN) {
		getWord();
		g_var_def_init(type, dimen, n, m,name);
	}
	//else if (tryword.getType() == COMMA || tryword.getType() == SEMICN) {
	else{
		//填入符号表
		if (signTable->havaSignInSameLevel(name, level))  Error::reDefError(getRow());
		else {
			if (dimen == 0) {
				signTable->addVar(type, name, dimen, level);
			}
			else if (dimen == 1) {
				signTable->addVar(type, name, dimen,n,1, level);
			}
			else if(dimen==2){
				signTable->addVar(type, name, dimen, n, m, level);
			}
			else {
				printf("未知维度??\n");
			}
			
		}

		g_var_def_no_init(type,dimen);
	}

	Output::printGrammar("<变量定义>");
}

//变量定义无初始化
void GrammarAnalyser::g_var_def_no_init(int type,int k) {
	char name[MAX_WORD_LEN];
	int n, m,dimen;
	bool valid_name;
	while (true) {
		if (tryWord(1) && tryword.getType() == COMMA) {
			getWord();

			getWord();
			if (word.getType() != IDENFR) goError();
			strcpy(name,word.getSmallword());
			valid_name = false;
			dimen = 0;
			//填入符号表
			if (signTable->havaSignInSameLevel(name, level))  Error::reDefError(getRow());
			else  valid_name = true;

			//第一个中括号
			if (tryWord(1) && tryword.getType() == LBRACK) {
				dimen = 1;
				getWord();

				getWord();
				g_unsigned_int(n);

				//可能缺失的右中括号
				tryWord(1);
				if (tryword.getType() != RBRACK)  Error::brackError(getRow());
				else getWord();

				//第二个中括号
				if (tryWord(1) && tryword.getType() == LBRACK) {
					dimen = 2;
					getWord();

					getWord();
					g_unsigned_int(m);

					//可能缺失的右中括号
					tryWord(1);
					if (tryword.getType() != RBRACK)  Error::brackError(getRow());
					else getWord();
				}
			}
			if (valid_name) {
				if (dimen == 0) {
					signTable->addVar(type, name, dimen, level);
				}
				else if (dimen == 1) {
					signTable->addVar(type, name, dimen,n,1, level);
				}
				else if (dimen == 2) {
					signTable->addVar(type, name, dimen,n,m, level);
				}
				else {
					printf("未知维度？！");
				}
			}
			
			
		}
		else {
			Output::printGrammar("<变量定义无初始化>");
			break;
		}
	}
}

//变量定义及初始化
void GrammarAnalyser::g_var_def_init(int type, int dimen, int n, int m,char* name) {
	int valueType;
	int value;
	vector<int> values;
	if (dimen == 0) {
		getWord();
		g_const(valueType,value);

		//填入符号表
		if (signTable->havaSignInSameLevel(name, level))  Error::reDefError(getRow());
		else  signTable->addVar(type, name, dimen, level, value);

		if (valueType != type) Error::constTypeError(getRow());
	}
	else if (dimen == 1) {
		//填入符号表
		if (signTable->havaSignInSameLevel(name, level))  Error::reDefError(getRow());
		else {
			getWord();
			g_one_d_arr(type, n, values);
			signTable->addVar(type, name, dimen,n,1, level, values);
		}
		
		
	}
	else {
		//填入符号表
		if (signTable->havaSignInSameLevel(name, level))  Error::reDefError(getRow());
		else {
			getWord();
			g_two_d_arr(type, n, m, values);
			signTable->addVar(type, name, dimen,n,m, level, values);
		}
	}


	Output::printGrammar("<变量定义及初始化>");
}

//二维数组初始化
void GrammarAnalyser::g_two_d_arr(int type, int n, int m, vector<int> &values) {
	if (word.getType() != LBRACE) goError();
	//此维没有数据，数组初始化个数不匹配
	tryWord(1);
	if (tryword.getType() == RBRACE) {
		Error::arrayInitError(getRow());
		getWord();
		return;
	}

	getWord();
	g_one_d_arr(type, m, values);
	int count = 1;
	while (true) {
		tryWord(1);
		if (tryword.getType() != COMMA) {
			break;
		}
		getWord();

		getWord();
		g_one_d_arr(type, m, values);
		count++;
	}
	if (count != n) {
		Error::arrayInitError(getRow());
	}

	getWord();
	if (word.getType() != RBRACE) goError();
}

//一维数组初始化
void GrammarAnalyser::g_one_d_arr(int type, int n,vector<int> &values) {
	int valueType;
	int value;

	if (word.getType() != LBRACE) goError();

	//此维没有数据，数组初始化个数不匹配
	tryWord(1);
	if (tryword.getType() == RBRACE) {
		Error::arrayInitError(getRow());
		getWord();
		return;
	}

	getWord();
	g_const(valueType, value);
	values.push_back(value);
	int count = 1;
	while (true) {
		tryWord(1);
		if (tryword.getType() != COMMA) {
			break;
		}
		getWord();

		getWord();
		g_const(valueType,value);
		values.push_back(value);
		count++;
	}
	//数组初始化个数不匹配
	if (count != n) {
		Error::arrayInitError(getRow());
	}

	getWord();
	if (word.getType() != RBRACE) goError();
}