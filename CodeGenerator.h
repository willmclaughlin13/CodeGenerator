//
// Created by Will on 12/5/2019.
//

#ifndef CODEGENERATOR_CODEGENERATOR_H
#define CODEGENERATOR_CODEGENERATOR_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class CodeGen
{
public:
    //CodeGen (string filename, LexicalAnalyzer * L);
    CodeGen (string filename);
    ~CodeGen ();
    void WriteCode (int tabs, string code);
    string Tabs(int tabs);
private:
    ofstream cpp;
};

#endif
