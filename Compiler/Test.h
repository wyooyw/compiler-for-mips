#pragma once
#include "Word.h"
class Test {
public:
	static void LexicalOutput(Word &word);//��һ�����飺�ʷ�����
	static void GrammarOutput(Word& word);//�ڶ������飺�﷨����
	static void ErrorOutput(Word& word);//������ʵ�飺������

	/*
		�����õ�ǰ���Ϊ�ڼ���ʵ������
	*/
	static void setStatus(int exp);
};