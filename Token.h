#pragma once
#include <string>
#include "Globals.h"

class Token {
public:
	Token(TOKEN_NAME newName, std::string newString, int newLineNum);
	std::string Print() const;
	TOKEN_NAME GetName() const;
private:
	TOKEN_NAME name;
	std::string myString;
	int lineNum;
};