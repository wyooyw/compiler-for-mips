#include "LexicalAnalyser.h"
using namespace std;
const string IN_PATH = "./testfile.txt";
LexicalAnalyser::LexicalAnalyser() {
	init();
}

/*
若存在下一个单词，则读取，存到word对象中，返回true
若不存在下一个单词，则返回false。
*/

bool LexicalAnalyser::tryReadWord(Word& word,int a) {
	char *ori_buf_ptr = buf_ptr;
	for (int i = 1; i <= a; i++) {
		if (!nextWord(word,false)) {
			buf_ptr = ori_buf_ptr;
			return false;
		}
	}

	buf_ptr = ori_buf_ptr;
	return true;
}

//readWord调用nextWord读取一个单词后，输出
bool LexicalAnalyser::readWord(Word& word) {
	if (nextWord(word,true)) {
		//fout << WORD_TYPE_STR[word.getType()] << " "<< word.getWord()<<endl;
		Output::printWord(word);
		return true;
	}
	else {
		return false;
	}
}

//nextWord读取下一个单词，不输出
bool LexicalAnalyser::nextWord(Word& word,bool realread) {
	int errflag;
	char myword[MAX_WORD_LEN];
	char mytype[20];
	char smallword[MAX_WORD_LEN];
	while (true) {
		errflag = 0;
		passSpace(realread);
		if (*buf_ptr == 0) {
			return false;
		}
		char* word_ptr = myword;
		if (isLetter(*buf_ptr)) {
			//标识符
			*word_ptr++ = *buf_ptr++;
			while (isLetter(*buf_ptr) || isDigital(*buf_ptr)) {
				*word_ptr++ = *buf_ptr++;
			}
			*word_ptr = '\0';
				//检查是否为关键词
			int i;
			for (i = 0; myword[i]; i++) {
				if (myword[i] >= 'A' && myword[i] <= 'Z') {
					smallword[i] = myword[i] - 'A' + 'a';
				}
				else {
					smallword[i] = myword[i];
				}
			}
			smallword[i] = '\0';
			map<string, WORD_TYPE>::iterator iter = keyword_table.find(smallword);
			if (iter != keyword_table.end()) {
				word.setType(iter->second);
			}
			else {
				word.setType(IDENFR);
			}
		}
		else if (isDigital(*buf_ptr)) {
			//整数
			*word_ptr++ = *buf_ptr++;
			while (isDigital(*buf_ptr)) {
				*word_ptr++ = *buf_ptr++;
			}
			*word_ptr = '\0';
			word.setType(INTCON);
		}
		else if (*buf_ptr == '\'') {
			buf_ptr++;
			//字符
			/*do {
				*word_ptr++ = *buf_ptr++;
			} while (*buf_ptr != '\'');*/
			if (!isChar(*buf_ptr)) {
				//字符不符合规范
				Error::wordError(row);
			}
			*word_ptr++ = *buf_ptr++;
			*word_ptr = '\0';

			if (*buf_ptr != '\'') {
				//少右单引号
				Error::wordError(row);
			}
			else {
				buf_ptr++;
			}

			word.setType(CHARCON);
		}
		else if (*buf_ptr == '"') {
			buf_ptr++;
			//字符串
			int len = 0;
			while (isValidInString(*buf_ptr)) {
				*word_ptr++ = *buf_ptr++;
				len++;
			}

			//如果字符串长度为0或者遇到的第一个非字符串字符不是双引号
			if (len == 0 || *buf_ptr != '"') {
				Error::wordError(row);
			}

			//如果最后一个字符是双引号，则指针向后移动一位
			if(*buf_ptr == '"'){
				buf_ptr++;
			}
			
			*word_ptr = '\0';
			word.setType(STRCON);
		}
		else if (*buf_ptr == '<') {
			// <= 和 <
			*word_ptr++ = *buf_ptr++;
			if (*buf_ptr == '=') {
				*word_ptr++ = *buf_ptr++;
				word.setType(LEQ);
			}
			else {	
				word.setType(LSS);
			}
			*word_ptr = '\0';
		}
		else if (*buf_ptr == '>') {
			// >= 和 >
			*word_ptr++ = *buf_ptr++;
			if (*buf_ptr == '=') {
				*word_ptr++ = *buf_ptr++;
				word.setType(GEQ);
			}
			else {
				word.setType(GRE);
			}
			*word_ptr = '\0';
		}
		else if (*buf_ptr == '=') {
			// == 和 =
			*word_ptr++ = *buf_ptr++;
			if (*buf_ptr == '=') {
				*word_ptr++ = *buf_ptr++;
				word.setType(EQL);
			}
			else {
				word.setType(ASSIGN);
			}
			*word_ptr = '\0';
		}
		else if (*buf_ptr == '!') {
			// !=
			*word_ptr++ = *buf_ptr++;
			if (*buf_ptr == '=') {
				*word_ptr++ = *buf_ptr++;
				word.setType(NEQ);
			}
			else {
				Error::wordError(row);
			}
			*word_ptr = '\0';
		}
		else {
			//独立符号
			map<char, WORD_TYPE>::iterator iter = sign_table.find(*buf_ptr);
			if (iter != sign_table.end()) {
				*word_ptr++ = *buf_ptr++;
				*word_ptr++ = '\0';
				word.setType(iter->second);
			}
			else {
				//printf("%c\n", *buf_ptr);
				buf_ptr++;
				Error::wordError(row);
				errflag = 1;
			}
		}
		if (errflag==0) {
			break;
		}
	}
	word.setWord(myword);
	return true;
}

