#pragma once
#include "Word.h"
class Test {
public:
	static void LexicalOutput(Word &word);//第一次试验：词法分析
	static void GrammarOutput(Word& word);//第二次试验：语法分析
	static void ErrorOutput(Word& word);//第三次实验：错误处理

	/*
		则设置当前输出为第几次实验的输出
	*/
	static void setStatus(int exp);
};