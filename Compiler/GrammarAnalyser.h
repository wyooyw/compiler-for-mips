#include "LexicalAnalyser.h"
#include "Word.h"
#include "Output.h"
#include "Error.h"
#include "SignTable.h"
#include "ASTNodeFactory.h"


#ifndef GRAMMAR_ANALYSER_H
#define GRAMMAR_ANALYSER_H
class GrammarAnalyser {
public:
	GrammarAnalyser(LexicalAnalyser &lexAnalyser,SignTable&signTable);
	void begin();
private:
	LexicalAnalyser lexicalAnalyser;
	SignTable signTable;
	Word word; 
	Word tryword;
	bool getWord();
	bool tryWord(int count);
	void goError();
	int getRow();
	bool hasReturnValue(char *funcname);
	void setReturnValue(char* funcname);
	map<string, bool> hasReturnValueTable;
	void g_one_d_arr(int type,int n);
	void g_two_d_arr(int type,int n,int m);
	int level = 1;

	void g_plus();				//加法运算符
	void g_mul();				//乘法运算符
	void g_relation();			//关系运算符
	void g_string();			//字符串
	void g_program();			//程序
	void g_const_declare();		//常量声明
	void g_const_def();			//常量定义
	void g_unsigned_int();		//无符号整数
	void g_unsigned_int(int& a);//无符号整数(将整数复制进a中)
	void g_int();				//整数
	void g_int(int& value);
	void g_declare_head(bool hasReturn, int& type, char name[]);		//声明头部
	void g_const(int &type);				//常量
	void g_var_declare();		//变量声明
	void g_var_def();			//变量定义
	void g_var_def_no_init(int type, int dimen);	//变量定义无初始化
	void g_var_def_init(int type, int d, int n, int m);	//变量定义及初始化
	void g_type_iden(int& type);			//类型标识符

	void g_para_table(int &paralen,int paratype[]);		//参数表
	void g_func_ret_def();		//有返回值函数定义
	void g_func_no_ret_def();	//无返回值函数定义
	void g_main();				//主函数
	void g_combine_statement();	//复合语句

	void g_expression(int &type);		//表达式
	void g_term(ASTNode* &term,int &type);				//项
	void g_factor(ASTNode* &factor, int &type);			//因子

	void g_statement();			//语句
	void g_assign_statement();	//赋值语句
	void g_cond_statement();	//条件语句
	void g_cond();				//条件
	void g_loop_statement();	//循环语句
	void g_loop_step();			//步长

	void g_switch_statement();	//情况语句
	void g_switch_table(int type);		//情况表
	void g_switch_sub_statement(int type);//情况子语句
	void g_default();			//缺省

	void g_statement_list();	//语句列

	void g_scanf_statement();	//读语句
	void g_printf_statement();	//写语句
	void g_return_statement();	//返回语句

	void g_value_table(int &paralen,int paratype[]);		//值参数表
	void g_func_ret_call();		//有返回值函数调用
	void g_func_no_ret_call();	//无返回值函数调用

	void g_call_iden();			//引用标识符
	void g_new_iden();			//新建标识符
};

#endif