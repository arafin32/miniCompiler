#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

static int error_count = 0;

void check_ast(ASTNode* node)
{
    if (!node) return;

    /* declaration: register variable */
    if (strcmp(node->type, "decl") == 0)
    {
        char* var_name = node->left->value;
        char* var_type = node->right->value;
        
        /* check duplicate in current scope */
        if (lookupCurrentScope(var_name) >= 0)
        {
            printf("SEMANTIC ERROR: Variable '%s' already declared in current scope\n", var_name);
            error_count++;
        }
        else
        {
            insert(var_name, var_type);
        }
        return;
    }

    /* assignment: check variable exists and is initialized */
    if (strcmp(node->type, "assign") == 0)
    {
        char* var_name = node->left->value;
        
        if (lookup(var_name) < 0)
        {
            printf("SEMANTIC ERROR: Undeclared variable '%s'\n", var_name);
            error_count++;
        }
        
        check_ast(node->right);
        return;
    }

    /* print: check variable exists */
    if (strcmp(node->type, "print") == 0)
    {
        check_ast(node->left);
        return;
    }

    /* scope entry/exit */
    if (strcmp(node->type, "scope") == 0)
    {
        enterScope();
        check_ast(node->left);
        exitScope();
        return;
    }

    /* recursive checks for control flow */
    if (strcmp(node->type, "if") == 0 ||
        strcmp(node->type, "ifelse") == 0 ||
        strcmp(node->type, "while") == 0)
    {
        check_ast(node->left);
        check_ast(node->right);
        return;
    }

    /* block: process statements */
    if (strcmp(node->type, "block") == 0)
    {
        check_ast(node->left);
        check_ast(node->right);
        return;
    }

    /* expression: check variable references */
    if (node->left || node->right)
    {
        if (strlen(node->value) > 0 && node->value[0] >= 'a' && node->value[0] <= 'z')
        {
            /* might be identifier - check if declared */
            if (lookup(node->value) < 0)
            {
                printf("SEMANTIC ERROR: Undeclared variable '%s'\n", node->value);
                error_count++;
            }
        }
        check_ast(node->left);
        check_ast(node->right);
    }
}

void semantic_check(ASTNode* root)
{
    error_count = 0;
    check_ast(root);
    
    if (error_count == 0)
        printf("Semantic analysis completed successfully\n");
}

int semantic_error_count()
{
    return error_count;
}
