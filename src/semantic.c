#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"
#include "symbol_table.h"

static int error_count = 0;
static int type_errors = 0;

/* Forward declaration for type inference */
char* inferExprType(ASTNode* node);

/* Get inferred type of an expression */
char* inferExprType(ASTNode* node)
{
    if (!node) return "int";  /* default to int */
    
    /* Leaf nodes - identifiers or literals */
    if (!node->left && !node->right)
    {
        if (strcmp(node->value, "true") == 0 || strcmp(node->value, "false") == 0)
        {
            return "bool";
        }

        /* If it's an identifier, look up its type */
        if (strlen(node->value) > 0 && isalpha(node->value[0]))
        {
            char* type = getSymbolType(node->value);
            return type ? type : "int";
        }
        /* Numbers are always int */
        return "int";
    }
    
    /* Binary operations return int */
    if (strcmp(node->type, "add") == 0 || strcmp(node->type, "sub") == 0 ||
        strcmp(node->type, "mul") == 0 || strcmp(node->type, "div") == 0)
    {
        char* leftType = inferExprType(node->left);
        char* rightType = inferExprType(node->right);
        
        /* Check type compatibility */
        if (strcmp(leftType, "int") != 0 || strcmp(rightType, "int") != 0)
        {
            printf("TYPE ERROR: Arithmetic operations require int operands\n");
            type_errors++;
        }
        return "int";
    }
    
    /* Comparison operations return bool */
    if (strcmp(node->type, "eq") == 0 || strcmp(node->type, "ne") == 0 ||
        strcmp(node->type, "lt") == 0 || strcmp(node->type, "gt") == 0)
    {
        char* leftType = inferExprType(node->left);
        char* rightType = inferExprType(node->right);
        
        /* Check type compatibility for comparisons */
        if (strcmp(leftType, rightType) != 0)
        {
            printf("TYPE ERROR: Comparison operands must have compatible types\n");
            type_errors++;
        }
        return "bool";
    }
    
    return "int";
}

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

    /* assignment: check variable exists and check type compatibility */
    if (strcmp(node->type, "assign") == 0)
    {
        char* var_name = node->left->value;
        
        if (lookup(var_name) < 0)
        {
            printf("SEMANTIC ERROR: Undeclared variable '%s'\n", var_name);
            error_count++;
            check_ast(node->right);
            return;
        }
        
        /* Get the variable type */
        char* var_type = getSymbolType(var_name);
        
        /* Infer the expression type */
        char* expr_type = inferExprType(node->right);
        
        /* Check type compatibility */
        if (strcmp(var_type, expr_type) != 0)
        {
            printf("TYPE ERROR: Cannot assign '%s' value to '%s' variable '%s'\n", 
                   expr_type, var_type, var_name);
            type_errors++;
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
            /* identifier usage must be declared */
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
    type_errors = 0;
    check_ast(root);
    
    if (error_count == 0 && type_errors == 0)
        printf("Semantic analysis completed successfully\n");
    else if (error_count > 0 || type_errors > 0)
        printf("Semantic analysis found %d semantic error(s) and %d type error(s)\n", 
               error_count, type_errors);
}

int semantic_error_count()
{
    return error_count + type_errors;
}
