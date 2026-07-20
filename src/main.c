#include <stdio.h>
#include "ast.h"
#include "tac.h"
#include "semantic.h"
#include "optimizer.h"
#include "codegen_extended.h"

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

        if (semantic_error_count() > 0)
        {
            printf("\nCompilation stopped due to semantic errors.\n");
            return 0;
        }

        /* TAC generation */
        printf("\n=== GENERATING TAC ===\n");
        generateTAC(root);
        printTAC();

        /* Code optimization */
        optimizeTAC();
        printTAC();
        printOptimizationStats();

        /* Stack machine code generation */
        generateStackMachineCode();
        printStackMachineTAC();

        /* Output to files */
        FILE* output = fopen("output.tac", "w");
        if (output)
        {
            fprintf(output, "=== UNOPTIMIZED THREE ADDRESS CODE ===\n");
            printTACToFile(output);
            fprintf(output, "\n");
            printOptimizationStats();
            fprintf(output, "\n");
            printStackMachineCode(output);
            fclose(output);
            printf("\nCompilation output written to output.tac\n");
        }
        else
        {
            printf("Error: could not open output.tac\n");
        }
    }

    return 0;
}
