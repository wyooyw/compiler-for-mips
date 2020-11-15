#include "ASTNode.h"
#include "SignTable.h"
#include "MidCode.h"
#include "MidCodeUnitFactory.h"

#pragma once

class CodeGenerator {
public:
	CodeGenerator(SignTable* signTable);
	MidCode* genProgram(ASTNode* astnode_program);
private:
	MidCode* genMain(ASTNode* astnode_main);
	MidCode* genStmtList(ASTNode* astnode_stmtlist);

	MidCode* genAssign(ASTNode* factor);
	MidCode* genFactor(ASTNode* factor, Sign* rst);
	
	ASTNode* astnode_program;
	SignTable* signTable;
	MidCodeUnitFactory* factory = new MidCodeUnitFactory();
};