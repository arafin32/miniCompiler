#ifndef TAC_H
#define TAC_H

#include "ast.h"

void generateTAC(ASTNode* node);
void printTAC();

/* NEW for control flow */
char* newLabel();

#endif