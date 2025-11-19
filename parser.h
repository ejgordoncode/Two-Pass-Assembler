#ifndef PARSER_H
#define PARSER_H


// Necessary Includes //
#include <string>
#include <fstream>

using namespace std;

/* FUNCTIONS */

/** Tokenize a line of the input file 
    @param line the line of the input file to tokenize
    @return  a vector of tokens containing the label, opcode, and value*/
vector<string> tokenize(string& line);

/** Parse a line of the input file 
    @param line the line of the input file to parse
    @param label the label of the line (output)
    @param opcode the opcode of the line (output)
    @param value the value of the line (output)
    @return true if the line gets parsed, false otherwise*/
bool parseLine(string& line, string& label, string& opcode, string& operand);

#endif