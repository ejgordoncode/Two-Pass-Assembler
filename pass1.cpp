// open each file from input

// scan each opened file

// get the starting address from START directive

// assign LOCCTR to the starting address

// FOR EACH LINE:

/*
    extract label, opcode, operand
    update SYMTAB
      check for duplicates
      update LOCCTR
    determine instruction format and increment LOCCTR accordingly
    if encountering an assembler directive that affects LOCCTR or
        literals, recognize and process accordingly
    record info to intermediate file (location, symbol, opcode, operand
*/

// build LITTAB for all literals encountered

// assign addresses to unassigned literals when encountering END or LTORG

// determine program length

// record LITTAB and SYMTAB


// Necessary Includes //
#include <iostream> // device input and output
#include <fstream> // file input and output
#include <string>
using namespace std;

// Assembler Directives //
unordered_set<string> assemblerDirectives = {
  "START",
  "END",
  "WORD",
  "RESW",
  "RESB",
  "BYTE",
  "LTORG",
  "BASE",
  "NOBASE",
}

// Set index = 0
int index = 0;

// main logic for pass1 //

int main(int argc, char* argv[]){
  // check if user provided an input file
  if (argc < 2) { 
    cerr << "Sorry, no input file provided." << endl;
    return 1; // terminate assembly
  }

  // open input file(s)
  for (int arg = 1; arg < argc; arg++) {
    string inputFileName = argv[arg];
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()){
      cerr << "Sorry, failed to open " << inputFileName << endl;
      return 1; //terminate assembly
    }
    else {
      cout << "Successfully opened %s" << inputFileName << endl;
    }
    inputFile.close(); // don't leave your windows open, son!
  }



  

  //
}

/* REFINED PSEUDOCODE:

Function Pass1_pass1(lines):

    Set index = 0 DONE

    If the first line's opcode is "START":
        Convert operand (hex string) to integer → LOCCTR
        index = 1

    For each line from lines[index] to end:
        Get label, opcode, operand

        If line is a comment:
            Skip

        Else
            If label is not empty:
                If label already in SYMTAB:
                    Error: duplicate symbol
                Else:
                    SYMTAB[label] = LOCCTR

            If opcode in OPTAB:
                Look up instruction length (format) from OPTAB
                LOCCTR += instruction length
            Else if opcode is assembler directive:
                If "WORD": LOCCTR += 3
                Else if "RESW": LOCCTR += 3 * operand value
                Else if "RESB": LOCCTR += operand value
                Else if "BYTE": calculate length based on operand; LOCCTR += length
                Else if "LTORG":
                    Assign addresses to literals in LITTAB if any; update LOCCTR
                Else if "BASE" or "NOBASE":
                    No LOCCTR increment
                Else if "END":
                    Process any remaining literals; break loop
                Else:
                    Error invalid opcode/directive

            If operand is a literal (starts with '='):
                If literal not in LITTAB:
                    Add literal to LITTAB

            Write line and LOCCTR info to listing output (optional)

    Write SYMTAB and LITTAB to their respective output files

End Function
 */

