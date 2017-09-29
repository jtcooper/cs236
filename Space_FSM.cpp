#include "Space_FSM.h"

Space_FSM::Space_FSM() : TokenFSM(WHITESPACE) {
	//
}
TOKEN_STATE Space_FSM::InputChar(char feedChar, int newLineNumber) {
	if (isspace(feedChar) && (feedChar != '\r')) {
		return FINISH;
	}
	return FAIL;
}