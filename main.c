#include <stdio.h>
#include "ast.h"
#include "tac.h"

extern int yyparse();
extern ASTNode* root;

int main()
{
    printf("MiniLang Compiler Started...\n");

    if (yyparse() == 0)
    {
        printf("\nParsing successful!\n");

        printf("\n=== AST ===\n");
        printAST(root);

        /* 🔥 GENERATE TAC */
        generateTAC(root);
        printTAC();
    }

    return 0;
}