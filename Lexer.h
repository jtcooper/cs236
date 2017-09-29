#pragma once
#include <vector>
#include <sstream>
#include <string>
#include "TokenFSM.h"
#include "Token.h"
#include "Space_FSM.h"
#include "Exact_FSM.h"
#include "ID_FSM.h"
#include "String_FSM.h"
#include "Comment_FSM.h"

class Lexer {
public:
	Lexer(std::string inputStream);
	~Lexer();
	std::string PrintTokens() const;
private:
	std::vector<TokenFSM*> tokenFSMs;
	std::stringstream sBuffer;
	std::vector<Token*> tokens;
	int currentLineNum;
	void Lexicate();
	void ChooseFSM(std::vector<TOKEN_STATE> tokenStates);
	void ChooseFinishedFSM(std::vector<TOKEN_STATE> tokenStates);
	void ResetFinishedFSMs();
	void ResetAllFSMs(TokenFSM* preserveFSM = NULL);
	void AddToken(TOKEN_NAME newName, std::string newString, int newLineNum);
	void AddToken(Token* newToken);
	//void PrintStates(std::vector<TOKEN_STATE> tokenStates, std::vector<int> tokenStatesCount) const; // debugging function
};