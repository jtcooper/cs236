// used to check answers, comparing two txt files
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
	ifstream answerKey;
	ifstream myAnswers;
	string fileName;
	string tempLine;
	string tempLineB;
	int totalErrors = 0;

	cout << "Enter answer key file name: ";
	cin >> fileName;
	cout << endl;
	answerKey.open(fileName);
	if (!answerKey.is_open()) {
		cout << "Invalid file name." << endl;
		return 1;
	}
	cout << endl;
	cout << "Enter your answer file name: ";
	cin >> fileName;
	cout << endl;
	myAnswers.open(fileName);
	if (!myAnswers.is_open()) {
		cout << "Invalid file name." << endl;
		return 1;
	}
	cout << endl;

	while (!answerKey.eof()) {
		getline(answerKey, tempLine);
		getline(myAnswers, tempLineB);
		if (tempLine.length() > 0) {
			if (tempLine.back() == '\n' || tempLine.back() == '\0' || tempLine.back() == 13) {
				tempLine.pop_back();
			}
		}
		if (tempLineB.length() > 0) {
			if (tempLineB.back() == '\n' || tempLine.back() == '\0' || tempLine.back() == 13) {
				tempLineB.pop_back();
			}
		}
		if (tempLine != tempLineB) {
			cout << tempLine << "  --- ERROR ---  " << tempLineB << endl;
			totalErrors++;
		}
		else {
			cout << tempLine << endl;
		}
	}
	if (!myAnswers.eof()) {
		cout << "Your answer file is longer than the answer key's file." << endl;
	}
	cout << "Total errors: " << totalErrors << endl << endl;
	answerKey.close();
	myAnswers.close();
}