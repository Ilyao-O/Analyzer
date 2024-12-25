#pragma once
#include "dfa.h"
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <algorithm>

class nfa
{
private:
	int countStates; // количество состояний автомата
	std::vector<char> alphabet; // входной алфавит
	std::vector<int> finalStates; // множество допускающих состояний
	std::vector<std::vector<std::vector<int>>> transitFunction; // функция переходов
	// transitFunction[i][j] = q означает, что из i-го состояния по j-му символу входного алфавита автомат переходит в множество состояний, который лежат в векторе q
	std::set<int> getStates(std::vector<int> states, char c);
public:
	nfa(int countStates, std::vector<char> alphabet, std::vector<int> finiteStates, std::vector<std::vector<std::vector<int>>> transitFunction);
	bool isAccept(std::string s);
	dfa createDFA();
};

nfa::nfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<std::vector<int>>> transitFunction)
{
	this->countStates = countStates;
	this->alphabet = alphabet;
	this->finalStates = finalStates;
	this->transitFunction.resize(countStates, std::vector<std::vector<int>>(alphabet.size(), std::vector<int>()));
	for (int i = 0; i < countStates; i++)
		for (int j = 0; j < alphabet.size(); j++)
			this->transitFunction[i][j] = transitFunction[i][j];
}


// Находит все состояния, в которые автомат попадает из состояний states по символу c
std::set<int> nfa::getStates(std::vector<int> states, char c)
{
	std::set<int> result;
	int i = 0;
	while (alphabet[i] != c)
	{
		i++;
		if (i == alphabet.size())
			throw std::exception("Wrong symbol");
	}
	for (int s : states)
	{
		auto t = transitFunction[s][i];
		if (t[0] != -1)
			result.insert(transitFunction[s][i].begin(), transitFunction[s][i].end());
	}
	return result;
}

// Проверяет, допускает ли НКА строку s
bool nfa::isAccept(std::string s)
{
	std::vector<int> states;
	states.push_back(0);
	for (char c : s)
	{
		std::set<int> t = getStates(states, c);
		states = std::vector<int>(t.begin(), t.end());
		if (states.size() == 0)
			return false;
	}
	for (int s : states)
		if (std::find(finalStates.begin(), finalStates.end(), s) != finalStates.end())
			return true;
	return false;
}

// Строит эквивалентный ДКА
dfa nfa::createDFA()
{
	std::vector<std::set<int>> states;
	std::queue<std::set<int>> q;
	std::set<int> s;
	std::vector<std::vector<int>> dfaTransit;
	s.insert(0);
	q.push(s);
	states.push_back(s);
	s.clear();
	while (!q.empty())
	{
		std::set<int> sp = q.front();
		q.pop();
		std::vector<int> transit(alphabet.size());
		for (char c : alphabet)
		{
			s = getStates(std::vector<int>(sp.begin(), sp.end()), c);
			int i = 0;
			while (alphabet[i] != c)
				i++;
			if (s.size() == 0)
				transit[i] = -1;
			else
			{
				if (std::find(states.begin(), states.end(), s) == states.end())
				{
					states.push_back(s);
					q.push(s);
				}
				int j;
				for (j = 0; j < states.size(); j++)
					if (states[j] == s)
						break;
				transit[i] = j;
			}
			s.clear();
		}
		dfaTransit.push_back(transit);
	}
	std::vector<int> finalDfa;
	for (std::set<int> s : states)
	{
		for (int f : finalStates)
			if (std::find(s.begin(), s.end(), f) != s.end())
			{
				int j;
				for (j = 0; j < states.size(); j++)
					if (states[j] == s)
						break;
				finalDfa.push_back(j);
				break;
			}
	}
	dfa result(states.size(), alphabet, finalDfa, dfaTransit);
	return result;
}

