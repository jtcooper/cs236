#include "Lexer.h"

Lexer::Lexer(std::string inputStream) {
	// initialize tokens
	//std::swap(newBuffer, sBuffer);
	sBuffer << inputStream;
	for (int i = COMMA; i <= MYEOF; i++) {
		TokenFSM* newToken;
		switch (static_cast<TOKEN_NAME>(i)) {
		case WHITESPACE:
			newToken = new Space_FSM();
			break;
		case ID:
			newToken = new ID_FSM();
			break;
		case STRING:
			newToken = new String_FSM();
			break;
		case COMMENT:
			newToken = new Comment_FSM();
			break;
		default:
			newToken = new Exact_FSM(static_cast<TOKEN_NAME>(i));
			break;
		}
		// FIXME: we might not need to create an undefined token
		tokenFSMs.push_back(newToken);
	}
	currentLineNum = 1;
	Lexicate();
}
Lexer::~Lexer() {
	for (int i = 0; i < tokenFSMs.size(); i++) {
		delete tokenFSMs.at(i);
	}
	for (int j = 0; j < tokens.size(); j++) {
		delete tokens.at(j);
	}
}
void Lexer::Lexicate() {
	// go through the stringstream buffer and create tokens from it
	std::vector<TOKEN_STATE> tokenStates(MYEOF + 1);
	std::vector<int> tokenStatesCount(NOTDEFINED + 1);
	char currentChar = sBuffer.get();
	if (static_cast<int>(currentChar) == -1) {
		currentChar = sBuffer.get();
	}
	char peekChar = sBuffer.peek();
	for (int j = 0; j <= MYEOF; j++) {
		tokenStates.at(j) = FAIL;
	}
	for (int l = 0; l <= FINISH; l++) {
		tokenStatesCount.at(l) = 0;
	}

	// feed currentChar to all of the FSMs
	// if we happen to get a NOTDEFINED state, we need to reset that FSM and create an UNDEFINED token
	for (int i = COMMA; i <= MYEOF; i++) {
		tokenStates.at(i) = tokenFSMs.at(static_cast<TOKEN_NAME>(i))->InputChar(currentChar, currentLineNum);
		if (!tokenFSMs.at(static_cast<TOKEN_NAME>(i))->IsFiniteLength()) {
			// STRING, COMMENT, or ID. We need to feed a peek character
			tokenStates.at(i) = tokenFSMs.at(static_cast<TOKEN_NAME>(i))->PeekCharChange(peekChar);
		}
		tokenStatesCount.at(tokenStates.at(i))++;
		if (tokenStates.at(i) == NOTDEFINED) {
			AddToken(UNDEFINED, tokenFSMs.at(static_cast<TOKEN_NAME>(i))->GetString(), currentLineNum);
			tokenFSMs.at(static_cast<TOKEN_NAME>(i))->ResetState();
		}
	}
	// if the MYEOF FSM returns CLEAR, we can end immediately; this will break the recursive call to Lexicate()
	if (tokenStates.at(MYEOF) != FAIL) {
		// if any indefinite length FSMs return FINISH, we need to pop them first. 
		// There can only be one at a time in this situation
		if (tokenStatesCount.at(FINISH) == 2) {
			for (int k = 0; k < MYEOF; k++) {
				if (tokenStates.at(k) == FINISH) {
					AddToken(tokenFSMs.at(k)->Pop());
				}
			}
		}
		else if (tokenStatesCount.at(FINISH) > 2) {
			// Pretty sure this won't happen!
		}
		AddToken(MYEOF, "", currentLineNum);
		return;
	}

	// If they all return FAIL, we create an UNDEFINED token.
	if (tokenStatesCount.at(FINISH) == 0 && tokenStatesCount.at(CLEAR) == 0) {
		std::string tempString = "";
		tempString.push_back(currentChar);
		AddToken(UNDEFINED, tempString, currentLineNum);

	}
	// If one returns FINISH and none of the others returns CLEAR, we pop that FSM and create a new token instance.
	else if (tokenStatesCount.at(FINISH) == 1 && tokenStatesCount.at(CLEAR) == 0) {
		for (int i = COMMA; i <= MYEOF; i++) {
			if (tokenFSMs.at(static_cast<TOKEN_NAME>(i))->GetState() == FINISH) {
				AddToken(tokenFSMs.at(static_cast<TOKEN_NAME>(i))->Pop());
				break;
			}
		}
	}
	// If more than one returns FINISH, we pick one first by looking at their string lengths. If the lengths are the
	//  same, we pick one based on priority.
	else if (tokenStatesCount.at(FINISH) > 1 && tokenStatesCount.at(CLEAR) == 0) {
		ChooseFinishedFSM(tokenStates);
	}
	// If at least one returns FINISH but at least one of the others returns CLEAR... (see the function it calls)
	else if (tokenStatesCount.at(FINISH) > 0 && tokenStatesCount.at(CLEAR) > 0) {
		ChooseFSM(tokenStates);
	}
	// If we get zero FINISH but two CLEAR, and both of them are indefinite lengths, 
	//  we can save headache later by removing one of them (meaning that the code won't return the right result
	//  without this)
	else if (tokenStatesCount.at(FINISH) == 0 && tokenStatesCount.at(CLEAR) == 2) {
		
		TokenFSM* fFSM = NULL;
		TokenFSM* sFSM = NULL;
		for (int k = 0; k <= MYEOF; k++) {
			if (tokenStates.at(k) == CLEAR) {
				if (fFSM == NULL) {
					fFSM = tokenFSMs.at(k);
				}
				else {
					sFSM = tokenFSMs.at(k);
					break;
				}
			}
		}
		if (!fFSM->IsFiniteLength() && !sFSM->IsFiniteLength()) {
			if (fFSM->GetLength() > sFSM->GetLength()) {
				ResetAllFSMs(fFSM);
			}
			else {
				// impossible for them both to be the same length
				ResetAllFSMs(sFSM);
			}
		}
	}

	//// Debugging code:
	/*std::cout << currentChar << "\n";
	PrintStates(tokenStates, tokenStatesCount);*/

	// Lastly, increment the line number
	if (currentChar == '\n') {
		currentLineNum++;
	}
	Lexicate();
}
void Lexer::ChooseFSM(std::vector<TOKEN_STATE> tokenStates) {
	// if a finished one is longer than any others, we pop it and reset the others
	// if there are multiple of each, we narrow it down to the longest CLEAR and the longest FINISH, and reset the others
		// if the CLEAR FSM is indefinite length, we reset the FINISH FSM (and leave the CLEAR FSM alone)
		// otherwise, we feed the longest CLEAR another character; 
			// if it returns FAIL, we reset the CLEAR and pop the FINISH FSM
			// else, we leave the CLEAR FSM alone and reset the FINISH FSM
	// This function is called when there is a minimum of 1 FINISH and 1 CLEAR FSM
	// examples: COLON: ":" FINISH; COLON_DASH: ":" CLEAR
	//        QUERIES:  "Queries" FINISH;  ID:      "Queries" CLEAR;  STRING: "'Queries" CLEAR
	//        STRING:   "'|#"     CLEAR;   COMMENT: "#|'|#"   FINISH
	//        QUERIES: "Queries"  FINISH;  ID:      "Queries" CLEAR
	std::vector<TokenFSM*> clearFSMs;
	std::vector<TokenFSM*> finishFSMs;
	TokenFSM* longestClearFSM;
	TokenFSM* longestFinishFSM;
	for (int i = 0; i <= MYEOF; i++) {
		if (tokenStates.at(static_cast<TOKEN_NAME>(i)) == CLEAR) {
			clearFSMs.push_back(tokenFSMs.at(static_cast<TOKEN_NAME>(i)));
		}
		else if (tokenStates.at(static_cast<TOKEN_NAME>(i)) == FINISH) {
			finishFSMs.push_back(tokenFSMs.at(static_cast<TOKEN_NAME>(i)));
		}
	}
	longestClearFSM = clearFSMs.at(0);
	for (int j = 1; j < clearFSMs.size(); j++) {
		if (clearFSMs.at(j)->GetLength() > longestClearFSM->GetLength()) {
			longestClearFSM = clearFSMs.at(j);
		}
	}
	longestFinishFSM = finishFSMs.at(0);
	for (int k = 1; k < finishFSMs.size(); k++) {
		if (finishFSMs.at(k)->GetLength() > longestFinishFSM->GetLength()) {
			longestFinishFSM = finishFSMs.at(k);
		}
	}
	// Now make comparisons
	if (longestFinishFSM->GetLength() > longestClearFSM->GetLength()) {
		AddToken(longestFinishFSM->Pop());
		ResetAllFSMs();
	}
	else {
		if (!longestClearFSM->IsFiniteLength()) {
			ResetAllFSMs(longestClearFSM);
		}
		else {
			if (longestClearFSM->PeekChar(sBuffer.peek()) == FAIL) {
				AddToken(longestFinishFSM->Pop());
				ResetAllFSMs();
			}
			else {
				ResetAllFSMs(longestClearFSM);
			}
		}
	}
}
void Lexer::ChooseFinishedFSM(std::vector<TOKEN_STATE> tokenStates) {
	// Called when there are no CLEAR FSMs and minimum 2 FINISH FSMs
	std::vector<int> tokenLengths(MYEOF + 1);
	TOKEN_NAME longest = MYEOF;
	TOKEN_NAME slongest = MYEOF; // ?there will never be a case where more than two have the same length and 
	//									are the longest?
	for (int k = 0; k <= MYEOF; k++) {
		tokenLengths.at(k) = 0;
	}
	// Get the longest and second longest lengths
	for (int i = COMMA; i < MYEOF; i++) { // don't include MYEOF
		if (tokenFSMs.at(static_cast<TOKEN_NAME>(i))->GetState() == FINISH) {
			tokenLengths.at(static_cast<TOKEN_NAME>(i)) = tokenFSMs.at(static_cast<TOKEN_NAME>(i))->GetLength();
			if (tokenLengths.at(static_cast<TOKEN_NAME>(i)) > tokenLengths.at(longest)) {
				slongest = longest;
				longest = static_cast<TOKEN_NAME>(i);
			}
			else if (tokenLengths.at(static_cast<TOKEN_NAME>(i)) > tokenLengths.at(slongest)) {
				slongest = static_cast<TOKEN_NAME>(i);
			}
		}
	}
	// make comparisons
	if (tokenLengths.at(longest) > tokenLengths.at(slongest)) {
		AddToken(tokenFSMs.at(longest)->Pop());
		ResetFinishedFSMs();
	}
	else {
		// FIXME: they're the same length; we need to pick the one with the greater priority now
		if (TOKEN_PRIORITY.at(longest) < TOKEN_PRIORITY.at(slongest)) {
			AddToken(tokenFSMs.at(longest)->Pop());
			ResetFinishedFSMs();
		}
	}
}

