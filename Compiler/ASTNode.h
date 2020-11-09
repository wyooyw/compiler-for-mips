#include <string>
#include <vector>
using namespace std;
#pragma once
enum ASTNodeType {
	ASTNodeType_Int, ASTNodeType_Char, 
	ASTNodeType_Var,
	ASTNodeType_Factor, ASTNodeType_Term, ASTNodeType_Expression
};
enum ASTNode_Factor {
	ASTNode_Factor_Left, ASTNode_Factor_Right
};
class ASTNode {
public:
	int getType();
	void setType(int type);

	ASTNode* getChild(int index);
	void setChild(int index, ASTNode* child);

	string getName();
	void setName(char* n);

	int getValue();
	void setValue(int value);

	void print();
private:
	int type;
	vector<ASTNode*> child;

	int value;
	string name;
};