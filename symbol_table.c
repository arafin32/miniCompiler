#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

#define MAX_SYMBOLS 256
#define MAX_SCOPE_DEPTH 32

struct Symbol {
    char name[50];
    char type[10];
    int depth;
};

static struct Symbol table[MAX_SYMBOLS];
static int symbolCount = 0;
static int currentScope = 0;
static int scopeStart[MAX_SCOPE_DEPTH];

void enterScope()
{
    if (currentScope + 1 < MAX_SCOPE_DEPTH)
    {
        currentScope++;
        scopeStart[currentScope] = symbolCount;
    }
}

void exitScope()
{
    if (currentScope > 0)
    {
        symbolCount = scopeStart[currentScope];
        currentScope--;
    }
}

void insert(char* name, char* type)
{
    if (symbolCount < MAX_SYMBOLS)
    {
        strcpy(table[symbolCount].name, name);
        strcpy(table[symbolCount].type, type);
        table[symbolCount].depth = currentScope;
        symbolCount++;
    }
}

int lookup(char* name)
{
    for (int i = symbolCount - 1; i >= 0; i--)
    {
        if (strcmp(table[i].name, name) == 0)
            return i;
    }
    return -1;
}

int lookupCurrentScope(char* name)
{
    for (int i = scopeStart[currentScope]; i < symbolCount; i++)
    {
        if (strcmp(table[i].name, name) == 0)
            return i;
    }
    return -1;
}

char* getSymbolType(char* name)
{
    int idx = lookup(name);
    if (idx >= 0)
        return table[idx].type;
    return NULL;
}
