/*******************************************************************************
* File name: SyntacticalAnalyzer.cpp                                           *
* Project: CS 460 Project 3 Fall 2019                                          *
* Author(s): Patrick Hernandez, William McLaughlin, Sean Miltenberger          *
* Date: 12/02/19                                                               *
* Description: This file contains the definitions for the Syntactical Analyzer.*
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

const static string tokenNames[] = {"LAMBDA", "IDENT_T", "NUMLIT_T", "STRLIT_T", "IF_T", "COND_T",
	"DISPLAY_T", "NEWLINE_T", "LISTOP1_T", "LISTOP2_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T",
	"NUMBERP_T", "LISTP_T", "ZEROP_T", "NULLP_T", "STRINGP_T", "MODULO_T", "ROUND_T",
	"ELSE_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T", "GT_T", "LT_T", "GTE_T",
	"LTE_T", "LPAREN_T", "RPAREN_T", "SQUOTE_T", "ERROR_T", "EOF_T", "MAX_TOKENS"};

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	code = new CodeGen (filename);
	string filenameStr = filename;
	//code = new CodeGen (filenameStr); // This seems like the proper way to do it, but
																			// it won't write.

	int filenameStrLength = filenameStr.length();

	// This if statement checks the ending of the filename for a '.ss'
	if( filenameStr.substr((filenameStrLength - 3), (filenameStrLength - 1)) != ".ss" ) {
		printf ("filename should be in the format: *.ss\n");
		exit (1);
	}

	// This section will create the output files strings '<filename>.lst', '<filename>.p2', and '<filename>.dbg'
	string filenameStrExtFree = filenameStr.substr(0, filenameStrLength - 3);
	string outputFileStrLST = filenameStrExtFree + ".lst";
	string outputFileStrP2 = filenameStrExtFree + ".p2";
	string outputFileStrDBG = filenameStrExtFree + ".dbg";

	// Open the output files
	listing.open(outputFileStrLST.c_str(), std::ios::out);
	p2.open(outputFileStrP2.c_str(), std::ios::out);
	debug.open(outputFileStrDBG.c_str(), std::ios::out);
	if( !listing.is_open() || !p2.is_open() || !debug.is_open()) {
        std::cout << "Unable to open "
			<< outputFileStrLST << ", "
			<< outputFileStrP2  << ", or "
			<< outputFileStrDBG
			<< ". Terminating...";
        exit(2);
    }

	indentation = 0;
	token = lex->GetToken();
	int totalErrors = program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	delete code;
	listing.close();
	p2.close();
	debug.close();
}

int SyntacticalAnalyzer::program ()
{
    p2 << "Entering Program function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	p2 << "Using Rule 1\n";
	if(token == LPAREN_T){
		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError("( expected, got: " + tokenNames[token]);
	}

	errors += define();

	if(token == LPAREN_T){

		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError("( expected, got: " + tokenNames[token]);
	}

	errors += more_defines();


	if(token != EOF_T){
		errors++;
		lex->ReportError("EOF expected, got: " + tokenNames[token]);
	}

	p2 << "Exiting Program function; current token is: " << tokenNames[token] << endl;

	return errors;
}

int SyntacticalAnalyzer::more_defines ()
{
    p2 << "Entering More_Defines function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	if(token == DEFINE_T){
		p2 << "Using Rule 2\n";
		errors += define();

		if(token == LPAREN_T){
			token = lex->GetToken();
		} else {
			errors++;
			lex->ReportError("( expected, got: " + tokenNames[token]);
		}

		errors += more_defines();
	} else if (token == IDENT_T){
		p2 << "Using Rule 3\n";
		token = lex->GetToken();
		errors += stmt_list();

		if(token == RPAREN_T){
			token = lex->GetToken();
		} else {
			errors++;
			lex->ReportError(") expected, got: " + tokenNames[token]);
		}

	} else {
		errors++;
		lex->ReportError("Define or Ident expected, got: " + tokenNames[token]);
	}

	p2 << "Exiting More_Defines function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::define ()
{
	bool mainFunc = false;
    p2 << "Entering Define function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	p2 << "Using Rule 4\n";
	if(token == DEFINE_T){
		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError("define expected, got: " + tokenNames[token]);
	}

	if(token == LPAREN_T){
		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError("( expected,4 got: " + tokenNames[token]);
	}

	if(token == IDENT_T){
        if (lex->GetLexeme() == "main") {
            code->WriteCode(0, "int " + lex->GetLexeme() + " ("); // This is
																	// the edge case where the function is main()
						mainFunc = true;
        } else {
            code->WriteCode(0, "Object " + lex->GetLexeme() + " ("); // Write
																							// Function name
						mainFunc = false;
				}
		token = lex->GetToken();

	} else {
		errors++;
		lex->ReportError("Ident expected, got: " + tokenNames[token]);
	}

	errors += param_list(); // get parameters
	code->WriteCode(0, "{\n"); // Beginning of a new function
	indentation++;
	code->WriteCode(indentation, "Object __RetVal;\n");
	if(token == RPAREN_T){
		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError(") expected, got: " + tokenNames[token]);
	}

	errors += stmt(code->Tabs(indentation), ";\n", true);

	// This is at the end of a statement line.
	errors += stmt_list(code->Tabs(indentation), ";\n", false, true);

	if(token == RPAREN_T){
		if (mainFunc)
			code->WriteCode(indentation, "return 0;\n"); // When it's main, we return int
		else
        	code->WriteCode(indentation, "return __RetVal;\n"); // Otherwise, return __RetVal
		indentation--;
        code->WriteCode(0, "}\n\n"); // Close function
		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError(") expected, got: " + tokenNames[token]);
	}

	p2 << "Exiting Define function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::stmt_list (string prefix, string suffix, bool skipFirst, bool retVal)
{
    p2 << "Entering Stmt_List function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	if(token == IDENT_T || token == LPAREN_T || token == NUMLIT_T ||
	token == STRLIT_T || token == SQUOTE_T){
		p2 << "Using Rule 5\n";
		if(skipFirst)
			errors += stmt("", suffix);
		else
			errors += stmt(prefix, suffix);
		errors += stmt_list(prefix, suffix);
	} else if (token == RPAREN_T){
		//cout << "\n Cleared Paren count!\n\n";
		//parenCount = 0;
		p2 << "Using Rule 6\n";
	} else {
		errors++;
		lex->ReportError("ident, (, numlit, strlit, `, or ) expected" + tokenNames[token]);
	}

	p2 << "Exiting Stmt_List function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::stmt (string prefix, string suffix, bool retVal)
{
    p2 << "Entering Stmt function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	code->WriteCode(0, prefix);

    if(token == IDENT_T){
		p2 << "Using Rule 8\n";
		if(retVal)
			code->WriteCode(0, "__RetVal = ");
		code->WriteCode(0, lex->GetLexeme());
		token = lex->GetToken();
	} else if(token == SQUOTE_T || token == NUMLIT_T || token == STRLIT_T){
		p2 << "Using Rule 7\n";
		if(retVal)
			code->WriteCode(0, "__RetVal = ");
		errors += literal();

	} else if(token == LPAREN_T){
		p2 << "Using Rule 9\n";
		token = lex->GetToken();

		errors += action(prefix, "", retVal);
		if(token == RPAREN_T){
			token = lex->GetToken();

		} else {
			errors++;
			lex->ReportError(") expected, got: " + tokenNames[token]);
		}
	} else {
		errors++;
		lex->ReportError("ident, (, numlit, or strlit expected, got: " + tokenNames[token]);
	}

	code->WriteCode(0, suffix);

	p2 << "Exiting Stmt function; current token is: " << tokenNames[token] << endl;
	return errors;
}


int SyntacticalAnalyzer::literal ()
{
    p2 << "Entering Literal function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;


	if(token == NUMLIT_T){
		code->WriteCode(0, "Object(" + lex->GetLexeme() + ") "); // numberino
		p2 << "Using Rule 10\n";
		token = lex->GetToken();
	}
	else if(token == STRLIT_T){
		code->WriteCode(0, "Object(" + lex->GetLexeme() + ") "); // string
		p2 << "Using Rule 11\n";
		token = lex->GetToken();
	}
	else if(token == SQUOTE_T){
		code->WriteCode(0, "Object(");
		p2 << "Using Rule 12\n";
		token = lex->GetToken();
		errors += quoted_lit();
		code->WriteCode(0, ") ");
		//code->WriteCode(0, ")");
	}
	else{
		errors++;
		lex->ReportError("Expected a literal value, got: " + tokenNames[token]);
	}

	p2 << "Exiting Literal function; current token is: " << tokenNames[token] << endl;
	return errors;
}


int SyntacticalAnalyzer::quoted_lit ()
{
    p2 << "Entering Quoted_Lit function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;
	if(token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T ||
	token == LISTOP2_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T ||
	token == LISTOP1_T || token == AND_T || token == OR_T || token == NOT_T ||
	token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T ||
	token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T ||
	token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T ||
	token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T ||
	token == LTE_T || token == SQUOTE_T || token == COND_T || token == ELSE_T){
		p2 << "Using Rule 13\n";
		code->WriteCode(0, "\"");
		errors += any_other_token();
		code->WriteCode(0, "\"");
	} else {
		errors++;
		lex->ReportError("Quoted lit expected, got: " + tokenNames[token]);
	}


	p2 << "Exiting Quoted_Lit function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
    p2 << "Entering More_Tokens function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;
	if(token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T ||
	token == LISTOP2_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T ||
	token == LISTOP1_T || token == AND_T || token == OR_T || token == NOT_T ||
	token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T ||
	token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T ||
	token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T ||
	token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T ||
	token == LTE_T || token == SQUOTE_T || token == COND_T || token == ELSE_T){
		p2 << "Using Rule 14\n";
		if (token == LPAREN_T) { // Balance parenthesis!
			//parenCount++;
			//p2 << "++ inside more_tokens!\n";
		}

		errors += any_other_token();
		code->WriteCode(0, " "); // Drop a space between elements of a list
		errors += more_tokens();
	} else if(token == RPAREN_T){
		code->WriteCode(0, ")"); // End of every listop?
		p2 << "-- inside more_tokens!\n";
		/*if (parenCount == 0)
			code->WriteCode(0, "); \n"); // When the parentheses are balanced, we done
		else {
			code->WriteCode(0, ", \n"); // When the are not balanced, little comma
			code->WriteCode(4, ""); // Big indent
		}*/

		p2 << "Using Rule 15\n";
	} else {
		errors++;
		lex->ReportError("Quoted lit expected, got: " + tokenNames[token]);
	}

	p2 << "Exiting More_Tokens function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::param_list ()
{
    p2 << "Entering Param_List function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	if(token == IDENT_T){
		p2 << "Using Rule 16\n";
		code->WriteCode(0, "Object " + lex->GetLexeme());
		token = lex->GetToken();
		if(token == IDENT_T)
			code->WriteCode(0, ", ");
		errors += param_list();
	} else if(token == RPAREN_T){
		code->WriteCode(0, ")\n");
		p2 << "Using Rule 17\n";
	} else {
		errors++;
		lex->ReportError("ident or ) expected, got: " + tokenNames[token]);
	}

	p2 << "Exiting Param_List function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::else_part ()
{
    p2 << "Entering Else_Part function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

	if(token == IDENT_T || token == LPAREN_T || token == NUMLIT_T ||
	token == STRLIT_T || token == SQUOTE_T){
		p2 << "Using Rule 18\n";
		code->WriteCode(indentation, "else {\n");
		indentation++;
		errors += stmt(code->Tabs(indentation), ";\n", true);
		indentation--;
		code->WriteCode(indentation, "}\n");
	} else if (token == RPAREN_T){
		p2 << "Using Rule 19\n";
	} else {
		errors++;
		lex->ReportError("ident, (, numlit, strlit, `, or ) expected, got: " + tokenNames[token]);
	}
	p2 << "Exiting Else_Part function; current token is: " << tokenNames[token] << endl;
	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
    p2 << "Entering Stmt_Pair function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;

    if (token == LPAREN_T){
        p2 << "Using Rule 20\n";
        token = lex->GetToken();
		code->WriteCode(indentation, "else ");
        errors += stmt_pair_body();
    } else if (token == RPAREN_T){ // Lamda
        p2 << "Using Rule 21\n";
    } else {
        errors++;
		lex->ReportError("( or ) expected, got: " + tokenNames[token]);
    }
	p2 << "Exiting Stmt_Pair function; current token is: " << tokenNames[token] << endl;
	return errors;
}


