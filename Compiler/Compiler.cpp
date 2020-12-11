#include "LexicalAnalyser.h"
#include "GrammarAnalyser.h"
#include "Output.h"
#include "CodeGenerator.h"
#include "MidCode.h"
#include "TargetCode.h"

#define _CRT_SECURE_NO_WARNINGS
using namespace std;
const string IN_PATH = "./testfile.txt";


int main(int argc, char* argv[]) {
	if (argc == 2 && argv[1][0] - '0'<=2) {
		//语法分析 or 词法分析 or 错误处理
		Output::setExp(argv[1][0]-'0');
	}
	else {
		//生成MIPS
		Output::setExp(EXP4);
	}
	//Output::setExp(2);
	LexicalAnalyser lexicalAnalyser;//语法分析器
	SignTable signTable;//符号表
	GrammarAnalyser grammarAnalyser(lexicalAnalyser, &signTable);//语法分析器
	printf("a");
	ASTNode* astnode_program;//抽象语法树 树根
	grammarAnalyser.begin(astnode_program);//语法分析（内嵌词法分析），提取抽象语法树
	
	//signTable.show();

	//astnode_program->print();

	TargetCode* targetCode = new TargetCode(&signTable);
	targetCode->t_begin(astnode_program);

/*
	CodeGenerator *generator = new CodeGenerator(&signTable);//中间代码生成器
	
	MidCode* midCode;//中间代码
	generator->genProgram(astnode_program,midCode);//生成中间代码

	midCode.print();*/
	

	Output::close();
	return 0;
}