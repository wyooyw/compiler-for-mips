#include "ASTNode.h"
#include "CodeGenerator.h"
#pragma once
/*
	将AST转换为中间代码
*/
class ASTParser {
public:
	void parseProgram();
private:
	CodeGenerator *generator = new CodeGenerator();

	void parseFactor(ASTNode* factor);

};