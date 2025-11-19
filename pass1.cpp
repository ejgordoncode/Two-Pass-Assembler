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

/* REFINED PSEUDOCODE:

Function Pass1_pass1(lines):

    Initialize LOCCTR to 0
    Set index = 0

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

