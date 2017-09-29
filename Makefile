CCFLAGS = -std=c++11 -g -Wall

lab: main.cpp LexAnalyzer.o
	g++ $(CCFLAGS) main.cpp LexAnalyzer.o -o lab

LexAnalyzer.o: Globals.h TokenFSM.h TokenFSM.cpp Comment_FSM.h Comment_FSM.cpp Exact_FSM.h Exact_FSM.cpp ID_FSM.h ID_FSM.cpp Space_FSM.h Space_FSM.cpp String_FSM.h String_FSM.cpp Token.h Token.cpp Lexer.h Lexer.cpp
	g++ $(CCFLAGS) -c TokenFSM.cpp Comment_FSM.cpp Exact_FSM.cpp ID_FSM.cpp Space_FSM.cpp String_FSM.cpp Token.cpp Lexer.cpp

CA: CheckAnswers.cpp
	g++ $(CCFLAGS) CheckAnswers.cpp -o checkanswers
