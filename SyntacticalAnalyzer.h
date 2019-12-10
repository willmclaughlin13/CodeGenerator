/*******************************************************************************
* File name:                                                                   *
* Project: CS 460 Project 2 Fall 2019                                          *
* Author(s):                                                                   *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "CodeGenerator.h"

using namespace std;

class SyntacticalAnalyzer
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
    CodeGen * code;
	token_type token;
	int program ();
	int more_defines ();
	int define ();
	int stmt_list (bool outerScope);
	int stmt (bool outerScope);
	int literal ();
	int quoted_lit ();
	int more_tokens ();
	int param_list ();
	int else_part ();
	int stmt_pair ();
	int stmt_pair_body ();
	int action (bool outerScope);
	int any_other_token ();
  int parenCount; // This is when we're inside a statement that has
                   // nested statements and we want a comma
  bool closeParen, adding, subtracting, dividing, multiplying, moduloing,
        equaling, GTing, GTEing, LTing, LTEing, rounding, oneParam;
	ofstream listing;
	ofstream p2;
	ofstream debug;
};

#endif
