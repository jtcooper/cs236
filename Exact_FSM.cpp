#include "Exact_FSM.h"

Exact_FSM::Exact_FSM(TOKEN_NAME newName) : TokenFSM(newName) {
	//
}
TOKEN_STATE Exact_FSM::InputChar(char feedChar, int newLineNumber) {
	// The Lexer is feeding us a character
	if (GetTokenValue() == '\0') {
		return FAIL;
	}
	else {
		if (feedChar == GetTokenValue(currentPosition)) {
			if (state == INITIALIZE) {
				lineNumber = newLineNumber;
				state = CLEAR;
			}
			currentString.push_back(feedChar);
			currentPosition++;
		}
		else {
			ResetState();
			return FAIL;
		}
		if (currentPosition >= GetTokenLength()) {
			// we've finished the entire string; return success
			state = FINISH;
		}
	}
	return state;
}