void pass2() {
    


    string label, opcode, operand;
    string textRecord = "";
    int textStart = PROG_START;
    int textLength = 0;
    int loc;

    bool baseFlag = false;
    int baseReg = 0;

    // header record as needed
    obj << "H" 
        << setw(6) << left << PROGRAM_NAME
        << setw(6) << right << setfill('0') << hex << PROG_START
        << setw(6) << right << setfill('0') << hex << PROGRAM_LENGTH
        << "\n";

    // read int. steps
    while (inter >> hex >> loc >> label >> opcode >> operand) {

        string objcode = "";


        //all intended to  be meshed w/ pre-defined directives
        
        //directives needed

        if (opcode == "BASE") {
            baseFlag = true;
            baseReg = SYMTAB[operand];
            continue;
        }

        if (opcode == "NOBASE") {
            baseFlag = false;
            continue;
        }

        if (opcode == "WORD") {
            int value = stoi(operand);
            stringstream s;
            s << setw(6) << setfill('0') << hex << value;
            objcode = s.str();
        }

        else if (opcode == "BYTE") {
            if (operand[0] == 'C') {
                string inside = operand.substr(2, operand.size() - 3);
                for (char c : inside) {
                    stringstream s;
                    s << setw(2) << setfill('0') << hex << (int)c;
                    objcode += s.str();
                }
            } else if (operand[0] == 'X') {
                objcode = operand.substr(2, operand.size() - 3);
            }
        }

        else if (opcode == "RESW" || opcode == "RESB") {
           
           
            if (textLen > 0) {
                obj << "T"
                    << setw(6) << setfill('0') << hex << textStart
                    << setw(2) << setfill('0') << hex << textLen/2
                    << textRecord << "\n";

                textRecord = "";
                textLen = 0;
            }
            continue;
        }

       //start of pass 2 instructions

        else if (OPTAB.count(opcode) || (opcode[0] == '+' && OPTAB.count(opcode.substr(1)))) {

            bool format4 = (opcode[0] == '+');
            string op = format4 ? opcode.substr(1) : opcode;

            int opValue = OPTAB[op].opcode;
            int fmt = OPTAB[op].format;
            if (format4) fmt = 4;

            // for format 1
            if (fmt == 1) {
                stringstream s;
                s << setw(2) << setfill('0') << hex << opValue;
                objcode = s.str();
            }

            // for format 2
            else if (fmt == 2) {
                int r1 = REG[operand[0]];
                int r2 = (operand.size() > 2) ? REG[operand[2]] : 0;

                stringstream s;
                s << setw(2) << setfill('0') << hex << opValue
                  << hex << r1 << hex << r2;
                objcode = s.str();
            }

            // format 3 and 4
            else if (fmt == 3 || fmt == 4) {

                bool n=1, i=1, x=0, b=0, p=0, e=format4;

                string operand2 = operand;
                int TA = 0;
                bool isConst = false;

                // addressing @ or #
                if (operand2[0] == '#') { n=0; i=1; operand2 = operand2.substr(1); }
                if (operand2[0] == '@') { n=1; i=0; operand2 = operand2.substr(1); }

                // indexing the operands
                if (operand2.find(",X") != string::npos) {
                    x = 1;
                    operand2 = operand2.substr(0, operand2.find(","));
                }

                // check operands
                if (isdigit(operand2[0])) {
                    isConst = true;
                    TA = stoi(operand2);
                } else {
                    TA = SYMTAB[operand2];
                }

                int disp = 0;

            

                int ni = (n << 1) | i;
                int xbpe = (x<<3)|(b<<2)|(p<<1)|e;

                stringstream s;

                if (fmt == 3) {
                    s << setw(2) << setfill('0') << hex << (opValue | ni)
                      << setw(2) << setfill('0') << hex << xbpe
                      << setw(3) << setfill('0') << hex << (disp & 0xFFF);
                }
                else if (fmt == 4) {
                    s << setw(2) << setfill('0') << hex << (opValue | ni)
                      << setw(2) << setfill('0') << hex << xbpe
                      << setw(5) << setfill('0') << hex << TA;
                }

                objcode = s.str();
            }
        }

        // text record section

        if (objcode != "") {
            if (textLen + objcode.size() > 60) {
                obj << "T"
                    << setw(6) << setfill('0') << hex << textStart
                    << setw(2) << setfill('0') << hex << textLen/2
                    << textRecord << "\n";

                textRecord = "";
                textLen = 0;
                textStart = loc;
            }

            textRecord += objcode;
            textLen += objcode.size();
        }

        //list file
        lst << setw(4) << hex << loc << "  "
            << setw(10) << objcode << "  "
            << label << " " << opcode << " " << operand << "\n";
    }

    // push last record
    if (textLen > 0) {
        obj << "T"
            << setw(6) << setfill('0') << hex << textStart
            << setw(2) << setfill('0') << hex << textLen/2
            << textRecord << "\n";
    }

   //end
    obj << "E" 
        << setw(6) << setfill('0') << hex << PROG_START << "\n";
}
