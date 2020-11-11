#include "GrammarAnalyser.h"
using namespace std;
GrammarAnalyser::GrammarAnalyser(LexicalAnalyser& lexAna , SignTable &signTbl) {
	lexicalAnalyser = lexAna;
	signTable = signTbl;
	factory = new ASTNodeFactory();
}
bool GrammarAnalyser::getWord() {
	return lexicalAnalyser.readWord(word);
}
bool GrammarAnalyser::tryWord(int count) {
	return lexicalAnalyser.tryReadWord(tryword,count);
}
void GrammarAnalyser::goError() {
	Output::printGrammar("Error");
}

int GrammarAnalyser::getRow() {
	return lexicalAnalyser.getRow();
}
void GrammarAnalyser::g_plus() {
	if (word.getType() != PLUS && word.getType() != MINU) {
		goError();
	}
}
void GrammarAnalyser::g_mul() {
	if (word.getType() != MULT && word.getType() != DIV) {
		goError();
	}
}

//关系运算符
void GrammarAnalyser::g_relation() {
	if (word.getType() != LSS && word.getType() != LEQ
		&& word.getType() != GRE && word.getType() != EQL
		&& word.getType() != GEQ && word.getType() != NEQ) {
		goError();
	}
}

//字符串(old)
void GrammarAnalyser::g_string() {
	if (word.getType() != STRCON) {
		goError();
	}
	Output::printGrammar("<字符串>");
}

//字符串
void GrammarAnalyser::g_string(char* str) {
	if (word.getType() != STRCON) {
		goError();
	}
	strcpy(str,word.getWord());
	Output::printGrammar("<字符串>");
}

//无符号整数
void GrammarAnalyser::g_unsigned_int() {
	if (word.getType() != INTCON) {
		goError();
	}
	Output::printGrammar("<无符号整数>");
}

//无符号整数
void GrammarAnalyser::g_unsigned_int(int &a) {
	if (word.getType() != INTCON) {
		goError();
	}
	a = stoi(word.getWord());
	Output::printGrammar("<无符号整数>");
}

//整数
void GrammarAnalyser::g_int() {
	if (word.getType() == PLUS || word.getType() == MINU) {
		getWord();
		g_unsigned_int();
	}
	else {
		g_unsigned_int();
	}
	Output::printGrammar("<整数>");
}

//整数
void GrammarAnalyser::g_int(int &value) {
	int sign;
	if (word.getType() == PLUS || word.getType()==MINU) {
		sign = word.getType();
		getWord();
		g_unsigned_int(value);
		if (sign == MINU) {
			value = -value;
		}
	}
	else {
		g_unsigned_int(value);
	}
	Output::printGrammar("<整数>");
}

//常量
void GrammarAnalyser::g_const(int &type) {
	if (word.getType() == CHARCON) {
		type = CHARTK;
	}
	else {
		type = INTTK;
		g_int();
	}
	Output::printGrammar("<常量>");
}

//类别标识符
void GrammarAnalyser::g_type_iden(int &type) {
	if (word.getType() != INTTK && word.getType() != CHARTK) {
		goError();
	}
	type = word.getType();
}

//声明头部 type:返回值类型；name:函数名
void GrammarAnalyser::g_declare_head(bool hasReturn,int &type,char name[]) {
	if (word.getType() != INTTK && word.getType() != CHARTK) {
		goError();
	}
	type = word.getType();
	getWord();
	if (word.getType() != IDENFR) goError();

	strcpy(name,word.getSmallword());

	if (hasReturn) {
		setReturnValue(word.getWord());
	}

	Output::printGrammar("<声明头部>");
}

//程序
void GrammarAnalyser::g_program(ASTNode*& astnode_program) {
	vector<ASTNode*> astnode_functions;
	ASTNode* function;
	if (tryWord(1) && tryword.getType() == CONSTTK) {
		getWord();
		g_const_declare();
	}

	if (tryWord(3) && tryword.getType() != LPARENT) {
		getWord();
		g_var_declare();
	}
	
	while (tryWord(2) && tryword.getType() != MAINTK) {
		if (tryWord(1) && tryword.getType() == VOIDTK) {
			getWord();
			g_func_no_ret_def();
		}
		else if(tryWord(1) && 
			(tryword.getType() == CHARTK || tryword.getType() == INTTK)){
			getWord();
			g_func_ret_def();
		}
		else {
			goError();
		}
	}
	if(tryWord(2) && tryword.getType() == MAINTK) {
		getWord();
		g_main(function);
		astnode_functions.push_back(function);
	}
	else {
		goError();
	}
	astnode_program = factory->makeASTNodeProgram(astnode_functions);

	Output::printGrammar("<程序>");
}
void GrammarAnalyser::begin(ASTNode*& program) {
	
	g_program(program);
	
	/*ASTNode* combine_stmt;
	g_combine_statement(combine_stmt);

	combine_stmt->print();*/
}

bool GrammarAnalyser::hasReturnValue(char* funcname) {
	char smallword[MAX_WORD_LEN];
	int i;
	for (i = 0; funcname[i]; i++) {
		if (funcname[i] >= 'A' && funcname[i] <= 'Z') {
			smallword[i] = funcname[i] - 'A' + 'a';
		}
		else {
			smallword[i] = funcname[i];
		}
	}
	smallword[i] = '\0';
	map<string, bool>::iterator iter = hasReturnValueTable.find(smallword);
	if (iter != hasReturnValueTable.end()) {
		return true;
	}
	else {
		return false;
	}

}
void GrammarAnalyser::setReturnValue(char* funcname) {
	char smallword[MAX_WORD_LEN];
	int i;
	for (i = 0; funcname[i]; i++) {
		if (funcname[i] >= 'A' && funcname[i] <= 'Z') {
			smallword[i] = funcname[i] - 'A' + 'a';
		}
		else {
			smallword[i] = funcname[i];
		}
	}
	smallword[i] = '\0';
	hasReturnValueTable.insert(make_pair(smallword,true));
}

//引用标识符
void GrammarAnalyser:: g_call_iden() {
	if (word.getType() != IDENFR) goError();
	
	//检查调用的标识符是否为未定义的名字
	if (!signTable.havaSign(word.getSmallword())) {
		printf("cc\n");
		Error::noDefError(getRow());
	}
}

//引用标识符（old）
void GrammarAnalyser::g_call_iden(ASTNode* &node_idfr) {
	ASTNodeFactory *factory = new ASTNodeFactory();
	if (word.getType() != IDENFR) goError();
	
	//检查调用的标识符是否为未定义的名字
	if (!signTable.havaSign(word.getSmallword())) {
		printf("cc\n");
		Error::noDefError(getRow());
	}

	node_idfr = factory->makeASTNodeVar(word.getSmallword());
}