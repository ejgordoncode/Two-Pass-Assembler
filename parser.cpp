// Necessary Includes //
#include "parser.h"
#include <sstream> // for streamlining parsing function
#include <vector>
#include <string>

using namespace std;

vector<string> tokenize(string& line){
    vector<string> tokens;
    string token;
    istringstream lineStream(line);
    while (lineStream >> token){
        tokens.push_back(token);
    }
    return tokens;
}

bool parseLine(string& line, string& label, string& opcode, string& operand){
    vector<string> tokens = tokenize(line);

    // check if line is empty
    if (tokens.empty()) {
        return false;
    }

    // check if line is comment
    if (tokens[0] == ".") {
        return false;
    }

    // Handle parsing based on number of tokens in line
    switch (tokens.size()){
        case 1:
            opcode = tokens[0];
            label = "";
            operand = "";
            break;
        case 2:
            label = "";
            opcode = tokens[0];
            operand = tokens[1];
            break;
        case 3:
            label = tokens[0];
            opcode = tokens[1];
            operand = tokens[2];
            break;
        default:
            return false;
    }
    return true;
}
