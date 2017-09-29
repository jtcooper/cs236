#include "TokenFSM.h"

TokenFSM::TokenFSM(TOKEN_NAME newName) {
	name = newName;
	state = INITIALIZE;
	currentPosition = 0;
	currentString = "";
	lineNumber = 0;
	isFiniteLength = true;
}
TOKEN_STATE TokenFSM::PeekChar(char feedChar) const {
	if (GetTokenValue() == '\0') {
		// call a special function

	}
	else {
		if (feedChar == GetTokenValue(currentPosition)) {
			// we're returning CLEAR or FINISH
			if ((currentPosition + 1) >= GetTokenLength()) {
				return FINISH;
			}
			else { return CLEAR; }
		}
		else { return FAIL; }
	}
	return FAIL;
}
TOKEN_STATE TokenFSM::PeekCharChange(char feedChar) {
	// default behavior: most functions that use this will overwrite this. IE: FSMs with a potentially infinite length
	return FAIL;
}
void TokenFSM::ResetState() {
	state = INITIALIZE;
	currentPosition = 0;
	currentString = "";
	lineNumber = 0;
}
Token* TokenFSM::Pop() {
	// return a pointer to a new Token instance
	Token* tokenPtr = new Token(name, currentString, lineNumber);
	ResetState();
	return tokenPtr;
}
Token* TokenFSM::Pop(int lineNumber) {
	// return a pointer to a new Token instance
	if (this->lineNumber != 0) {
		this->lineNumber = lineNumber;
	}
	Token* tokenPtr = new Token(name, currentString, this->lineNumber);
	ResetState();
	return tokenPtr;
}
TOKEN_NAME TokenFSM::GetName() const {
	return name;
}
TOKEN_STATE TokenFSM::GetState() const {
	return state;
}
std::string TokenFSM::GetString() const {
	return currentString;
}
int TokenFSM::GetLength() const {
	return currentPosition;
}
bool TokenFSM::IsFiniteLength() const {
	return isFiniteLength;
}

char TokenFSM::GetTokenValue(int position) const { //default: position = 0
	/*char valueString[] = TOKEN_VALUE.find(COMMA);
	return valueString[position];*/
	return TOKEN_VALUE[name][position];
}
int TokenFSM::GetTokenLength() const {
	int returnVal = TOKEN_VALUE[name].length();
	return returnVal;
}