#include "dataStructures.h"

unordered_map<string, Literal> LITTAB;
unordered_map<string, int> SYMTAB;
unordered_map<string, OpTabRow> OPTAB;
int LOCCTR = 0;


// OPTAB LOGIC //

void initOPTAB() { // call before pass1

    // OpTab Entries //
    OPTAB["LDA"] = {0x00, 3};
    OPTAB["STA"] = {0x0C, 3};
    OPTAB["LDB"] = {0x68, 3};
    OPTAB["STB"] = {0x78, 3};
    OPTAB["LDF"] = {0x70, 3};
    OPTAB["STF"] = {0x80, 3};
    OPTAB["LDL"] = {0x08, 3};
    OPTAB["STL"] = {0x14, 3};
    OPTAB["LDS"] = {0x6C, 3};
    OPTAB["STS"] = {0x7C, 3};
    OPTAB["LDT"] = {0x74, 3};
    OPTAB["STT"] = {0x84, 3};
    OPTAB["LDX"] = {0x04, 3};
    OPTAB["STX"] = {0x10, 3};
    OPTAB["LDCH"] = {0x50, 3};
    OPTAB["STCH"] = {0x54, 3};
    
    OPTAB["ADD"] = {0x18, 3};
    OPTAB["ADDR"] = {0x90, 2};
    OPTAB["SUB"] = {0x1C, 3};
    OPTAB["SUBR"] = {0x94, 2};
    OPTAB["MUL"] = {0x20, 3};
    OPTAB["MULR"] = {0x98, 2};
    OPTAB["DIV"] = {0x24, 3};
    OPTAB["DIVR"] = {0x9C, 2};
    
    OPTAB["COMP"] = {0x28, 3};
    OPTAB["COMPR"] = {0xA0, 2};
    OPTAB["TIX"] = {0x2C, 3};
    OPTAB["TIXR"] = {0xB8, 2};
    OPTAB["J"] = {0x3C, 3};
    OPTAB["JSUB"] = {0x48, 3};
    OPTAB["RSUB"] = {0x4C, 3};
    OPTAB["JGT"] = {0x34, 3};
    OPTAB["JLT"] = {0x38, 3};
    OPTAB["JEQ"] = {0x30, 3};
    
    OPTAB["TD"] = {0xE0, 3};
    OPTAB["RD"] = {0xD8, 3};
    OPTAB["WD"] = {0xDC, 3};
    
    OPTAB["CLEAR"] = {0xB4, 2};
}



