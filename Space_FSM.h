#pragma once
#include "TokenFSM.h"

// Space_FSM is a unique FSM that will never create a Token instance. Lexer uses it to recognize
//  space characters
class Space_FSM : public TokenFSM {
public:
	Space_FSM();
	TOKEN_STATE InputChar(char feedChar, int newLineNumber);
};
