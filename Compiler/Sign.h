#pragma once
#include <string.h>
#include <vector>
#include "Word.h"
using namespace std;
#define MAX_ARRAY_LEN 2000
enum CATEGORY {
	C_FUNCTION, C_VAR, C_CONST, C_PARA
};
class Sign {
public:
	int getId();
	void setId(int i);

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
	void setDimenN(int d);
	int getDimenN();
	void setDimenM(int d);
	int getDimenM();
	void setF_para_len(int l);
	int getF_para_len();
	void setF_para_type(int l,int i);
	int getF_para_type(int i);
	void setF_return(int r);
	int getF_return();

	int getOffset();
	void setOffset(int para_offset);
	int getBase();
	void setBase(int para_base);

	int getInitValue();
	void setInitValue(int para_initvalue);

	int getInitValue(int n);			//一维数组
	int getInitValue(int n, int m);		//二维数组
	void setInitValue(vector<int> arr);
	bool hasInitValue();
private:
	int id = 0;
	char name[MAX_WORD_LEN];	//名称
	int level = 0;		//层级
	int category = 0;	//类别	函数/变量/常量....
	int type = 0;		//类别  void/int/...
	int dimen = 0;		//维度
	int dimen_n = 0;	//第一维的长度
	int dimen_m = 0;	//第二维的长度
	int f_para_len = 0;	//参数个数（<100）
	int f_para_type[100];//参数类别	1代表整数，2代表字符
	int f_return = 0;	//返回值类型


	/*
	若该变量为全局变量，则为相对gp指向地址的偏移量；
	若为局部变量，则为相对sp指向地址的偏移量
	*/
	int offset = 0;
	int base = 0;		//为$sp或$gp

	/*
		初值
		若为0，则不需要初始化
		若不为0，则需要初始化
	*/

	vector<int> initValue = {0};

	bool has_init_value = false;
};