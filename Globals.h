#pragma once
#include <vector>
#include <string>
#include <map>
// TOKEN_NAME: all the different tokens defined in the language
enum TOKEN_NAME { // MYEOF must be the last enum or a lot of things will break
	COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD,
	SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, MYEOF
};

const std::vector<std::string> TOKEN_NAME_TOSTRING{
	"COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON", "COLON_DASH", "MULTIPLY", "ADD",
	"SCHEMES", "FACTS", "RULES", "QUERIES", "ID", "STRING", "COMMENT", "WHITESPACE", "UNDEFINED", "EOF"
};

// TOKEN_VALUE: if the token value is a regular string, try to match the string exactly
// If token value is NULL, we're looking for some special behavior that can't be mapped by a single string
//const std::vector<std::vector<char>> TOKEN_VALUE = {
//	{ {','} },
//	{ {'.'} },
//	{ {'?'} },
//	{ {'('} },
//	{ {'}'} },
//	{ {':'} },
//	{ {':', '-'} },
//	{ {'*'} },
//	{ {'+'} },
//	{ {'S', 'c', 'h', 'e', 'm', 'e', 's'} },
//	{ {'F', 'a', 'c', 't', 's'} },
//	{ {'R', 'u', 'l', 'e', 's'} },
//	{ {'Q', 'u', 'e', 'r', 'i', 'e', 's'} },
//	{ {0} },
//	{ {0} },
//	{ {0} }, // COMMENT needs to back out if it sees '/r'
//	{ {0} },
//	{ {0} },
//	{ {0} }
//};
const std::vector<std::string> TOKEN_VALUE = { ",", ".", "?", "(", "}", ":", ":-", "*", "+",
"Schemes", "Facts", "Rules", "Queries", "\0", "\0", "\0", "\0", "\0", "\r" };
//const std::map<TOKEN_NAME, std::vector<char>> TOKEN_VALUE = {
//	{COMMA, {','} },
//	{PERIOD, {'.'} },
//	{Q_MARK, {'?'} },
//	{LEFT_PAREN, {'('} },
//	{RIGHT_PAREN, {'}'} },
//	{COLON, {':'} },
//	{COLON_DASH, {':', '-'} },
//	{MULTIPLY, {'*'} },
//	{ADD, {'+'} },
//	{SCHEMES, {'S', 'c', 'h', 'e', 'm', 'e', 's'} },
//	{FACTS, {'F', 'a', 'c', 't', 's'} },
//	{RULES, {'R', 'u', 'l', 'e', 's'} },
//	{QUERIES, {'Q', 'u', 'e', 'r', 'i', 'e', 's'} },
//	{ID, {0} },
//	{STRING, {0} },
//	{COMMENT, {0} }, // COMMENT needs to back out if it sees '/r'
//	{WHITESPACE, {0} },
//	{UNDEFINED, {0} },
//	{MYEOF, {0} }
//};

// TOKEN_PRIORITY: 1 has greater priority, 5 is lowest priority
const std::map<TOKEN_NAME, int> TOKEN_PRIORITY = {
	{ COMMA, 2 },{ PERIOD, 2 },{ Q_MARK, 2 },{ LEFT_PAREN, 2 },{ RIGHT_PAREN, 2 },{ COLON, 2 },{ COLON_DASH, 2 },
	{ MULTIPLY, 2 },{ ADD, 2 },{ SCHEMES, 2 },{ FACTS, 2 },{ RULES, 2 },{ QUERIES, 2 },
	{ ID, 3 },{ STRING, 3 },{ COMMENT, 3 },{ WHITESPACE, 4 },{ UNDEFINED, 5 },{ MYEOF, 1 }
};

// TOKEN_STATE: return values sent to Lexer after the Token instance receives input. A token will only
// return FINISH if it's TOKEN_VALUE is not equal to null
enum TOKEN_STATE {
	INITIALIZE, FAIL, CLEAR, FINISH, NOTDEFINED
};