#include "ASTNode.h"
#include "Word.h"
#pragma once
class ASTNodeFactory {
public:
	
	ASTNode* makeASTNodeInt(int value);
	ASTNode* makeASTNodeChar(char c);
	ASTNode* makeASTNodeVar(char* name);

	ASTNode* makeASTNodeFactor(int sign, ASTNode* left, ASTNode* right);

	ASTNode* makeASTNodeAssign(ASTNode* left, ASTNode* right);

	ASTNode* makeASTNodePrint(ASTNode* expression,int type);
	ASTNode* makeASTNodePrint(char* str);
	ASTNode* makeASTNodePrint(char* str, ASTNode* expression,int type);
	ASTNode* makeASTNodeScanf(ASTNode* var);

	ASTNode* makeASTNodeCondition(int sign, ASTNode* left, ASTNode* right);
	ASTNode* makeASTNodeIf(ASTNode* condition, ASTNode* stmt);
	ASTNode* makeASTNodeIfelse(ASTNode* condition, ASTNode* stmt, ASTNode* elsestmt);
	ASTNode* makeASTNodeWhile(ASTNode* condition,ASTNode* stmt);
	ASTNode* makeASTNodeSwitch(ASTNode* expression, ASTNode* caselist, ASTNode* astnode_default);
	ASTNode* makeASTNodeCaselist(vector<ASTNode*> cases);
	ASTNode* makeASTNodeCase(int constvalue, ASTNode* stmt);
	ASTNode* makeASTNodeFor(ASTNode* init, ASTNode* cond, ASTNode* update, ASTNode* stmt);


	ASTNode* makeASTNodeStamentList(vector<ASTNode*> vec_statements);

	ASTNode* makeASTNodeMain(ASTNode* stmt_list);

	ASTNode* makeASTNodeProgram(vector<ASTNode*> functions);

	//函数
	ASTNode* makeASTNodeFunc(char *name,ASTNode* paraListm, ASTNode* stmtList);
	ASTNode* makeASTNodeParaList(vector<ASTNode*> paras);
	ASTNode* makeASTNodePara(char* name);

	//返回语句
	ASTNode* makeASTNodeReturn(ASTNode* expression);
	ASTNode* makeASTNodeReturn();

	//函数调用
	ASTNode* makeASTNodeCall(char* name, ASTNode* valuelist);
	ASTNode* makeASTNodeValueList(vector<ASTNode*> expressions);

	//一维数组和二维数组
	ASTNode* makeASTNodeArr(char* name,ASTNode* expression);
	ASTNode* makeASTNodeArr2(char* name, ASTNode* expression1, ASTNode* expression2);

	ASTNode* makeASTNodeNull();
};
