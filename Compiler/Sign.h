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
private:
	char name[MAX_WORD_LEN];	//����
	int level;		//�㼶
	int category;	//���	����/����/����....
	int type;		//���  void/int/...
	int dimen;		//ά��
	int f_para_len;	//����������<100��
	int f_para_type[100];//�������	1����������2�����ַ�
	int f_return = 0;	//����ֵ����
	
};