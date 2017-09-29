#pragma once
#include "TokenFSM.h"

class ID_FSM : public TokenFSM {
public:
	ID_FSM();
	TOKEN_STATE InputChar(char feedChar, int newLineNumber);
	//TOKEN_STATE PeekChar(char feedChar) const;
	TOKEN_STATE PeekCharChange(char feedChar);
};