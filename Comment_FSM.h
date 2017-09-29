#pragma once
#include "TokenFSM.h"

// Space_FSM is a unique FSM that will never create a Token instance. Lexer uses it to recognize
//  space characters
class Comment_FSM : public TokenFSM {
public:
	Comment_FSM();
	TOKEN_STATE InputChar(char feedChar, int NewLineNumber);
	//TOKEN_STATE PeekChar(char feedChar) const;
	TOKEN_STATE PeekCharChange(char feedChar);
	void ResetState();
private:
	enum SUBSTATE { INITIAL, OPENING_POUND, SINGLE_LINE, MULTI_LINE, CLOSING_VERTICAL, CLOSING };
	SUBSTATE subState;
};
