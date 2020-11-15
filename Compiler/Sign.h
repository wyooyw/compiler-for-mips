#pragma once
#include <string.h>
#include "Word.h"
enum CATEGORY {
	C_FUNCTION, C_VAR, C_CONST, C_PARA
};
class Sign {
public:
	void setName(char* n);
	char* getName();
	void setLevel(int l);
	int getLevel();
	void setCategory(int l);
	int getCategory();
	void setType(int l);
	int getType();
	void setDimen(int d);
	int getDimen();
	void setF_para_len(int l);
	int getF_para_len();
	void setF_para_type(int l,int i);
	int getF_para_type(int i);
	void setF_return(int r);
	int getF_return();

	int getOffset();
	void setOffset(int para_offset);

	int getInitValue();
	void setInitValue(int para_initvalue);
private:
	char name[MAX_WORD_LEN];	//名称
	int level;		//层级
	int category;	//类别	函数/变量/常量....
	int type;		//类别  void/int/...
	int dimen;		//维度
	int f_para_len;	//参数个数（<100）
	int f_para_type[100];//参数类别	1代表整数，2代表字符
	int f_return = 0;	//返回值类型


	/*
	若该变量为全局变量，则为相对gp指向地址的偏移量；
	若为局部变量，则为相对sp指向地址的偏移量
	*/
	int offset;

	/*
		初值
		若为0，则不需要初始化
		若不为0，则需要初始化
	*/

	int initValue = 0;
};