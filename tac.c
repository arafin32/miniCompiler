#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

int tempCount = 1;
int labelCount = 1;

char tacCode[300][120];
int tacIndex = 0;

/* ---------------- TEMP ---------------- */
char* newTemp()
{
    char* temp = (char*) malloc(10);
    sprintf(temp, "t%d", tempCount++);
    return temp;
}

/* ---------------- LABEL ---------------- */
char* newLabel()
{
    char* label = (char*) malloc(10);
    sprintf(label, "L%d", labelCount++);
    return label;
}

/* ---------------- EMIT ---------------- */
void emit(char* code)
{
    strcpy(tacCode[tacIndex++], code);
}

/* ---------------- OPERATOR MAP ---------------- */
char* mapOp(char* type)
{
    if (strcmp(type, "add") == 0) return "+";
    if (strcmp(type, "sub") == 0) return "-";
    if (strcmp(type, "mul") == 0) return "*";
    if (strcmp(type, "div") == 0) return "/";
    if (strcmp(type, "gt") == 0) return ">";
    if (strcmp(type, "lt") == 0) return "<";
    if (strcmp(type, "eq") == 0) return "==";

    return type;
}

/* ---------------- EXPRESSIONS ---------------- */
char* genExpr(ASTNode* node)
{
    if (!node) return "";

    if (!node->left && !node->right)
        return node->value;

    char* l = genExpr(node->left);
    char* r = genExpr(node->right);

    char* t = newTemp();

    char line[120];
    sprintf(line, "%s = %s %s %s", t, l, mapOp(node->type), r);

    emit(line);

    return t;
}

/* ---------------- MAIN TAC ---------------- */
void generateTAC(ASTNode* node)
{
    if (!node) return;

    /* assignment */
    if (strcmp(node->type, "assign") == 0)
    {
        char* r = genExpr(node->right);

        char line[120];
        sprintf(line, "%s = %s", node->left->value, r);
        emit(line);
    }

    /* ---------------- IF ---------------- */
    else if (strcmp(node->type, "if") == 0)
    {
        char* cond = genExpr(node->left);

        char* L1 = newLabel();

        char line1[120];
        sprintf(line1, "ifFalse %s goto %s", cond, L1);
        emit(line1);

        generateTAC(node->right);

        char line2[120];
        sprintf(line2, "%s:", L1);
        emit(line2);
    }

    /* ---------------- IF-ELSE ---------------- */
    else if (strcmp(node->type, "ifelse") == 0)
    {
        char* cond = genExpr(node->left);

        char* L1 = newLabel();
        char* L2 = newLabel();

        char line1[120];
        sprintf(line1, "ifFalse %s goto %s", cond, L1);
        emit(line1);

        generateTAC(node->right->left);

        char line2[120];
        sprintf(line2, "goto %s", L2);
        emit(line2);

        sprintf(line1, "%s:", L1);
        emit(line1);

        generateTAC(node->right->right);

        sprintf(line1, "%s:", L2);
        emit(line1);
    }

    /* ---------------- WHILE ---------------- */
    else if (strcmp(node->type, "while") == 0)
    {
        char* L1 = newLabel();
        char* L2 = newLabel();

        char start[120];
        sprintf(start, "%s:", L1);
        emit(start);

        char* cond = genExpr(node->left);

        char line1[120];
        sprintf(line1, "ifFalse %s goto %s", cond, L2);
        emit(line1);

        generateTAC(node->right);

        char line2[120];
        sprintf(line2, "goto %s", L1);
        emit(line2);

        sprintf(line1, "%s:", L2);
        emit(line1);
    }
}

/* ---------------- PRINT ---------------- */
void printTAC()
{
    printf("\n=== THREE ADDRESS CODE ===\n");

    for(int i = 0; i < tacIndex; i++)
        printf("%s\n", tacCode[i]);
}