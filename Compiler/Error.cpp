#include "Error.h"
int errorUnique[10001];
	bool Error::isError() {
		return error;
	}
	void Error::setError(bool e) {
		error = e;
	}
	int Error::getRow() {
		return row;
	}
	void Error::setRow(int r) {
		row = r;
	}
	char Error::getType() {
		return type;
	}
	void Error::setType(char t) {
		type = t;
	}
	void Error::wordError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('a');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::semiError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('k');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::parentError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('l');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::brackError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('m');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::missDefaultError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('p');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::arrayInitError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('n');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::reDefError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('b');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::noDefError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('c');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::paraLenError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('d');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::paraTypeError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('e');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::condTypeError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('f');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::indexTypeError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('i');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::constTypeError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('o');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::changeConstError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('j');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::returnError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('h');
		Output::printError(error);
		errorUnique[row] = 1;
	}
	void Error::haveReturnError(int row) {
		if (errorUnique[row] == 1) return;
		Error error;
		error.setRow(row);
		error.setType('g');
		Output::printError(error);
		errorUnique[row] = 1;
	}