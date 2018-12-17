#ifndef CG_H
#define CG_H

#include <iostream>
#include <fstream>
#include <stack>
#include "LexicalAnalyzer.h"

using namespace std;

class CodeGen 
{
    public:
	CodeGen (string filename, LexicalAnalyzer * L);
	~CodeGen ();
	void WriteCode (int tabs, string code);
	void WriteNest(int tabs, stack<string> s);
    	bool isOperator(string s);
    private:
	LexicalAnalyzer * lex;
	ofstream cpp;
};
	
#endif