int SyntacticalAnalyzer::stmt_pair_body ()
{
    p2 << "Entering Stmt_Pair_Body function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
    int errors = 0;

	if(token == ELSE_T){
        //<stmt_pair_body> -> ELSE_T <stmt> RPAREN_T
		p2 << "Using Rule 23\n";
        token = lex->GetToken();
		code->WriteCode(0, "{\n");
		indentation++;
        errors += stmt(code->Tabs(indentation), ";\n", true);
		indentation--;
		code->WriteCode(indentation, "}\n");

        if(token == RPAREN_T){
		    token = lex->GetToken();
        } else {
            errors++;
            lex->ReportError(") expected");
        }

	} else if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T ||
    token == STRLIT_T || token == SQUOTE_T){
        p2 << "Using Rule 22\n";
        //<stmt_pair_body> -> <stmt> <stmt> RPAREN_T <stmt_pair>
        errors += stmt("if(", "){\n");
		indentation++;
        errors += stmt(code->Tabs(indentation), ";\n", true);
		indentation--;
		code->WriteCode(indentation, "}\n");

        if(token == RPAREN_T){
		    token = lex->GetToken();
        } else {
            errors++;
            lex->ReportError(") expected");
        }

        errors += stmt_pair();
	} else {
		errors++;
		lex->ReportError("Else, Ident, (, Numlit, Strlit, or ` expected, got: " + tokenNames[token]);
	}
	p2 << "Exiting Stmt_Pair_Body function; current token is: " << tokenNames[token] << endl;
    return errors;
}


