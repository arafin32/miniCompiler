#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char type[20];          // operator type: add, sub, assign
    char value[20];         // leaf value: x, 5, 10
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

/* constructors */
ASTNode* createNode(char* type, ASTNode* left, ASTNode* right);
ASTNode* createLeaf(char* value);
ASTNode* createLeafInt(int value);

/* utilities */
void printAST(ASTNode* node);

#endif