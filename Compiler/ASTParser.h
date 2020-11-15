#include "ASTNode.h"
#include "CodeGenerator.h"
#pragma once
/*
	��ASTת��Ϊ�м����
*/
class ASTParser {
public:
	void parseProgram();
private:
	CodeGenerator *generator = new CodeGenerator();

	void parseFactor(ASTNode* factor);

};