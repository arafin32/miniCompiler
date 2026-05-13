#ifndef CODEGEN_EXTENDED_H
#define CODEGEN_EXTENDED_H

#include <stdio.h>

/* Extended code generation for stack machine */

/* Generate stack machine code from TAC */
void generateStackMachineCode();

/* Print stack machine code to file */
void printStackMachineCode(FILE* file);

/* Print stack machine code to console */
void printStackMachineTAC();

#endif
