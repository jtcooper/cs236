#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Lexer.h"

int main() {
	std::ifstream inputFile;
	std::ofstream outputFile;
	std::string fileName;
	//std::string tempLine;
	std::string fileStream;
	Lexer* lexObj;

	std::cout << "Enter input file name: ";
	std::getline(std::cin, fileName);
	std::cout << std::endl;
	inputFile.open(fileName);
	if (!inputFile.is_open()) {
		std::cout << "Invalid file name." << std::endl;
		return 1;
	}
	std::cout << std::endl;
	while (!inputFile.eof()) {
		fileStream.push_back(inputFile.get());
	}
	inputFile.close();
	if (fileStream.back() != '\r') {
		fileStream.push_back('\r');
	}

	lexObj = new Lexer(fileStream);
	std::cout << lexObj->PrintTokens();
	
	std::cout << "\n";
	std::cout << "Output file name: ";
	std::getline(std::cin, fileName);

	outputFile.open(fileName);
	if (!outputFile.is_open()) {
		std::cout << "Unable to open/create file.\n";
		return 1;
	}
	outputFile << lexObj->PrintTokens();
	outputFile.close();
	delete lexObj;

	std::cout << "Press enter to exit.";
	std::getline(std::cin, fileName);
	return 0;
}