#pragma once
#include "Node.h"
#include "lexical_analyzer.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Syntactic_analysis
{
private:
	string input, lexeme, type_lexeme;
	int i;
	int line;
	void error() {
		ofstream out("error.txt");
		out << "Ошибка в строке " << line;
		out.close();
		exit(0);
	}
	void error(string message) {
		ofstream out("error.txt");
		out << message;
		out.close();
		exit(0);
	}
public:

	void setlex(string lex, string typelex) {
		lexeme = lex;
		type_lexeme = typelex;
	}

	

	Syntactic_analysis() {
		i = 0;
	}

	void parse(Node& root,int l)
	{
		line = l;
		if (root.flage == 0) {
			Procedure(root);
		}
		else error();
	}

	void Procedure(Node& n)
	{
		bool f = 1;
		if (n.anons == 0) {
			n.addSon("Begin");
			n.addSon("Descriptions");
			n.addSon("Operators");
			n.addSon("End");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			Begin(n[0]);
			f = 0;
		}
		else if (n[1].flage == 0) {
			Descriptions(n[1]);
			f = 0;
		}
		if (n[2].flage == 0 && n[1].flage == 1) {
			Operators(n[2]);
			f = 0;
		}
		if (n[3].flage == 0 && n[2].flage == 1) {
			End(n[3]);
			if (n[3].flage == 1) {
				n.flage = 1;
			}
			f = 0;
		}
		if (f) error();
	}

	void Begin(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("procedure");
			n.addSon("ProcedureName");
			n.addSon(";");
			n.addSon("begin");
			n.anons = 1;
		}
		if (n[0].flage == 0 && lexeme == "procedure") {
			n[0].flage = 1;
		}
		else if (n[1].flage == 0 && n[0].flage == 1) {
			ProcedureName(n[1]);
		}
		else if (n[2].flage == 0 && lexeme == ";" && n[1].flage == 1) {
			n[2].flage = 1;
		}
		else if (n[3].flage == 0 && n[2].flage == 1 && lexeme == "begin") {
			n[3].flage = 1;
			n.flage = 1;
		}
		else error();
	}

	void End(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("end");
			n.anons = 1;
		}
		if (n[0].flage == 0 && lexeme == "end") {
			n[0].flage = 1;
			n.flage = 1;
		}
		else error();
	}

	void ProcedureName(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("Id");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			Id(n[0]);
			if (n[0].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void Descriptions(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("DescrList");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			DescrList(n[0]);
			if (n[0].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void DescrList(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("var");
			n.addSon("Descr");
			n.addSon("DescrList1");
			n.anons = 1;
		}
		if (n[0].flage == 0 && lexeme == "var") {
			n[0].flage = 1;
		}
		else if (n[1].flage == 0 && n[0].flage == 1) {
			Descr(n[1]);
		}
		else if (n[2].flage == 0 && n[1].flage == 1) {
			DescrList1(n[2]);
			if (n[2].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void DescrList1(Node& n)
	{
		if (n.anons == 0 && (type_lexeme == "id_name" || type_lexeme == "repeat")) {
			n.flage = 1;
			n.addSon("eps");
		}
		else {
			if (n.anons == 0) {
				n.addSon("var");
				n.addSon("Descr");
				n.addSon("DescrList1");
				n.anons = 1;
			}
			if (n[0].flage == 0 && lexeme == "var") {
				n[0].flage = 1;
			}
			else if (n[1].flage == 0 && n[0].flage == 1) {
				Descr(n[1]);
			}
			else if (n[2].flage == 0 && n[1].flage == 1) {
				DescrList1(n[2]);
				if (n[2].flage == 1) {
					n.flage = 1;
				}
			}
			else error();
		}
	}

	void Descr(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("VarList");
			n.addSon(":");
			n.addSon("Type");
			n.addSon(";");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			VarList(n[0]);
		}
		else if (n[1].flage == 0 && lexeme == ":") {
			n[1].flage == 1;
		}
		else if (n[2].flage == 0) {
			Type(n[2]);
		}
		else if (n[3].flage == 0 && lexeme == ";") {
			n[3].flage = 1;
			if (n[3].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void VarList(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("Id");
			n.addSon("VarList1");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			Id(n[0]);
		}
		else if (n[1].flage == 0) {
			VarList1(n[1]);
			if (n[1].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void VarList1(Node& n)
	{
		if (n.anons == 0 && lexeme == ":") {
			n.flage = 1;
			n.addSon("eps");
		}
		else {
			if (n.anons == 0) {
				n.addSon(",");
				n.addSon("Id");
				n.addSon("VarList1");
				n.anons = 1;
			}
			if (n[0].flage == 0 && lexeme == ",") {
				n[0].flage = 1;
			}
			else if (n[1].flage == 0 && n[0].flage == 1) {
				Id(n[1]);
			}
			else if (n[2].flage == 0 && n[1].flage == 1) {
				VarList1(n[2]);
				if (n[2].flage == 1) {
					n.flage = 1;
				}
			}
			else error();
		}
	}

	void Type(Node& n)
	{
		if (lexeme == "integer") {
			n.addSon("integer");
			n.flage = 1;
		}
		else error("Ожидался тип данных вместо " + lexeme);
	}

	void Operators(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("Op");
			n.addSon("Operators1");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			Op(n[0]);
		}
		else if (n[1].flage == 0) {
			Operators1(n[1]);
			if (n[1].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void Operators1(Node& n)
	{
		if (n.anons == 0 && (lexeme == "end" || lexeme == "until")) {
			n.flage = 1;
			n.addSon("eps");
		}
		else {
			if (n.anons == 0) {
				n.addSon("Op");
				n.addSon("Operators1");
				n.anons = 1;
			}
			if (n[0].flage == 0) {
				Op(n[0]);
			}
			else if (n[1].flage == 0) {
				Operators1(n[1]);
				if (n[1].flage == 1) {
					n.flage = 1;
				}
			}
			else error();
		}
	}

	void Op(Node& n)
	{
		if (n.anons == 0) {
			if (type_lexeme == "id_name") {
				n.addSon("Id");
				n.addSon(":=");
				n.addSon("Expr");
				n.addSon(";");
			}
			else if (type_lexeme == "repeat") {
				n.addSon("repeat");
				n.addSon("Operators");
				n.addSon("until");
				n.addSon("Condition");
				n.addSon(";");
			}
			else {
				error();
			}
			n.anons = 1;
		}
		if (n[0].data == "Id") {
			if (n[0].flage == 0) {
				Id(n[0]);
			}
			else if (n[1].flage == 0 && lexeme == ":=") {
				n[1].flage = 1;
			}
			else if (n[2].flage == 0 && n[1].flage == 1) {
				Expr(n[2]);
				n[3].flage = 1;
				if (n[2].flage == 1 && lexeme == ";")
					n.flage = 1;
			}
			else error();
		}
		else if (n[0].data == "repeat") {
			bool f = true;
			if (n[0].flage == 0 && lexeme == "repeat") {
				n[0].flage = 1;
				f = 0;
			}
			else if (n[1].flage == 0 && n[0].flage == 1) {
				Operators(n[1]);
				f = 0;
			}
			if (n[2].flage == 0 && lexeme == "until" && n[1].flage == 1) {
				n[2].flage = 1;
				f = 0;
			}
			else if (n[3].flage == 0 && n[2].flage == 1) {
				Condition(n[3]);
				f = 0;
			}
			if (n[4].flage == 0 && lexeme == ";" && n[3].flage == 1) {
				n[4].flage = 1;
				n.flage = 1;
				f = 0;
			}
			if(f) error();
		}
		else {
			error();
		}
	}

	void Expr(Node& n)
	{
		if (n.anons == 0) {
			n.addSon("SimpleExpr");
			n.addSon("Expr1");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			SimpleExpr(n[0]);
		}
		else if (n[1].flage == 0) {
			Expr1(n[1]);
			if (n[1].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void Expr1(Node& n)
	{
		if (n.anons == 0 && (lexeme == ")" || lexeme == ";" || lexeme == ">" || lexeme == "<" || lexeme == "=" || lexeme == "<>")) {
			n.addSon("eps");
			n.anons = 1;
			n.flage = 1;
		}
		else {
			if (n.anons == 0) {
				if (lexeme == "+") {
					n.addSon("+");
					n.addSon("SimpleExpr");
					n.addSon("Expr1");
				}
				else if (lexeme == "-") {
					n.addSon("-");
					n.addSon("SimpleExpr");
					n.addSon("Expr1");
				}
				n.anons = 1;
			}
			if (n.size > 0 && n[0].flage == 0) {
				n[0].flage = 1;
			}
			else if (n.size > 0 && n[1].flage == 0 && n[0].flage == 1) {
				SimpleExpr(n[1]);
			}
			else if (n.size > 0 && n[2].flage == 0 && n[1].flage == 1) {
				Expr1(n[2]);
				if (n[2].flage == 1) {
					n.flage = 1;
				}
			}
			else error();
		}
	}

	void SimpleExpr(Node& n)
	{
		if (n.anons == 0) {
			if (lexeme == "(") {
				n.addSon("(");
				n.addSon("Expr");
				n.addSon(")");
				n.anons = 1;
			}
			else if (type_lexeme == "int_num") {
				n.addSon("Const");
				n.anons = 1;
			}
			else if (type_lexeme == "id_name") {
				n.addSon("Id");
				n.anons = 1;
			}
			else error();
		}
		if (n[0].data == "(") {
			if (n[0].flage == 0 && lexeme == "(") {
				n[0].flage = 1;
			}
			else if (n[1].flage == 0 && (lexeme == "+" || lexeme == "-" || lexeme == "(" || type_lexeme == "int_num" || type_lexeme == "id_name") && n[0].flage == 1) {
				Expr(n[1]);
			}
			else if (n[2].flage == 0 && lexeme == ")") {
				Expr(n[1]);
				n[2].flage = 1;
				n.flage = 1;
			}
			else error();
		}
		else if (n[0].data == "Const") {
			Const(n[0]);
			if (n[0].flage == 1) {
				n.flage = 1;
			}
		}
		else if (n[0].data == "Id") {
			Id(n[0]);
			if (n[0].flage == 1) {
				n.flage = 1;
			}
		}
		else error();
	}

	void Condition(Node& n)
	{
		bool f = true;
		if (n.anons == 0) {
			n.addSon("Expr");
			n.addSon("RelationOperator");
			n.addSon("Expr");
			n.anons = 1;
		}
		if (n[0].flage == 0) {
			Expr(n[0]);
			f = 0;
		}
		if (n[1].flage == 0 && n[0].flage == 1) {
			RelationOperator(n[1]);
			f = 0;
		}
		else if (n[2].flage == 0 && n[1].flage == 1) {
			Expr(n[2]);
			if (n[2].flage == 1) {
				n.flage = 1;
			}
			f = 0;
		}
		if(f) error();
	}

	void RelationOperator(Node& n)
	{
		if (lexeme == "=" || lexeme == "<>" || lexeme == ">" || lexeme == "<") {
			n.addSon(lexeme);
			n[0].flage = 1;
			n.flage = 1;
		}
		else {
			error();
		}
	}

	void Id(Node& n)
	{
		if (type_lexeme == "id_name") {
			n.addSon(lexeme);
			n[0].flage = 1;
			n.flage = 1;
		}
		else {
			error();
		}
	}

	void Const(Node& n)
	{
		if (type_lexeme == "int_num") {
			n.addSon(lexeme);
			n[0].flage = 1;
			n.flage = 1;
		}
		else {
			error();
		}
	}


};
