#pragma once
#include "TokenFSM.h"

// Space_FSM is a unique FSM that will never create a Token instance. Lexer uses it to recognize
//  space characters
class String_FSM : public TokenFSM {
public:
	String_FSM();
	TOKEN_STATE InputChar(char feedChar, int newLineNumber);
	//TOKEN_STATE PeekChar(char feedChar) const;
	TOKEN_STATE PeekCharChange(char feedChar);
	void ResetState();
private:
	enum SUBSTATE {INITIAL, OPENING_APOSTROPHE, BODY, CLOSING_APOSTROPHE};
	SUBSTATE subState;
};
