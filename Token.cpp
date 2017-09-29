#include "Token.h"

Token::Token(TOKEN_NAME newName, std::string newString, int newLineNum) {
	name = newName;
	myString = newString;
	lineNum = newLineNum;
}
std::string Token::Print() const {
	std::string returnString = "(";
	returnString += TOKEN_NAME_TOSTRING.at(name);
	returnString += ",\"";
	returnString += myString + "\",";
	returnString += std::to_string(lineNum);
	returnString += ")";
	return returnString;
}
TOKEN_NAME Token::GetName() const {
	return name;
}