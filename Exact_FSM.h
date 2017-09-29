#pragma once
#include "TokenFSM.h"

// Exact_FSM is the default FSM where TOKEN_NAME_TOSTRING has a regular, fixed value
class Exact_FSM : public TokenFSM {
public:
	Exact_FSM(TOKEN_NAME newName);
	TOKEN_STATE InputChar(char feedChar, int newLineNumber);
};
