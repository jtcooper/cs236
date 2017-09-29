#include "String_FSM.h"

String_FSM::String_FSM() : TokenFSM(STRING) {
	//
	subState = INITIAL;
	isFiniteLength = false;
}
TOKEN_STATE String_FSM::InputChar(char feedChar, int newLineNumber) {
	//
	if (state == INITIALIZE) {
		if (feedChar == '\'') {
			state = CLEAR;
			subState = OPENING_APOSTROPHE;
			currentString.push_back(feedChar);
			currentPosition++;
			lineNumber = newLineNumber;
		}
		else {
			return FAIL;
		}
	}
	else {
		if (feedChar == '\r') { // reached the end of the file before a closing apostrophe
			state = NOTDEFINED;
		}
		else if (subState == OPENING_APOSTROPHE) {
			if (feedChar == '\'') {
				subState = CLOSING_APOSTROPHE;
			}
			else {
				subState = BODY;
			}
			currentString.push_back(feedChar);
			currentPosition++;
		}
		else if (subState == BODY) {
			if (feedChar == '\'') {
				subState = CLOSING_APOSTROPHE;
			}
			currentString.push_back(feedChar);
			currentPosition++;
		}
		else if (subState == CLOSING_APOSTROPHE) {
			if (feedChar == '\'') {
				subState = OPENING_APOSTROPHE;
				currentString.push_back(feedChar);
				currentPosition++;
			}
			else {
				// if we apply the PeekChar functions correctly, this should never be called
				state = FINISH;
				currentString.push_back(feedChar);
				currentPosition++;
			}
		}
	}
	return state;
}
//TOKEN_STATE String_FSM::PeekChar(char feedChar) const {
//	if (feedChar == '\r') {
//		return FAIL;
//	}
//	if ((subState == CLOSING_APOSTROPHE) && (feedChar != '\'')) {
//		return FINISH;
//	}
//	return CLEAR;
//}
TOKEN_STATE String_FSM::PeekCharChange(char feedChar) {
	switch (state) {
	case INITIALIZE:
		return FAIL;
		break;
	default:
		if ((subState == CLOSING_APOSTROPHE) && (feedChar != '\'')) {
			state = FINISH;
		}
		else if (feedChar == '\r') {
			if (subState == CLOSING_APOSTROPHE) {
				state = FINISH;
			}
			else {
				state = NOTDEFINED;
			}
		}
		break;
	}
	return state;
}
void String_FSM::ResetState() {
	TokenFSM::ResetState();
	//state = INITIALIZE;
	subState = INITIAL;
	//currentPosition = 0;
	//currentString = "";
}