int SyntacticalAnalyzer::action (string prefix, string suffix, bool retVal)
{
    p2 << "Entering Action function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
	int errors = 0;


	if(token != NEWLINE_T && token != DISPLAY_T &&
	 	token != IF_T && token != COND_T && retVal)
		code->WriteCode(0, "__RetVal = ");

    if(token == IF_T) {
		p2 << "Using Rule 24\n";
        token = lex->GetToken();
        errors += stmt("if(", "){\n");
		indentation++;
        errors += stmt(code->Tabs(indentation),";\n", true);
		indentation--;
		code->WriteCode(indentation, "}\n");
        errors += else_part();

    } else if(token == COND_T) {
        p2 << "Using Rule 25\n";
        token = lex->GetToken();

        if (token == LPAREN_T) {
            token = lex->GetToken();
            errors += stmt_pair_body();
        } else {
            errors++;
            lex->ReportError("( expected");
        }

    } else if(token == LISTOP1_T){
				//code->WriteCode(0, "listop "); // cons, car, cddr, muah
        p2 << "Using Rule 26\n";
        code->WriteCode(0, "listop(\"" + lex->GetLexeme() + "\", ");
        token = lex->GetToken();
        errors += stmt();
		code->WriteCode(0, ")");

    } else if(token == LISTOP2_T){
        p2 << "Using Rule 27\n";
        code->WriteCode(0, "listop(\"" + lex->GetLexeme() + "\", ");
        token = lex->GetToken();
        errors += stmt();
		code->WriteCode(0, ", ");
        errors += stmt();
		code->WriteCode(0, ")");

    } else if(token == AND_T){
        p2 << "Using Rule 28\n";
        token = lex->GetToken();
		code->WriteCode(0, "(");
        errors += stmt_list(" && ", "", true);
		code->WriteCode(0, ")");

    } else if(token == OR_T){
        p2 << "Using Rule 29\n";
        token = lex->GetToken();
		code->WriteCode(0, "(");
        errors += stmt_list(" || ", "", true);
		code->WriteCode(0, ")");

    } else if(token == NOT_T){
        p2 << "Using Rule 30\n";
        token = lex->GetToken();
        errors += stmt("!(", ")");

    } else if(token == NUMBERP_T){
        p2 << "Using Rule 31\n";
        token = lex->GetToken();
        errors += stmt("numberp(", ")");

    } else if(token == LISTP_T){
        p2 << "Using Rule 32\n";
        token = lex->GetToken();
        errors += stmt("listp(", ")");

    } else if(token == ZEROP_T){
        p2 << "Using Rule 33\n";
        token = lex->GetToken();
        errors += stmt("zerop(", ")");

    } else if(token == NULLP_T){
        p2 << "Using Rule 34\n";
        token = lex->GetToken();
        errors += stmt("nullp(", ")");

    } else if(token == STRINGP_T){
        p2 << "Using Rule 35\n";
        token = lex->GetToken();
        errors += stmt("stringp(", ")");

    } else if(token == PLUS_T){
        p2 << "Using Rule 36\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list(" + ", "", true);
				code->WriteCode(0, ")");

    } else if(token == MINUS_T){
        p2 << "Using Rule 37\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt();
        errors += stmt_list(" - ");
				code->WriteCode(0, ")");

    } else if(token == DIV_T){
        p2 << "Using Rule 38\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt();
        errors += stmt_list(" / ");
				code->WriteCode(0, ")");

    } else if(token == MULT_T){
        p2 << "Using Rule 39\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list(" * ", "", true);
				code->WriteCode(0, ")");

    } else if(token == MODULO_T){
        p2 << "Using Rule 40\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt();
        errors += stmt(" % ");
				code->WriteCode(0, ")");

	} else if(token == ROUND_T){
        p2 << "Using Rule 41\n";
        token = lex->GetToken();
        errors += stmt("round(", ")");

    } else if(token == EQUALTO_T){
        p2 << "Using Rule 42\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list(" == ", "", true);
				code->WriteCode(0, ")");

    } else if(token == GT_T){
        p2 << "Using Rule 43\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list(" > ", "", true);
				code->WriteCode(0, ")");

    } else if(token == LT_T){
        p2 << "Using Rule 44\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list(" < ", "", true);
				code->WriteCode(0, ")");

    } else if(token == GTE_T){
        p2 << "Using Rule 45\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list(" >= ", "", true);
				code->WriteCode(0, ")");

    } else if(token == LTE_T){
        p2 << "Using Rule 46\n";
				code->WriteCode(0, "(");
        token = lex->GetToken();
        errors += stmt_list();
				code->WriteCode(0, ")");

    } else if(token == IDENT_T){
        p2 << "Using Rule 47\n";
				code->WriteCode(0, lex->GetLexeme() + "("); // These are the variables from cout
        token = lex->GetToken();
        errors += stmt_list(", ", "", true);
		code->WriteCode(0, ")");

    } else if(token == DISPLAY_T){
        p2 << "Using Rule 48\n";
				code->WriteCode(0, "cout << "); // This is when we cout
        token = lex->GetToken();
        errors += stmt();
				//code->WriteCode(0, ")");

    } else if(token == NEWLINE_T){
        p2 << "Using Rule 49\n";
				code->WriteCode(0, "cout << endl"); // This is when we cout a newline
        token = lex->GetToken();

    } else {
        errors++;
        lex->ReportError("Action expected, got: " + tokenNames[token]);
    }

	p2 << "Exiting Action function; current token is: " << tokenNames[token] << endl;
	return errors;
}


