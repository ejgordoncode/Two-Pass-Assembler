// read intermediate file and load SYMTAB and OPTAB to be used

// translate instructions to object code (do not produce .obj file)

// handle pc and base-relative addressing

//format output for .l file (see fig 2.10 in book)

//output .l and .st files


/* REFINED PSEUDOCODE: 

Function Pass2_pass2(lines, filename):

    Open listing output file

    Initialize currentAddress = 0

    If first line opcode is START:
        Convert operand hex string to currentAddress

    For each line in lines:
        If comment:
            Skip

        Initialize objectCode = ""

        If opcode == BYTE:
            If operand starts with 'C':
                Convert characters to ASCII hex string
                objectCode = hex string
            Else if operand starts with 'X':
                objectCode = operand hex string (uppercase)
            Write listing line with objectCode
            currentAddress += length of BYTE

        Else if opcode == WORD:
            Convert integer operand to 6-digit hex string
            objectCode = hex string
            Write listing line
            currentAddress += 3

        Else if opcode == RESW or RESB:
            objectCode = ""
            Write listing line (reserve space)
            currentAddress += corresponding bytes

        Else if opcode in OPTAB:
            Get opcode hex value (string or int)
            If opcode is RSUB:
                objectCode = opcode + "0000"
            Else if operand has symbol in SYMTAB:
                Convert symbol address to 4-digit hex string
                Concatenate with opcode to form objectCode
            Write listing line with objectCode
            currentAddress += instruction length

        Else if opcode == START or END:
            objectCode = ""
            Write listing line
            If END: break

    Write SYMTAB to output file

End Function
*/