#include "Word.h"
using namespace std;
#define MAX_WORD_LEN 200
char* Word::getSmallword() {
	return smallword;
}
char* Word::getWord() {
	return word;
}
void Word::setWord(char* str) {
	strcpy(word, str);
	int i;
	for (i = 0; str[i];i++) {
		word[i] = str[i];
		smallword[i] = (str[i] >= 'A' && str[i] <= 'Z')?(str[i] + 'a' - 'A') : str[i];
	}
	word[i] = '\0';
	smallword[i] = '\0';
}
WORD_TYPE Word::getType() {
	return type;
}
void Word::setType(const WORD_TYPE t) {
	type = t;
}
Error Word::getError() {
	return error;
}
void Word::setError(Error err) {
	error = err;
}