int SyntacticalAnalyzer::any_other_token ()
{
    p2 << "Entering Any_Other_Token function; current token is: " << tokenNames[token] << ", lexeme: " << lex->GetLexeme() << endl;;
    int errors = 0;

    if (token == LPAREN_T){
        p2 << "Using Rule 50\n";
				p2 << "++ inside any_other_token!\n";
				code->WriteCode(0, lex->GetLexeme());
        token = lex->GetToken();
        errors += more_tokens();

        if(token == RPAREN_T){
            token = lex->GetToken();
        } else {
            errors++;
            lex->ReportError(") expected");
        }

    } else if(token == IDENT_T){
				code->WriteCode(0, lex->GetLexeme()); // I think this prints the variables when we are in a statement
        p2 << "Using Rule 51\n";
        token = lex->GetToken();

    } else if(token == NUMLIT_T){
				code->WriteCode(0, lex->GetLexeme()); // I think this prints the variables when we are in a statement
        p2 << "Using Rule 52\n";
        token = lex->GetToken();

    } else if(token == STRLIT_T){
				code->WriteCode(0, lex->GetLexeme()); // I think this prints the variables when we are in a statement
        p2 << "Using Rule 53\n";
        token = lex->GetToken();

    } else if(token == LISTOP1_T){
        p2 << "Using Rule 54\n";
        token = lex->GetToken();

    } else if(token == IF_T){
        p2 << "Using Rule 55\n";
        token = lex->GetToken();

    } else if(token == DISPLAY_T){
        p2 << "Using Rule 56\n";
        token = lex->GetToken();

    } else if(token == NEWLINE_T){
		code->WriteCode(1, "cout << endl"); // This is also when we cout a newline
		p2 << "Using Rule 57\n";
        token = lex->GetToken();

    } else if(token == LISTOP2_T){
        p2 << "Using Rule 58\n";
        token = lex->GetToken();

    } else if(token == AND_T){
        p2 << "Using Rule 59\n";
        token = lex->GetToken();

    } else if(token == OR_T){
        p2 << "Using Rule 60\n";
        token = lex->GetToken();

    } else if(token == NOT_T){
        p2 << "Using Rule 61\n";
        token = lex->GetToken();

    } else if(token == DEFINE_T){
        p2 << "Using Rule 62\n";
        token = lex->GetToken();

    } else if(token == NUMBERP_T){
        p2 << "Using Rule 63\n";
        token = lex->GetToken();

    } else if(token == LISTP_T){
        p2 << "Using Rule 64\n";
        token = lex->GetToken();

    } else if(token == ZEROP_T){
        p2 << "Using Rule 65\n";
        token = lex->GetToken();

    } else if(token == NULLP_T){
        p2 << "Using Rule 66\n";
        token = lex->GetToken();

    } else if(token == STRINGP_T){
        p2 << "Using Rule 67\n";
        token = lex->GetToken();

    } else if(token == PLUS_T){
        p2 << "Using Rule 68\n";
        token = lex->GetToken();

    } else if(token == MINUS_T){
        p2 << "Using Rule 69\n";
        token = lex->GetToken();

    } else if(token == DIV_T){
        p2 << "Using Rule 70\n";
        token = lex->GetToken();

    } else if(token == MULT_T){
        p2 << "Using Rule 71\n";
        token = lex->GetToken();

    } else if(token == MODULO_T){
        p2 << "Using Rule 72\n";
        token = lex->GetToken();

    } else if(token == ROUND_T){
        p2 << "Using Rule 73\n";
        token = lex->GetToken();

    } else if(token == EQUALTO_T){
        p2 << "Using Rule 74\n";
        token = lex->GetToken();

    } else if(token == GT_T){
        p2 << "Using Rule 75\n";
        token = lex->GetToken();

    } else if(token == LT_T) {
        p2 << "Using Rule 76\n";
        token = lex->GetToken();

	} else if(token == GTE_T){
        p2 << "Using Rule 77\n";
        token = lex->GetToken();

    } else if(token == LTE_T){
        p2 << "Using Rule 78\n";
        token = lex->GetToken();

    } else if(token == SQUOTE_T){
        p2 << "Using Rule 79\n";
        token = lex->GetToken();
        errors += any_other_token();

    } else if(token == COND_T){
        p2 << "Using Rule 80\n";
        token = lex->GetToken();

    } else if(token == ELSE_T){
        p2 << "Using Rule 81\n";
        token = lex->GetToken();

    } else {
        errors++;
        lex->ReportError("Any other token expected, got: " + tokenNames[token]);
    }
	p2 << "Exiting Any_Other_Token function; current token is: " << tokenNames[token] << endl;
    return errors;
}
