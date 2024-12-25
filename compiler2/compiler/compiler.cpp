#include <iostream>
#include <fstream>
#include "lexical_analyzer.h"
#include "HashTable.h"
#include "Semantic.h"

int main() {
	setlocale(0, "");
	ifstream inputFile("input.txt");
	ofstream outputFile("output.txt");
	ofstream outputErrors("errors.txt");
	ofstream outputsemerr("semerr.txt");
	ofstream outputsem("outputsem.txt");
	HashTable table;
	Lexer s(inputFile, table);
	Node n("Procedure");
	s.analyze(n);
	table.print(outputFile);
	n.print(0);
	s.showErrors(outputErrors);
	Semantic_Analyzer(n, outputsemerr);
	if (!error) {
		n.postfixbegin(n[0], outputsem);
		n.postfixDescriptions(n[1], outputsem);
		n.postfixOperators(n[2], outputsem);
		n.postfixend(n[3], outputsem);
	}
}