#ifndef TAC_H
#define TAC_H

#include "ast.h"
#include <stdio.h>

void generateTAC(ASTNode* node);
void printTAC();
void printTACToFile(FILE* file);

char* newLabel();

#endif
