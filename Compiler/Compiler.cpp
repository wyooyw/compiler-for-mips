#include "LexicalAnalyser.h"
#include "GrammarAnalyser.h"
#include "Output.h"

#define _CRT_SECURE_NO_WARNINGS
using namespace std;
const string IN_PATH = "./testfile.txt";


int main(int argc, char* argv[]) {
	if (argc == 2 && argv[1][0] - '0'<=2) {
		//语法分析 or 词法分析 or 错误处理
		Output::setExp(argv[1][0]-'0');
	}
	else {
		//错误处理
		Output::setExp(0);
	}
	//Output::setExp(2);
	LexicalAnalyser lexicalAnalyser;//语法分析器
	SignTable signTable;//符号表
	GrammarAnalyser grammarAnalyser(lexicalAnalyser, signTable);//语法分析器
	ASTNode* program;//抽象语法树 树根

	grammarAnalyser.begin(program);//语法分析（内嵌词法分析），提取抽象语法树
	
	//program->print();

	Output::close();
	return 0;
}