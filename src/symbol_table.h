#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

void insert(char* name, char* type);
int lookup(char* name);
int lookupCurrentScope(char* name);
void enterScope();
void exitScope();
char* getSymbolType(char* name);

#endif
