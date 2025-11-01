#ifndef DATASTRUCTURES_H 
#define DATASTRUCTURES_H

#include <unordered_map>
#include <string>

using namespace std;

// I'm using extern here since these are accessed by more than one file, 
//   allowing access to be shared without additional storage for each 
//   instance

struct Literal {
  string label;
  string val;
  int len;
  int address;
};

extern unordered_map<string, Literal> LITTAB;
// example usage in logic:
//    LITTAB["C'EOF'"] = {"C'EOF'", "454F46", 3, 0x002D};

extern unordered_map<string, int> SYMTAB;

extern unordered_map<string, int> OPTAB;

extern int LOCCTR;



#endif // DATASTRUCTURES_H
