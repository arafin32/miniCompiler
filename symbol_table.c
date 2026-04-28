#include <stdio.h>
#include <string.h>

struct Symbol {
    char name[50];
    char type[10];
};

struct Symbol table[100];
int count = 0;

void insert(char* name, char* type) {
    strcpy(table[count].name, name);
    strcpy(table[count].type, type);
    count++;
}

int lookup(char* name) {
    for(int i=0;i<count;i++) {
        if(strcmp(table[i].name, name)==0)
            return 1;
    }
    return 0;
}