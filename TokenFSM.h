#pragma once
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include "Token.h"

class TokenFSM {
public:
	TokenFSM(TOKEN_NAME newName);
	virtual TOKEN_STATE InputChar(char feedChar, int newLineNumber) = 0;
	virtual TOKEN_STATE PeekChar(char feedChar) const;
	virtual TOKEN_STATE PeekCharChange(char feedChar);
	virtual void ResetState();
	Token* Pop();
	Token* Pop(int lineNumber);
	TOKEN_NAME GetName() const;
	TOKEN_STATE GetState() const;
	std::string GetString() const;
	int GetLength() const;
	bool IsFiniteLength() const;
protected:
	TOKEN_NAME name;
	TOKEN_STATE state;
	int currentPosition; // used to calculate length
	//std::vector<char> currentString;
	std::string currentString;
	int lineNumber;
	bool isFiniteLength;
	char GetTokenValue(int position = 0) const;
	int GetTokenLength() const;
};