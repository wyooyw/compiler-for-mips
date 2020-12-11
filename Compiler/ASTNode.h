#include <string>
#include <string.h>
#include <vector>
#include "Word.h"
using namespace std;
#pragma once
enum ASTNodeType {
	ASTNodeType_Int, ASTNodeType_Char, 
	ASTNodeType_Str,
	ASTNodeType_Var,
	ASTNodeType_Factor,
	ASTNodeType_Assign,
	ASTNodeType_Scanf,
	ASTNodeType_Print,
	ASTNodeType_PrintString,
	ASTNodeType_PrintExpression,
	ASTNodeType_StmtList,
	ASTNodeType_Main,
	ASTNodeType_Program,
	//控制
	ASTNodeType_If,
	ASTNodeType_Ifelse,
	ASTNodeType_While,
	ASTNodeType_Condition,
	ASTNodeType_Switch,
	ASTNodeType_Caselist,
	ASTNodeType_Case,
	ASTNodeType_For,

	//函数
	ASTNodeType_Func,
	ASTNodeType_ParaList,
	ASTNodeType_Para,

	//返回语句
	ASTNodeType_Return,

	//函数调用
	ASTNodeType_Call,
	ASTNodeType_ValueList,

	ASTNodeType_Arr,//数组变量
	ASTNodeType_Arr2,//二维数组变量

	ASTNodeType_Null//空语句
};
enum ASTNode_Factor {
	ASTNode_Factor_Left, ASTNode_Factor_Right
}; 
enum ASTNode_Assign {
	ASTNode_Assign_Left, ASTNode_Assign_Right
};
enum ASTNode_Print {
	ASTNode_Print_String,
	ASTNode_Print_Expression
};
enum ASTNode_Scanf {
	ASTNode_Scanf_Var
};
enum ASTNode_Main {
	ASTNodeMain_Stmtlist
};
enum ASTNode_If {
	ASTNode_If_Cond,
	ASTNode_If_Stmt
};
enum ASTNode_Ifelse {
	ASTNode_Ifelse_Cond,
	ASTNode_Ifelse_Stmt,
	ASTNode_Ifelse_ElseStmt
};
enum ASTNode_While {
	ASTNode_While_Cond,
	ASTNode_While_Stmt
};
enum ASTNode_Condition {	//value为比较符号
	ASTNode_Condition_Left,
	ASTNode_Condition_Right
};
enum ASTNode_Switch {
	ASTNode_Switch_Expression,
	ASTNode_Switch_Caselist,
	ASTNode_Switch_Default
};
enum ASTNode_Case {
	ASTNode_Case_Stmt
};
enum ASTNode_For {
	ASTNode_For_Init,
	ASTNode_For_Cond,
	ASTNode_For_Update,
	ASTNode_For_Stmt
};
enum ASTNode_Func {
	ASTNode_Func_ParaList,
	ASTNode_Func_StmtList
};
enum ASTNode_Return {
	ASTNode_Return_Expression
};
enum ASTNode_Call {
	ASTNode_Call_ValueList
};
enum ASTNode_Arr {
	ASTNode_Arr_Express
};
enum ASTNode_Arr2 {
	ASTNode_Arr2_Express1,
	ASTNode_Arr2_Express2
};
class ASTNode {
public:
	ASTNode(int childlen);
	ASTNode();

	int getType();
	void setType(int type);

	ASTNode* getChild(int index);
	void setChild(int index, ASTNode* child);

	char* getValueStr();
	void setValueStr(char* n);

	int getValue();
	void setValue(int value);

	void print();

	
private:
	int type;
	ASTNode* *child;

	//ASTNode** point2me;			//父节点指向自己的指针的指针

	int value;
	char value_s[MAX_WORD_LEN];
};