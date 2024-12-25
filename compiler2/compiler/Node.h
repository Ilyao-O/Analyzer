#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

bool value = 0;
int cnt = 0;
int cntop = 1;

class Node
{
private:
	vector<Node> children;
public:
	Node* root;
	bool value = 0;
	string data;
	int size = 0;
	bool anons = 0;
	bool flage = 0;
	Node(string s)
	{
		data = s;
		this->root = this;
	}

	void addSon(string s)
	{
		Node n(s);
		n.root = this;
		children.push_back(n);
		size++;
	}

	Node& operator[](int i)
	{
		return children[i];
	}

	void print(int lvl)
	{
		for (int i = 0; i < lvl; ++i)
			cout << "  ";
		cout << data << endl;
		for (Node n : children)
			n.print(lvl + 1);
	}

	void postfixbegin(Node& n, ofstream& outputsem) {
		if (n.data == "Begin") {
			for (int i = 0;i < 2;i++) {
				postfixbegin(n[i], outputsem);
			}
		}
		else if (n.data == "procedure") {
			outputsem << "procedure ";
		}
		else if (n.data == "ProcedureName") {
			postfixbegin(n[0], outputsem);
		}
		else if (n.data == "Id") {
			postfixbegin(n[0], outputsem);
		}
		else {
			outputsem << n.data << " FB\n";
		}
	}


	void postfixDescriptions(Node& n, ofstream& outputsem) {
		if (n.data == "Descriptions") {
			postfixDescriptions(n[0], outputsem);
		}
		else if (n.data == "DescrList") {
			postfixDescriptions(n[1], outputsem);
			postfixDescriptions(n[2], outputsem);
		}
		else if (n.data == "Descr") {
			postfixDescriptions(n[2], outputsem);
			postfixDescriptions(n[0], outputsem);
		}
		else if (n.data == "Type") {
			postfixDescriptions(n[0], outputsem);
		}
		else if (n.data == "integer") {
			cnt = 0;
			outputsem << "integer ";
		}
		else if (n.data == "VarList") {
			postfixDescriptions(n[0], outputsem);
			postfixDescriptions(n[1], outputsem);
		}
		else if (n.data == "Id") {
			value = 1;
			postfixDescriptions(n[0], outputsem);
		}
		else if (value) {
			value = 0;
			outputsem << n.data << ' ';
			cnt++;
		}
		else if (n.data == "VarList1") {
			for (int i = 0;i < n.size;i++) {
				if (n.data == "Id") {
					value = 1;
					postfixDescriptions(n[i], outputsem);
				}
				else if (n.data == "VarList1") {
					postfixDescriptions(n[i], outputsem);
				}
			}
		}
		else if (n.data == "eps") {
			outputsem << cnt + 1 << " DECL\n";
		}
		else if (n.data == "DescrList1") {
			if (n.size == 3) {
				postfixDescriptions(n[1], outputsem);
				postfixDescriptions(n[2], outputsem);
			}
		}
	}


	void postfixOperators(Node& n, ofstream& outputsem) {
		if (n.data == "Operators") {
			postfixOperators(n[0], outputsem);
			postfixOperators(n[1], outputsem);
		}
		else if (n.data == "Op") {
			if (n.size == 4) {
				postfixOperators(n[0], outputsem);
				postfixOperators(n[2], outputsem);
				value = 1;
				postfixOperators(n[1], outputsem);
				outputsem << '\n';
			}
			else if (n.size == 5) {
				outputsem << 'm' << cntop << " DEFL\n";
				cntop += 2;
				postfixOperators(n[1], outputsem);
				postfixOperators(n[3], outputsem);
				cntop -= 2;
				outputsem << 'm' << cntop + 1 << " BF\n";
				outputsem << 'm' << cntop << " BRL\n";
				outputsem << 'm' << cntop + 1 << " DEFL\n";
			}
		}
		else if (n.data == "Const") {
			value = 1;
			postfixOperators(n[0], outputsem);
		}
		else if (n.data == "Id") {
			value = 1;
			postfixOperators(n[0], outputsem);
		}
		else if (value) {
			value = 0;
			outputsem << n.data << ' ';
		}
		else if (n.data == "Expr") {
			postfixOperators(n[0], outputsem);
			postfixOperators(n[1], outputsem);
		}
		else if (n.data == "SimpleExpr") {
			if (n.size == 3) {
				postfixOperators(n[1], outputsem);
			}
			else {
				postfixOperators(n[0], outputsem);
			}
		}
		else if (n.data == "Expr1") {
			if (n.size == 3) {
				postfixOperators(n[1], outputsem);
				postfixOperators(n[2], outputsem);
				value = 1;
				postfixOperators(n[0], outputsem);
			}
		}
		else if (n.data == "Operators1") {
			if (n.size == 2) {
				postfixOperators(n[0], outputsem);
				postfixOperators(n[1], outputsem);
			}
		}
		else if (n.data == "Condition") {
			postfixOperators(n[0], outputsem);
			postfixOperators(n[2], outputsem);
			postfixOperators(n[1], outputsem);
		}
		else if (n.data == "RelationOperator") {
			value = 1;
			postfixOperators(n[0], outputsem);
			outputsem << '\n';
		}
	}

	void postfixend(Node& n, ofstream& outputsem) {
		outputsem << "end";
	}
};
