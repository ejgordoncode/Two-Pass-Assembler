#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <string>
#include <iomanip>
#include "dataStructures.h"
#include "parser.h"

using namespace std;

// directives
unordered_set<string> DIRECTIVES = {
    "START", "END", "WORD", "RESW", "RESB", "BYTE", "LTORG", "BASE", "NOBASE"
};

// assign address to literal
void processLiterals() {
    for (auto &lit : LITTAB) {
        if (lit.second.address == -1) { // unassigned case
            lit.second.address = LOCCTR; // assign address
            LOCCTR += lit.second.len;
        }
    }
}

// Helper function to generate output filename
string getOutputFilename(const string& inputFile, const string& extension) {
    size_t dotPos = inputFile.find_last_of('.');
    if (dotPos != string::npos) {
        return inputFile.substr(0, dotPos) + extension;
    }
    return inputFile + extension;
}

// Helper function to write SYMTAB to .st file
void writeSYMTABFile(const string& filename) {
    ofstream stOut(filename);
    if (!stOut) {
        cerr << "Error: Could not create .st file: " << filename << endl;
        return;
    }
    
    stOut << "SYMBOL TABLE\n";
    stOut << "------------\n";
    for (auto &s : SYMTAB) {
        stOut << setw(10) << s.first << " : " << hex << uppercase 
              << setfill('0') << setw(4) << s.second << endl;
    }
    
    stOut.close();
    cout << "Generated: " << filename << endl;
}


//MAIN PASS1!!!!

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "no input present, closing.\n";
        return 1;
    }

    // Initialize OPTAB
    initOPTAB();

    for (int f = 1; f < argc; f++) {
        string file = argv[f];
        ifstream fin(file);
        if (!fin) {
            cerr << "Sorry, unable to open " << file << endl;
            continue;
        }

        cout << "Loading " << file << endl;

        // Clear SYMTAB and LITTAB for each new file
        SYMTAB.clear();
        LITTAB.clear();

        LOCCTR = 0;
        bool startFound = false;

        string line, label, opcode, operand;

        while (getline(fin, line)) {
            if (!parseLine(line, label, opcode, operand))
                continue;

            //start directive here
            if (opcode == "START" && !startFound) {
                if (operand.empty()) {
                    cerr << "Sorry, START requires a hexadecimal address as an operand" << endl;
                    fin.close();
                    return 1;
                }
                LOCCTR = stoi(operand, nullptr, 16);
                startFound = true;
                continue;
            }

            //symtab label processing
            if (!label.empty()) {
                if (SYMTAB.count(label)) {
                    cerr << "Sorry, duplicate symbol name found for: " << label << endl;
                    return 1;
                }
                SYMTAB[label] = LOCCTR;
            }

            /* =============================
               OPCODE PROCESSING
               ============================= */

            bool format4 = false;

            // handle extended formatting
            if (opcode[0] == '+') {
                opcode = opcode.substr(1);
                format4 = true;
            }

            if (OPTAB.count(opcode)) {
                if (format4) {
                    LOCCTR += 4;
                } else {
                    LOCCTR += OPTAB[opcode].format;
                }
            }

            // ASSEMBLER DIRECTIVE cases
            else if (DIRECTIVES.count(opcode)) {

                if (opcode == "WORD") {
                    LOCCTR += 3;
                }

                else if (opcode == "RESW") LOCCTR += 3 * stoi(operand);

                else if (opcode == "RESB") LOCCTR += stoi(operand);

                else if (opcode == "BYTE") {
                    if (operand.empty() || operand.length() < 4) {
                        cerr << "ERROR: Invalid BYTE operand" << endl;
                        fin.close();
                        return 1;
                    }
                    // handle char literals
                    if (operand[0] == 'C' && operand[1] == '\'' && 
                        operand[operand.length()-1] == '\'') {
                        LOCCTR += operand.length() - 3;  // length - quotes
                    }
                    // Handle int literals
                    else if (operand[0] == 'X' && operand[1] == '\'' && 
                             operand[operand.length()-1] == '\'') {
                        string hexval = operand.substr(2, operand.length() - 3);
                        
                        LOCCTR += hexval.length() / 2;
                    }
                }

                else if (opcode == "LTORG") {
                    processLiterals();
                }

                else if (opcode == "END") {
                    processLiterals();
                    break; // terminate pass1
                }
            }

            // process literals
            if (!operand.empty() && operand[0] == '=') {

                string literal = operand;

                if (!LITTAB.count(literal)) {
                    Literal L;
                    L.label = literal;
                    L.address = -1;
                    // process char literals
                    if (literal.length() > 1 && literal[1] == 'C' && 
                        literal.length() > 4 && literal[2] == '\'' && 
                        literal[literal.length()-1] == '\'') {
                        string chars = literal.substr(3, literal.length() - 4);
                        L.len = chars.length();
                        // convert char to hex
                        ostringstream hexStream;
                        hexStream << hex << uppercase << setfill('0'); // padding for hex format
                        for (char c : chars) {
                            hexStream << setw(2) << (unsigned int)(unsigned char)c;
                        }
                        L.val = hexStream.str();
                    } else if (literal.length() > 1 && literal[1] == 'X' &&
                               literal.length() > 4 && literal[2] == '\'' &&
                               literal[literal.length()-1] == '\'') {
                        string hexval = literal.substr(3, literal.length() - 4);

                        L.len = hexval.length() / 2;
                        L.val = hexval;
                    } else {
                        cerr << "Heads up! Invalid literal format here: " << literal << endl;
                        L.len = 0;
                        L.val = "";
                    }

                    LITTAB[literal] = L;
                }
            }
        }

        fin.close();

        // Generate .st file for this file
        string stFile = getOutputFilename(file, ".st");
        writeSYMTABFile(stFile);

        // Print SYMTAB and LITTAB for this file (for debugging/verification)
        cout << "\n==== SYMTAB for " << file << " ====\n";
        for (auto &s : SYMTAB)
            cout << setw(10) << s.first << " : " << hex << uppercase 
                 << setfill('0') << setw(4) << s.second << endl;

        if (!LITTAB.empty()) {
            cout << "\n==== LITTAB for " << file << " ====\n";
            for (auto &l : LITTAB)
                cout << setw(15) << l.first << " @ " << hex << uppercase 
                     << setfill('0') << setw(4) << l.second.address
                     << " len=" << dec << l.second.len << endl;
        }
        cout << endl;
    }

    return 0;
}

