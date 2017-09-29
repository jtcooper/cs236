#include "Comment_FSM.h"

Comment_FSM::Comment_FSM() : TokenFSM(COMMENT) {
	//
	subState = INITIAL;
	isFiniteLength = false;
}
TOKEN_STATE Comment_FSM::InputChar(char feedChar, int newLineNumber) {
	//
	if (state == INITIALIZE) {
		if (feedChar == '#') {
			state = CLEAR;
			subState = OPENING_POUND;
			currentString.push_back(feedChar);
			currentPosition++;
			lineNumber = newLineNumber;
		}
		else {
			return FAIL;
		}
	}
	else {
		if ((subState == OPENING_POUND || subState == SINGLE_LINE) && feedChar == '\r') {
			subState = CLOSING;
			state = FINISH; // a single-line comment can end on EOF
		}
		else if (feedChar == '\r') { // reached the end of the file before proper closing syntax
			state = NOTDEFINED;
		}
		switch (subState) {
		case OPENING_POUND:
			if (feedChar == '|') { subState = MULTI_LINE; }
			else if (feedChar == '\n') {
				subState = CLOSING;
				state = FINISH;
				return state;
			}
			else                 { subState = SINGLE_LINE; }
			currentString.push_back(feedChar);
			currentPosition++;
			break;
		case SINGLE_LINE:
			if (feedChar == '\n') { 
				subState = CLOSING;
				state = FINISH;
			}
			else {
				currentString.push_back(feedChar);
				currentPosition++;
			}
			break;
		case MULTI_LINE:
			if (feedChar == '|') { subState = CLOSING_VERTICAL; }
			currentString.push_back(feedChar);
			currentPosition++;
			break;
		case CLOSING_VERTICAL:
			if (feedChar == '#') { 
				subState = CLOSING; 
				state = FINISH;
			}
			else                 { subState = MULTI_LINE; }
			currentString.push_back(feedChar);
			currentPosition++;
			break;
		}
	}
	return state;
}
//TOKEN_STATE Comment_FSM::PeekChar(char feedChar) const {
//	switch (subState) {
//	case INITIAL:
//	case CLOSING:
//		return FAIL;
//		break;
//	case OPENING_POUND:
//	case SINGLE_LINE:
//		if (feedChar == '\r' || feedChar == '\n') { return FINISH; }
//		break;
//	case CLOSING_VERTICAL:
//		if (feedChar == '#') { return FINISH; }
//		// don't break!
//	case MULTI_LINE:
//		if (feedChar == '\r') { return FAIL; }
//		break;
//	}
//	return CLEAR;
//	
//}
TOKEN_STATE Comment_FSM::PeekCharChange(char feedChar) {
	// the only reason this will change the state is if we change to FINISH (or NOTDEFINED)
	if (feedChar == '\r') {
		if (subState == MULTI_LINE || subState == CLOSING_VERTICAL) {
			state = NOTDEFINED;
		}
	}
	else {
		switch (subState) {
		case INITIAL:
			return FAIL;
			break;
		case OPENING_POUND:
		case SINGLE_LINE:
			if (feedChar == '\r' || feedChar == '\n') { 
				state = FINISH; }
			break;
		case CLOSING_VERTICAL:
			if (feedChar == '#') { state = CLEAR; }
			break;
			// case MULTI_LINE: don't need this one; state should return CLEAR
			//case CLOSING: don't need this one; state is already set to FINISH
		}
	}
	return state;
}
void Comment_FSM::ResetState() {
	TokenFSM::ResetState();
	subState = INITIAL;
}