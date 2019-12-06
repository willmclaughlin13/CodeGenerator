//
// Created by Will on 12/5/2019.
//

#ifndef CODEGENERATOR_LEXICALANALYZER_H
#define CODEGENERATOR_LEXICALANALYZER_H

/*******************************************************************************
* File name: LexicalAnalyzer.h                                                 *
* Project: CS 460 Project 1 - Lexical Analyzer for Scheme to C++ Translator    *
* Author(s): Dr. Watts                                                         *
* Date: Fall 2019                                                              *
* Description: This file contains the enumerated type for token_type and the   *
*		class description of LexicalAnalyzer.                          *
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

/*******************************************************************************
* Type: token_type                                                             *
*                                                                              *
* Description: The token_type enumerated type is used to identify the tokens   *
*              associated with the lexemes scanned from an input file.         *
*******************************************************************************/

enum token_type {NONE = -1, LAMBDA, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, COND_T,
    DISPLAY_T, NEWLINE_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T,
    NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, MODULO_T, ROUND_T,
    ELSE_T, PLUS_T, MINUS_T, DIV_T, MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T,
    LTE_T, LPAREN_T, RPAREN_T, SQUOTE_T, ERROR_T, EOF_T, MAX_TOKENS};

/*******************************************************************************
* Class: Lexical_Analyzer                                                      *
*                                                                              *
* Description: This class is designed to                                       *
*******************************************************************************/

class LexicalAnalyzer
{
public:
    LexicalAnalyzer (char * filename);
    ~LexicalAnalyzer ();
    token_type GetToken ();
    string GetTokenName (token_type t) const;
    string GetLexeme () const;
    void ReportError (const string & msg);
    ofstream debug;
private:
    ifstream input;
    ofstream listing;
    ofstream p1file;
    string line;
    int linenum;
    int pos;
    string lexeme;
    int error_type;
    int errors;
    map <string, token_type> symbols;
};

#endif
