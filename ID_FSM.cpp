#include "ID_FSM.h"

ID_FSM::ID_FSM() : TokenFSM(ID) {
	//
	isFiniteLength = false;
}
TOKEN_STATE ID_FSM::InputChar(char feedChar, int newLineNumber) {
	// We need to have at least one alpha character; any alphanumeric character after that 
	//  is valid
	if (state == INITIALIZE) {
		if (isalpha(feedChar)) {
			state = CLEAR;
			currentString.push_back(feedChar);
			currentPosition++;
			lineNumber = newLineNumber;
		}
		else {
			return FAIL;
		}
	}
	else {
		if (isalnum(feedChar)) {
			currentString.push_back(feedChar);
			currentPosition++;
		}
		else {
			ResetState();
			return FAIL;
		}
	}
	return state;
}
//TOKEN_STATE ID_FSM::PeekChar(char feedChar) const {
//	if (isalnum(feedChar)) {
//		return CLEAR;
//	}
//	return FAIL;
//}
TOKEN_STATE ID_FSM::PeekCharChange(char feedChar) {
	switch (state) {
	case INITIALIZE:
		return FAIL;
		break;
	default:
		if (isalnum(feedChar)) {
			return CLEAR;
		}
		else {
			state = FINISH;
		}
		break;
	}
	return state;
}