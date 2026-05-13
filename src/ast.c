#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ---------------- NODE (operator) ---------------- */
ASTNode* createNode(char* type, ASTNode* left, ASTNode* right)
{
    ASTNode* node = (ASTNode*) malloc(sizeof(ASTNode));
    strcpy(node->type, type);

    node->value[0] = '\0';   // important: no value for internal nodes

    node->left = left;
    node->right = right;

    return node;
}

/* ---------------- LEAF (identifier) ---------------- */
ASTNode* createLeaf(char* value)
{
    ASTNode* node = (ASTNode*) malloc(sizeof(ASTNode));

    node->type[0] = '\0';    // no type for leaf nodes
    strcpy(node->value, value);

    node->left = NULL;
    node->right = NULL;

    return node;
}

/* ---------------- LEAF (number) ---------------- */
ASTNode* createLeafInt(int value)
{
    ASTNode* node = (ASTNode*) malloc(sizeof(ASTNode));

    node->type[0] = '\0';
    sprintf(node->value, "%d", value);

    node->left = NULL;
    node->right = NULL;

    return node;
}

/* ---------------- PRETTY AST PRINT ---------------- */
void printAST(ASTNode* node)
{
    if (!node) return;

    /* leaf node */
    if (!node->left && !node->right)
    {
        printf("%s", node->value);
        return;
    }

    printf("(");
    printf("%s ", node->type);

    printAST(node->left);
    printf(" ");
    printAST(node->right);

    printf(")");
}