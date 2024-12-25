#pragma once
#include "Node.h"
#include "lexical_analyzer.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

vector<pair<string, bool>>initial;
bool f = 0;
bool error = 0;
bool DescriptionsAnnons = 0;
bool Opid = 0;
bool SimpleExprid = 0;
bool Conditionalid = 0;
bool izExpr = 0;

void Semantic_Analyzer(Node& n, ofstream& errorsem) {
	if (n.data == "Procedure" && !error) {
		for (int i = 1;i < 3;i++) {
			Semantic_Analyzer(n[i], errorsem);
		}
	}
	else if (n.data == "Descriptions" && !error) {
		for (int i = 0;i < n.size;i++) {
			if (error) {
				break;
			}
			string ni = n[i].data;
			if (ni == "DescrList") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "DescrList1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}
	else if (n.data == "DescrList" && !error) {
		for (int i = 0;i < n.size;i++) {
			if (error) {
				break;
			}
			string ni = n[i].data;
			if (ni == "Descr") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "DescrList1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}
	else if (n.data == "Descr" && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "VarList") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}
	else if (n.data == "DescrList1") {
		for (int i = 0;i < n.size;i++) {
			if (error) {
				break;
			}
			string ni = n[i].data;
			if (ni == "Descr") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "DescrList1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}
	else if (n.data == "VarList" && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "Id") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "VarList1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}
	else if (n.data == "VarList1" && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "Id") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "VarList1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}
	else if (n.data == "Id" && DescriptionsAnnons == 0 && !error) {
		f = 1;
		Semantic_Analyzer(n[0], errorsem);
	}
	else if (f && DescriptionsAnnons == 0 && !error) {
		f = 0;
		pair<string, bool>p{ n.data, 0 };
		if (find(initial.begin(), initial.end(), p) != initial.end()) {
			errorsem << "Переменная " << n.data << " уже объявлена\n";
			error = 1;
		}
		else {
			initial.push_back(p);
		}
	}

	else if (n.data == "Operators" && !error) {
		DescriptionsAnnons = 1;
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "Op") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "Operators1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}

	else if (n.data == "Op" && !error) {
		int id;
		if (n.size == 4) {
			Opid = 1;
			Semantic_Analyzer(n[0], errorsem);
			Semantic_Analyzer(n[2], errorsem);
			if (!error) {
				izExpr = 1;
				Semantic_Analyzer(n[0], errorsem);
			}
		}
		else if (n.size == 5) {
			Semantic_Analyzer(n[1], errorsem);
			Semantic_Analyzer(n[3], errorsem);
		}
	}

	else if (n.data == "Operators1" && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "Op") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "Operators1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}


	else if (n.data == "Id" && Opid && !error) {
		f = 1;
		Semantic_Analyzer(n[0], errorsem);
	}
	else if (f && Opid && !error) {
		f = 0;
		Opid = 0;
		pair<string, bool>p{ n.data, 0 };
		pair<string, bool>p1{ n.data, 1 };
		if ((find(initial.begin(), initial.end(), p) == initial.end()) && (find(initial.begin(), initial.end(), p1) == initial.end())) {
			errorsem << "Переменная " << n.data << " не объявлена\n";
			error = 1;
		}
	}

	else if (n.data == "Id" && izExpr && !Conditionalid && !error) {
		f = 1;
		Semantic_Analyzer(n[0], errorsem);
	}

	else if (f && izExpr && !error) {
		f = 0;
		izExpr = 0;
		for (int i = 0;i < initial.size();i++) {
			if (initial[i].first == n.data) {
				initial[i].second = 1;
			}
		}
	}

	else if (n.data == "Expr" && !Conditionalid && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "SimpleExpr") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "Expr1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}

	else if (n.data == "Expr1" && !Conditionalid && !error) {
			for (int i = 0;i < n.size;i++) {
				string ni = n[i].data;
				if (error) {
					break;
				}
				if (ni == "SimpleExpr") {
					Semantic_Analyzer(n[i], errorsem);
				}
				else if (ni == "Expr1") {
					Semantic_Analyzer(n[i], errorsem);
				}
			}
			}


	else if (n.data == "SimpleExpr" && !Conditionalid && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
						break;
			}
			if (ni == "Id") {
				SimpleExprid = 1;
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "Expr") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}



	else if (n.data == "Id" && SimpleExprid && !Conditionalid && !error) {
		f = 1;
		Semantic_Analyzer(n[0], errorsem);
	}
	else if (f && SimpleExprid && !error) {
		f = 0;
		SimpleExprid = 0;
		pair<string, bool>p1{ n.data, 1 };
		pair<string, bool>p2{ n.data, 0 };
		bool f1 = find(initial.begin(), initial.end(), p1) != initial.end();
		bool f2 = find(initial.begin(), initial.end(), p2) != initial.end();
		if (!f2 && !f1) {
			errorsem << "Переменная " << n.data << " не объявлена\n";
			error = 1;
		}
		if (!f1 && f2) {
			errorsem << "Переменная " << n.data << " не инициализирована\n";
			error = 1;
		}
	}








	else if (n.data == "Condition" && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			int id;
			if (error) {
				break;
			}
			if (ni == "Expr") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
		Conditionalid = 0;
	}



	else if (n.data == "Expr" && Conditionalid && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			int id;
			if (error) {
				break;
			}
			if (ni == "SimpleExpr") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "Expr1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}


	else if (n.data == "Expr1" && Conditionalid && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			int id;
			if (error) {
				break;
			}
			if (ni == "SimpleExpr") {
				Semantic_Analyzer(n[i], errorsem);
			}
			else if (ni == "Expr1") {
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}

	else if (n.data == "SimpleExpr" && Conditionalid && !error) {
		for (int i = 0;i < n.size;i++) {
			string ni = n[i].data;
			if (error) {
				break;
			}
			if (ni == "Id") {
				SimpleExprid = 1;
				Semantic_Analyzer(n[i], errorsem);
			}
		}
	}


	else if (n.data == "Id" && Conditionalid && !error) {
		f = 1;
		Semantic_Analyzer(n[0], errorsem);
	}
	else if (f && Conditionalid && !error) {
		f = 0;
		pair<string, bool>p{ n.data, 0 };
		pair<string, bool>p1{ n.data, 1 };
		if ((find(initial.begin(), initial.end(), p) == initial.end()) && (find(initial.begin(), initial.end(), p1) == initial.end())) {
			errorsem << "Переменная " << n.data << " не объявлена\n";
			error = 1;
		}
		else if (find(initial.begin(), initial.end(), p) == initial.end()) {
			errorsem << "Переменная " << n.data << " не инициализирована\n";
			error = 1;
		}
	}
}