#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <unordered_map>
#include "dataStructures.h"
#include "parser.h"

using namespace std;

// generate output filename
static string getOutputFilename(const string& inputFilename, const string& dotEnding) {
    size_t dotPos = inputFilename.find_last_of('.'); // finds file extension delimiter
    if (dotPos != string::npos) { // if filename HAS a dot
        return inputFilename.substr(0, dotPos) + dotEnding; // get filename without dot or extension
    }
    return inputFilename + dotEnding;
}

// pass2
void pass2(const string& sourceFile, int addressToLoadAt) {
    string label, opcode, operand;
    int currentLocation = addressToLoadAt;
    int currentInstrLocation; // address of current instruction
    
    bool baseFlag = false;
    int baseReg = 0;
    
    // open source file to read later
    ifstream fin(sourceFile);
    if (!fin) {
        cerr << "Sorry, failed to open  " << sourceFile << endl;
        return;
    }
    
    // initialize listing file
    string listingFile = getOutputFilename(sourceFile, ".l");
    ofstream lstOut(listingFile);
    if (!lstOut) {
        cerr << "Sorry, couldn't make the listing file " << listingFile << endl;
        fin.close();
        return;
    }
    
    string line;
    bool startFound = false;
    
    // PARSE source file
    while (getline(fin, line)) {
        //comments, empty lines case handling
        if (!parseLine(line, label, opcode, operand)) {
            lstOut << "        " << line << "\n";
            continue;
        }
        
        currentInstrLocation = currentLocation;
        string objCode = "";
        
        // handle START
        if (opcode == "START" && !startFound) {
            lstOut << uppercase << hex << setfill('0') << setw(4) << currentInstrLocation << "      "
                   << setw(8) << left << label 
                   << setw(8) << left << opcode 
                   << operand << "\n";
            startFound = true;
            continue;
        }
        
        // BASE
        if (opcode == "BASE") {
            if (SYMTAB.count(operand)) {
                baseFlag = true;
                baseReg = SYMTAB[operand];
            }
            lstOut << uppercase << hex << setfill('0') << setw(4) << currentInstrLocation << "      "
                   << setw(8) << left << label 
                   << setw(8) << left << opcode 
                   << operand << "\n";
            continue;
        }
        
        // NOBASE
        if (opcode == "NOBASE") {
            baseFlag = false;
            lstOut << uppercase << hex << setfill('0') << setw(4) << currentInstrLocation << "      "
                   << setw(8) << left << label 
                   << setw(8) << left << opcode << "\n";
            continue;
        }
        
        // WORD
        if (opcode == "WORD") {
            int value = stoi(operand);
            stringstream s;
            s << uppercase << hex << setfill('0') << setw(6) << value;
            objCode = s.str();
            currentLocation += 3;
        }
        
        // BYTE
        else if (opcode == "BYTE") {
            if (operand[0] == 'C' && operand[1] == '\'' && operand[operand.length()-1] == '\'') {
                string inside = operand.substr(2, operand.length() - 3);
                stringstream s;
                s << uppercase << hex << setfill('0');
                for (char c : inside) {
                    s << setw(2) << (unsigned int)(unsigned char)c;
                }
                objCode = s.str();
                currentLocation += inside.length();
            } else if (operand[0] == 'X' && operand[1] == '\'' && operand[operand.length()-1] == '\'') {
                objCode = operand.substr(2, operand.length() - 3);
                currentLocation += objCode.length() / 2;
            }
        }
        
        // RESW
        else if (opcode == "RESW") {
            int count = stoi(operand);
            currentLocation += 3 * count;
            // No object code for RESW
        }
        // RESB
        else if (opcode == "RESB") {
            int count = stoi(operand);
            currentLocation += count;
            // No object code for RESB
        }
        
        // LTORG
        else if (opcode == "LTORG") {
            lstOut << uppercase << hex << setfill('0') << setw(4) << currentInstrLocation << "      "
                   << setw(8) << left << label 
                   << setw(8) << left << opcode << "\n";
            // process literals from pass1 and output to listing file
            for (auto &lit : LITTAB) {
                if (lit.second.address >= currentLocation) {
                    lstOut << uppercase << hex << setfill('0') << setw(4) << lit.second.address << "  "
                           << setfill(' ') << setw(10) << left << lit.second.val << "  "
                           << "*" << lit.second.label << "\n";
                    // update curr location after processing literal
                    if (lit.second.address + lit.second.len > currentLocation) {
                        currentLocation = lit.second.address + lit.second.len;
                    }
                }
            }
            continue;
        }
        
        // handle END
        else if (opcode == "END") {
            // handle literals that haven't been processed yet
            for (auto &lit : LITTAB) {
                if (lit.second.address >= currentLocation) {
                    lstOut << uppercase << hex << setfill('0') << setw(4) << lit.second.address << "  "
                           << setfill(' ') << setw(10) << left << lit.second.val << "  "
                           << "*" << lit.second.label << "\n";
                }
            }
            lstOut << "        "  // if no address
                   << setw(8) << left << label 
                   << setw(8) << left << opcode 
                   << operand << "\n";
            break;
        }
        
        // instructions handling
        else if (OPTAB.count(opcode) || (opcode[0] == '+' && OPTAB.count(opcode.substr(1)))) {
            bool format4 = (opcode[0] == '+');
            string op = format4 ? opcode.substr(1) : opcode;
            
            int opValue = OPTAB[op].opcode;
            int fmt = OPTAB[op].format;
            if (format4) fmt = 4;
            
            // format 1 instructions
            if (fmt == 1) {
                stringstream s;
                s << uppercase << hex << setfill('0') << setw(2) << opValue;
                objCode = s.str();
                currentLocation += 1;
            }
            
            
            // format 3 and 4 instructions
            else if (fmt == 3 || fmt == 4) {
                bool n = 1, i = 1, x = 0, b = 0, p = 0, e = format4;
                
                string operand2 = operand;
                int TA = 0;
                bool isConst = false;
                
                // ADDRESSING MODES case handling //

                // IMMEDIATE @ing mode
                if (!operand2.empty() && operand2[0] == '#') { 
                    n = 0;
                    i = 1;
                    operand2 = operand2.substr(1);
                }
                // INDIRECT @ing mode
                if (!operand2.empty() && operand2[0] == '@') {
                    n = 1;
                    i = 0;
                    operand2 = operand2.substr(1);
                }
                
                // INDEXED @ing mode
                if (operand2.find(",X") != string::npos) {
                    x = 1;
                    operand2 = operand2.substr(0, operand2.find(","));
                }
                
                // calculate target address
                if (!operand2.empty() && isdigit(operand2[0])) {
                    isConst = true;
                    TA = stoi(operand2);
                } else if (!operand2.empty() && operand2[0] == '=') {
                    // if operand is a literal
                    if (LITTAB.count(operand2)) {
                        TA = LITTAB[operand2].address;
                    } else {
                        cerr << "Error: Undefined literal " << operand2 << endl;
                    }
                } else if (!operand2.empty()) {
                    // if operand is a symbol
                    if (SYMTAB.count(operand2)) {
                        TA = SYMTAB[operand2];
                    } else {
                        cerr << "Error: Undefined symbol " << operand2 << endl;
                    }
                }
                
                // initialize displacement
                int disp = 0;
                int PC = currentLocation + (fmt == 3 ? 3 : 4);
                
                if (fmt == 4) {
                    // format 4, use TA
                    disp = TA;
                } else {
                    // format 3, determine displacement amount
                    if (isConst) {
                        disp = TA;
                    } else {
                        // PC relative case handling
                        disp = TA - PC;
                        if (disp >= -2048 && disp <= 2047) {
                            p = 1;
                        } else {
                            // BASE RELATIVE case handling
                            if (baseFlag) {
                                disp = TA - baseReg;
                                if (disp >= 0 && disp <= 4095) {
                                    b = 1;
                                } else {
                                    cerr << "Sorry, couldn't calculate displacement for base-relative addressing " << operand2 << endl;
                                }
                            } else {
                                cerr << "Sorry, couldn't calculate displacement for pc-relative addressing " << operand2 << endl;
                            }
                        }
                    }
                }
                
                int ni = (n << 1) | i;
                int xbpe = (x << 3) | (b << 2) | (p << 1) | e;
                
                stringstream s;
                if (fmt == 3) {
                    s << uppercase << hex << setfill('0') 
                      << setw(2) << (opValue | ni)
                      << setw(2) << xbpe
                      << setw(3) << (disp & 0xFFF);
                } else if (fmt == 4) {
                    s << uppercase << hex << setfill('0')
                      << setw(2) << (opValue | ni)
                      << setw(2) << xbpe
                      << setw(5) << TA;
                }
                
                objCode = s.str();
                currentLocation += (fmt == 3 ? 3 : 4);
            }
        }
        
        // write LISTING file (Yippee!!)
        lstOut << uppercase << hex << setfill('0') << setw(4) << currentInstrLocation << "  "
               << setfill(' ') << setw(10) << left << objCode << "  "
               << setw(8) << left << label 
               << setw(8) << left << opcode 
               << operand << "\n";
    }
    
    fin.close();
    lstOut.close();
    cout << "Successfully generated listing file " << listingFile << endl;
}
