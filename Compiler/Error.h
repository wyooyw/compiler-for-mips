#include "Output.h"
#ifndef ERROR_H
#define ERROR_H

class Error {
public:
	bool isError();
	void setError(bool e);
	int getRow();
	void setRow(int r);
	char getType();
	void setType(char t);
	static void wordError(int line);
	static void semiError(int line);
	static void parentError(int line);
	static void brackError(int line);
	static void missDefaultError(int line);
	static void arrayInitError(int line);
	static void reDefError(int line);
	static void noDefError(int line);
	static void paraLenError(int line);
	static void paraTypeError(int line);
	static void condTypeError(int line);
	static void indexTypeError(int line);
	static void constTypeError(int line);
	static void changeConstError(int line);
	static void returnError(int line);
	static void haveReturnError(int line);
private:
	bool error = false;
	int row;
	char type;
};

#endif