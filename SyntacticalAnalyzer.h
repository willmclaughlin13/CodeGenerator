//
// Created by Will on 12/5/2019.
//

#ifndef CODEGENERATOR_SYNTACTICALANALYZER_H
#define CODEGENERATOR_SYNTACTICALANALYZER_H

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
    string cppString;
    int program ();
    int more_defines ();
    int define ();
    int stmt_list ();
    int stmt ();
    int literal ();
    int quoted_lit ();
    int more_tokens ();
    int param_list ();
    int else_part ();
    int stmt_pair ();
    int stmt_pair_body ();
    int action ();
    int any_other_token ();
    ofstream listing;
    ofstream p2;
    ofstream debug;
};

#endif


#endif //CODEGENERATOR_SYNTACTICALANALYZER_H