void LexicalAnalyser::readfile() {
	filebuf* pbuf;
	ifstream filestr;
	filestr.open(IN_PATH, ios::binary);
	pbuf = filestr.rdbuf();

	buffer_size = pbuf->pubseekoff(0, ios::end, ios::in);
	pbuf->pubseekpos(0, ios::in);

	buffer = (char *)malloc(sizeof(char) * (buffer_size+50));//new char[buffer_size];

	pbuf->sgetn(buffer, buffer_size);

	filestr.close();
}
void LexicalAnalyser::init() {
	//读取文件到内存
	readfile();
	buffer[buffer_size] = '\0';

	//设置指针
	buf_ptr = buffer;

	//初始化关键词表
	initKeywordTable();

	//初始化独立特殊符号表
	initSignTable();
}
void LexicalAnalyser::initKeywordTable() {
	keyword_table["const"] = CONSTTK;
	keyword_table["int"] = INTTK;
	keyword_table["char"] = CHARTK;
	keyword_table["void"] = VOIDTK;
	keyword_table["main"] = MAINTK;
	keyword_table["if"] = IFTK;
	keyword_table["else"] = ELSETK;
	keyword_table["switch"] = SWITCHTK;
	keyword_table["case"] = CASETK;
	keyword_table["default"] = DEFAULTTK;
	keyword_table["while"] = WHILETK;
	keyword_table["for"] = FORTK;
	keyword_table["scanf"] = SCANFTK;
	keyword_table["printf"] = PRINTFTK;
	keyword_table["return"] = RETURNTK;
}
void LexicalAnalyser::initSignTable() {
	sign_table['+'] = PLUS;
	sign_table['-'] = MINU;
	sign_table['*'] = MULT;
	sign_table['/'] = DIV;
	sign_table[':'] = COLON;
	sign_table[';'] = SEMICN;
	sign_table[','] = COMMA;
	sign_table['('] = LPARENT;
	sign_table[')'] = RPARENT;
	sign_table['['] = LBRACK;
	sign_table[']'] = RBRACK;
	sign_table['}'] = RBRACE;
	sign_table['{'] = LBRACE;
}
void LexicalAnalyser::passSpace(bool recordrow) {
	while (*buf_ptr > 0 && *buf_ptr <= 32) {
		if (recordrow && *buf_ptr == '\n') {
			row++;
		}
		++buf_ptr;
	}
}
bool LexicalAnalyser::isDigital(char c) {
	return c >= '0' && c <= '9';
}
bool LexicalAnalyser::isLetter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool LexicalAnalyser::isPlus(char c) {
	return c == '+' || c == '-';
}
bool LexicalAnalyser::isMul(char c) {
	return c == '*' || c == '/';
}
bool LexicalAnalyser::isChar(char c) {
	return isPlus(c) || isMul(c) || isDigital(c) || isLetter(c);
}
bool LexicalAnalyser::isValidInString(char c) {
	return c == 32 || c == 33 || (c >= 35 && c <= 126);
}

int LexicalAnalyser::getRow() {
	return row;
}