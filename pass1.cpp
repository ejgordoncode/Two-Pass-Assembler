#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>

using namespace std;


//optab(accounted for as many as I could find)

struct OpInfo {
    int format;   //4 possible formats as listed below
};

unordered_map<string, OpInfo> OPTAB = {
    // Format 1
    {"FIX", {1}}, {"FLOAT", {1}}, {"HIO", {1}}, {"NORM", {1}}, {"SIO", {1}}, {"TIO", {1}},

    // Format 2
    {"ADDR", {2}}, {"CLEAR", {2}}, {"COMPR", {2}}, {"DIVR", {2}}, {"MULR", {2}},
    {"RMO", {2}}, {"SHIFTL", {2}}, {"SHIFTR", {2}}, {"SUBR", {2}},
    {"SVC", {2}}, {"TIXR", {2}},

    // Format 3/4 instructions
    {"ADD", {3}}, {"ADDF", {3}}, {"AND", {3}}, {"COMP", {3}}, {"COMPF", {3}},
    {"DIV", {3}}, {"DIVF", {3}}, {"J", {3}}, {"JEQ", {3}}, {"JGT", {3}},
    {"JLT", {3}}, {"JSUB", {3}}, {"LDA", {3}}, {"LDB", {3}}, {"LDCH", {3}},
    {"LDF", {3}}, {"LDL", {3}}, {"LDS", {3}}, {"LDT", {3}}, {"LDX", {3}},
    {"LPS", {3}}, {"MUL", {3}}, {"MULF", {3}}, {"OR", {3}}, {"RD", {3}},
    {"RSUB", {3}}, {"SSK", {3}}, {"STA", {3}}, {"STB", {3}}, {"STCH", {3}},
    {"STF", {3}}, {"STI", {3}}, {"STL", {3}}, {"STS", {3}}, {"STSW", {3}},
    {"STT", {3}}, {"STX", {3}}, {"SUB", {3}}, {"SUBF", {3}}, {"TD", {3}},
    {"TIX", {3}}, {"WD", {3}}
};

// directives
unordered_set<string> DIRECTIVES = {
    "START", "END", "WORD", "RESW", "RESB", "BYTE", "LTORG", "BASE", "NOBASE"
};

// symbol table (accessing symtab)
unordered_map<string, int> SYMTAB;

// literal table (unsure if this format is right)
struct Literal {
    string value;
    int length;
    int address;
};
unordered_map<string, Literal> LITTAB;


//helper functions below:

// parses a line onto  - LABEL, OPCODE, OPERAND
bool parseLine(const string &line, string &label, string &opcode, string &operand) {
    if (line.empty() || line[0] == '.') return false; 

    stringstream ss(line);
    ss >> label >> opcode >> operand;

    // if line inputed only has opcode + oprand, label is empty
    if (opcode.empty()) {
        opcode = label;
        label = "";
    }

    return true;
}


//assign addresses to literals (utilizing locctr)

void processLiterals(int &LOCCTR) {
    for (auto &lit : LITTAB) {
        if (lit.second.address == -1) { // unassigned
            lit.second.address = LOCCTR;
            LOCCTR += lit.second.length;
        }
    }
}


//MAIN PASS1!!!!

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "no input present, closing.\n";
        return 1;
    }

    for (int f = 1; f < argc; f++) {
        string file = argv[f];
        ifstream fin(file);
        if (!fin) {
            cerr << "Error opening the file " << file << endl;
            continue;
        }

        cout << "Loading " << file << endl;

        int LOCCTR = 0;
        bool started = false;

        string line, label, opcode, operand;

        while (getline(fin, line)) {
            if (!parseLine(line, label, opcode, operand))
                continue;

            //start directive here
            if (opcode == "START" && !started) {
                LOCCTR = stoi(operand, nullptr, 16);
                started = true;
                continue;
            }

            //symtab label processing
            if (!label.empty()) {
                if (SYMTAB.count(label)) {
                    cerr << "ERROR: multiple labels with name: " << label << endl;
                    return 1;
                }
                SYMTAB[label] = LOCCTR;
            }

            /* =============================
               OPCODE PROCESSING
               ============================= */

            bool format4 = false;

            // format 4: +OP
            if (opcode[0] == '+') {
                opcode = opcode.substr(1);
                format4 = true;
            }

            if (OPTAB.count(opcode)) {
                // format 4 is always +1 byte over format 3 => 4 bytes
                if (format4) {
                    LOCCTR += 4;
                } else {
                    LOCCTR += OPTAB[opcode].format;
                }
            }

            //directives are here
            else if (DIRECTIVES.count(opcode)) {

                if (opcode == "WORD") LOCCTR += 3;

                else if (opcode == "RESW") LOCCTR += 3 * stoi(operand);

                else if (opcode == "RESB") LOCCTR += stoi(operand);

                else if (opcode == "BYTE") {
                    if (operand[0] == 'C') {
                        LOCCTR += operand.length() - 3;  // C'EOF'
                    }
                    else if (operand[0] == 'X') {
                        LOCCTR += (operand.length() - 3) / 2; // X'05'
                    }
                }

                else if (opcode == "LTORG") {
                    processLiterals(LOCCTR);
                }

                else if (opcode == "END") {
                    processLiterals(LOCCTR);
                    break; // pass1 stops here
                }
            }

            // literal processing
            if (!operand.empty() && operand[0] == '=') {

                string literal = operand;

                if (!LITTAB.count(literal)) {
                    Literal L;
                    L.address = -1;

                    if (literal[1] == 'C') {
                        string chars = literal.substr(3, literal.length() - 4);
                        L.length = chars.length();
                    } else if (literal[1] == 'X') {
                        string hexval = literal.substr(3, literal.length() - 4);
                        L.length = hexval.length() / 2;
                    }

                    LITTAB[literal] = L;
                }
            }
        }

    }

    //return and print current symbol table
    cout << "\n==== SYMTAB ====\n";
    for (auto &s : SYMTAB)
        cout << setw(10) << s.first << " : " << hex << s.second << endl;

    //return and print littab
    cout << "\n==== LITTAB ====\n";
    for (auto &l : LITTAB)
        cout << setw(10) << l.first << " @ " << hex << l.second.address
             << " len=" << dec << l.second.length << endl;

    return 0;
}



End Function
 */

