#include <string>
#include <vector>
using namespace std;
#pragma once
enum ASTNodeType {
	ASTNodeType_Int, ASTNodeType_Char, 
	ASTNodeType_Var,
	ASTNodeType_Factor,
	ASTNodeType_Assign,
	ASTNodeType_Scanf,
	ASTNodeType_Print,
	ASTNodeType_PrintString,
	ASTNodeType_PrintExpression
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
class ASTNode {
public:
	ASTNode(int childlen);
	ASTNode();

	int getType();
	void setType(int type);

	ASTNode* getChild(int index);
	void setChild(int index, ASTNode* child);

	string getValueStr();
	void setValueStr(char* n);

	int getValue();
	void setValue(int value);

	void print();
private:
	int type;
	ASTNode* *child;

	int value;
	string value_s;
};