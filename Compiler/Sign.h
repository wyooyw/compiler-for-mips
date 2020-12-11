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

	int getInitValue(int n);			//һά����
	int getInitValue(int n, int m);		//��ά����
	void setInitValue(vector<int> arr);
	bool hasInitValue();
private:
	int id = 0;
	char name[MAX_WORD_LEN];	//����
	int level = 0;		//�㼶
	int category = 0;	//���	����/����/����....
	int type = 0;		//���  void/int/...
	int dimen = 0;		//ά��
	int dimen_n = 0;	//��һά�ĳ���
	int dimen_m = 0;	//�ڶ�ά�ĳ���
	int f_para_len = 0;	//����������<100��
	int f_para_type[100];//�������	1����������2�����ַ�
	int f_return = 0;	//����ֵ����


	/*
	���ñ���Ϊȫ�ֱ�������Ϊ���gpָ���ַ��ƫ������
	��Ϊ�ֲ���������Ϊ���spָ���ַ��ƫ����
	*/
	int offset = 0;
	int base = 0;		//Ϊ$sp��$gp

	/*
		��ֵ
		��Ϊ0������Ҫ��ʼ��
		����Ϊ0������Ҫ��ʼ��
	*/

	vector<int> initValue = {0};

	bool has_init_value = false;
};