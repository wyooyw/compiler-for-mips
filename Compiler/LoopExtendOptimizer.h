#pragma once
#include "ASTNode.h"
#include"SignTable.h"
#include <map>
using namespace std;
class LoopExtendOptimizer {
public:
	LoopExtendOptimizer(SignTable* signTable);
	void optimize(ASTNode* program);
	
private:
	map<string, int> record;
	SignTable* signTable;
	void initRecord();
	int safe;
	bool getDefineValue(char* name, int& value);
	void var2int(ASTNode* factor);
	void checkAssign(ASTNode* assign);
	void checkWhile(ASTNode* assign);
	bool checkWhileCond(ASTNode* assign, char*& name, int& cmp, int& value);
	bool checkWhileStmt(ASTNode* assign, char* varname, int& step);
	bool checkWhileStmtAssignRight(ASTNode* right, char* varname,int &step);

	int checkWhileStmt_findUpdate;
	void replace(ASTNode* astnode_old,ASTNode* astnode_new);
};