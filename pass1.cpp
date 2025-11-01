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



