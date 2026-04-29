#include <stdio.h>
#include "ast.h"
#include "tac.h"
#include "semantic.h"

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

        /* Semantic analysis */
        printf("\n=== SEMANTIC ANALYSIS ===\n");
        semantic_check(root);

        /* TAC generation and output to file */
        printf("\n=== GENERATING TAC ===\n");
        generateTAC(root);
        
        FILE* output = fopen("output.tac", "w");
        if (output)
        {
            printTACToFile(output);
            fclose(output);
            printf("TAC written to output.tac\n");
        }
        else
        {
            printf("Error: could not open output.tac\n");
            printTAC();
        }
    }

    return 0;
}