void Lexer::ResetFinishedFSMs() {
	for (int i = COMMA; i <= MYEOF; i++) {
		if (tokenFSMs.at(static_cast<TOKEN_NAME>(i))->GetState() == FINISH) {
			tokenFSMs.at(static_cast<TOKEN_NAME>(i))->ResetState();
		}
	}
	return;
}
void Lexer::ResetAllFSMs(TokenFSM* preserveFSM) {
	// called after a token has been added; no reason to hang on to any others
	for (int i = 0; i <= MYEOF; i++) {
		if (preserveFSM != NULL) {
			if (preserveFSM->GetName() != tokenFSMs.at(static_cast<TOKEN_NAME>(i))->GetName()) {
				tokenFSMs.at(static_cast<TOKEN_NAME>(i))->ResetState();
			}
		}
		
	}
}

std::string Lexer::PrintTokens() const {
	std::string outputString = "";
	for (int i = 0; i < static_cast<int>(tokens.size()); i++) {
		outputString += tokens.at(i)->Print();
		outputString += "\n";
	}
	outputString += "Total Tokens = " + std::to_string(tokens.size());
	return outputString;
}
void Lexer::AddToken(TOKEN_NAME newName, std::string newString, int newLineNum) {
	if (newName == WHITESPACE) {
		// we don't want to add a whitespace token
	}
	else {
		tokens.push_back(new Token(newName, newString, newLineNum));
		//std::cout << PrintTokens() << "\n";
	}
}
void Lexer::AddToken(Token* newToken) {
	if (newToken->GetName() == WHITESPACE) {
		// we don't want to add a whitespace token
		delete newToken;
	}
	else {
		tokens.push_back(newToken);
		//std::cout << PrintTokens() << "\n"; // debugging
	}
}
// Debugging function
//void Lexer::PrintStates(std::vector<TOKEN_STATE> tokenStates, std::vector<int> tokenStatesCount) const {
//	for (int m = 0; m <= MYEOF; m++) {
//		TOKEN_STATE myState = tokenStates.at(static_cast<TOKEN_NAME>(m));
//		std::cout << TOKEN_NAME_TOSTRING.at(m) << ": ";
//		switch (myState) {
//		case INITIALIZE:
//			 std::cout << "Initialize";
//			break;
//		case FAIL:
//			std::cout << "Fail";
//			break;
//		case CLEAR:
//			std::cout << "Clear";
//			break;
//		case FINISH:
//			std::cout << "Finish";
//			break;
//		default:
//			std::cout << "What?";
//			break;
//		}
//		std::cout << ";  ";
//	}
//	std::cout << "\n";
//	std::cout << "INITIALIZE: " << tokenStatesCount.at(INITIALIZE) << ";  ";
//	std::cout << "FAIL: "       << tokenStatesCount.at(FAIL) << ";  ";
//	std::cout << "CLEAR: "      << tokenStatesCount.at(CLEAR) << ";  ";
//	std::cout << "FINISH: "     << tokenStatesCount.at(FINISH) << ";  ";
//	std::cout << "\n";
//	std::string tempoString;
//	std::getline(std::cin, tempoString);
//}