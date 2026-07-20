#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen_extended.h"

extern char tacCode[300][120];
extern int tacIndex;

char stackCode[300][200];
int stackIndex = 0;

/* Variable storage - simple linear allocation */
typedef struct {
    char name[30];
    int offset;
} VarStorage;

VarStorage varStorage[100];
int varCount = 0;
int nextOffset = 0;

/* Register allocation (simple round-robin) */
typedef struct {
    char name[10];
    int inUse;
    char tempVar[20];
} Register;

Register registers[4];  /* r0-r3 */

static int isNum(const char* str);
static void spillRegister(int reg);
void storeValue(int reg, const char* varName);
static int allocateRegisterAvoid(const char* tempVar, int avoid1, int avoid2);

void initRegisters()
{
    strcpy(registers[0].name, "r0");
    strcpy(registers[1].name, "r1");
    strcpy(registers[2].name, "r2");
    strcpy(registers[3].name, "r3");
    for (int i = 0; i < 4; i++)
    {
        registers[i].inUse = 0;
        registers[i].tempVar[0] = '\0';
    }
}

static int findRegister(const char* tempVar)
{
    if (!tempVar || !*tempVar) return -1;
    for (int i = 0; i < 4; i++)
    {
        if (registers[i].inUse && strcmp(registers[i].tempVar, tempVar) == 0)
            return i;
    }
    return -1;
}

static void spillRegister(int reg)
{
    if (reg < 0 || reg >= 4) return;
    if (!registers[reg].inUse)
        return;

    if (registers[reg].tempVar[0] != '\0' && !isNum(registers[reg].tempVar))
    {
        storeValue(reg, registers[reg].tempVar);
    }

    registers[reg].inUse = 0;
    registers[reg].tempVar[0] = '\0';
}

static int allocateRegisterAvoid(const char* tempVar, int avoid1, int avoid2)
{
    int idx = findRegister(tempVar);
    if (idx >= 0)
        return idx;

    for (int i = 0; i < 4; i++)
    {
        if (!registers[i].inUse && i != avoid1 && i != avoid2)
        {
            registers[i].inUse = 1;
            registers[i].tempVar[0] = '\0';
            return i;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (!registers[i].inUse)
        {
            registers[i].inUse = 1;
            registers[i].tempVar[0] = '\0';
            return i;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (i != avoid1 && i != avoid2)
        {
            spillRegister(i);
            registers[i].inUse = 1;
            registers[i].tempVar[0] = '\0';
            return i;
        }
    }

    spillRegister(0);
    registers[0].inUse = 1;
    registers[0].tempVar[0] = '\0';
    return 0;
}

int allocateRegister(const char* tempVar)
{
    return allocateRegisterAvoid(tempVar, -1, -1);
}

int allocateRegisterForDest(const char* tempVar, int avoid1, int avoid2)
{
    int idx = findRegister(tempVar);
    if (idx >= 0)
        return idx;

    for (int i = 0; i < 4; i++)
    {
        if (!registers[i].inUse && i != avoid1 && i != avoid2)
        {
            registers[i].inUse = 1;
            registers[i].tempVar[0] = '\0';
            return i;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (!registers[i].inUse)
        {
            registers[i].inUse = 1;
            registers[i].tempVar[0] = '\0';
            return i;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (i != avoid1 && i != avoid2)
        {
            spillRegister(i);
            registers[i].inUse = 1;
            registers[i].tempVar[0] = '\0';
            return i;
        }
    }

    spillRegister(0);
    registers[0].inUse = 1;
    registers[0].tempVar[0] = '\0';
    return 0;
}

void emitStack(const char* code)
{
    strcpy(stackCode[stackIndex++], code);
}

/* Get variable offset in memory */
int getVarOffset(const char* varName)
{
    for (int i = 0; i < varCount; i++)
    {
        if (strcmp(varStorage[i].name, varName) == 0)
            return varStorage[i].offset;
    }
    
    /* New variable */
    strcpy(varStorage[varCount].name, varName);
    varStorage[varCount].offset = nextOffset;
    nextOffset += 4;
    varCount++;
    
    return varStorage[varCount - 1].offset;
}

/* Check if value is a number */
int isNum(const char* str)
{
    if (!str || !*str) return 0;
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i]) && str[i] != '-') return 0;
    return 1;
}

static int isLabelLine(const char* line)
{
    if (!line || !*line) return 0;
    const char* colon = strchr(line, ':');
    if (!colon) return 0;
    for (const char* p = line; p < colon; p++)
    {
        if (*p == ' ' || *p == '\t')
            return 0;
    }
    return 1;
}

static void invalidateVariableRegisters(const char* varName, int keepReg)
{
    if (!varName || !*varName) return;
    for (int i = 0; i < 4; i++)
    {
        if (i == keepReg)
            continue;
        if (registers[i].inUse && strcmp(registers[i].tempVar, varName) == 0)
        {
            registers[i].tempVar[0] = '\0';
        }
    }
}

/* Generate code to load a value into a register */
int loadValue(int reg, const char* value)
{
    int existing = -1;
    if (!isNum(value))
        existing = findRegister(value);
    if (existing >= 0)
        return existing;

    if (registers[reg].inUse && strcmp(registers[reg].tempVar, value) != 0)
    {
        spillRegister(reg);
    }

    char code[200];
    if (isNum(value))
    {
        sprintf(code, "  LOAD %s, #%s       ; Load constant %s", 
                registers[reg].name, value, value);
    }
    else
    {
        int offset = getVarOffset(value);
        sprintf(code, "  LOAD %s, [bp-%d]    ; Load variable %s", 
                registers[reg].name, offset, value);
    }
    emitStack(code);
    registers[reg].inUse = 1;
    strcpy(registers[reg].tempVar, value);
    return reg;
}

/* Generate code to store a register to variable */
void storeValue(int reg, const char* varName)
{
    if (registers[reg].inUse && strcmp(registers[reg].tempVar, varName) == 0)
        return;

    invalidateVariableRegisters(varName, reg);
    char code[200];
    int offset = getVarOffset(varName);
    sprintf(code, "  STORE [bp-%d], %s   ; Store %s", 
            offset, registers[reg].name, varName);
    emitStack(code);
}

/* Translate TAC to stack machine code */
void generateStackMachineCode()
{
    printf("\n=== STACK MACHINE CODE GENERATION ===\n");
    
    stackIndex = 0;
    varCount = 0;
    nextOffset = 0;
    initRegisters();
    
    emitStack("");
    emitStack("; Pseudo-assembly for simple stack machine");
    emitStack("; Registers: r0, r1, r2, r3");
    emitStack("; Stack frame: bp (base pointer)");
    emitStack("");
    
    char code[200];
    char line[120];
    char temp[20], op1[20], op[10], op2[20];
    char var[20], val[20];
    char label[20], cond[20];
    
    for (int i = 0; i < tacIndex; i++)
    {
        strcpy(line, tacCode[i]);
        
        /* Skip empty lines and labels */
        if (strlen(line) == 0 || isLabelLine(line))
        {
            if (strlen(line) > 0)
            {
                sprintf(code, "%s", line);
                emitStack(code);
                initRegisters();
            }
            continue;
        }
        
        /* Parse different TAC instruction types */
        
        /* Binary operation: t = a op b */
        if (sscanf(line, "%s = %s %s %s", temp, op1, op, op2) == 4)
        {
            int r1 = allocateRegisterAvoid(op1, -1, -1);
            int r2 = allocateRegisterAvoid(op2, r1, -1);
            int dest = allocateRegisterForDest(temp, r1, r2);
            
            loadValue(r1, op1);
            loadValue(r2, op2);
            
            sprintf(code, "  ; Computing %s %s %s", op1, op, op2);
            emitStack(code);
            
            if (strcmp(op, "+") == 0)
                sprintf(code, "  ADD %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);
            else if (strcmp(op, "-") == 0)
                sprintf(code, "  SUB %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);
            else if (strcmp(op, "*") == 0)
                sprintf(code, "  MUL %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);
            else if (strcmp(op, "/") == 0)
                sprintf(code, "  DIV %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);
            else if (strcmp(op, ">") == 0)
                sprintf(code, "  GT %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);
            else if (strcmp(op, "<") == 0)
                sprintf(code, "  LT %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);
            else if (strcmp(op, "==") == 0)
                sprintf(code, "  EQ %s, %s, %s", 
                        registers[dest].name, registers[r1].name, registers[r2].name);

            emitStack(code);
            strcpy(registers[dest].tempVar, temp);
            continue;
        }
        
        /* Assignment to temp: t = value */
        if (sscanf(line, "%s = %s", temp, val) == 2 && temp[0] == 't')
        {
            int r = allocateRegisterForDest(temp, -1, -1);
            loadValue(r, val);
            strcpy(registers[r].tempVar, temp);
            continue;
        }
        
        /* Assignment: var = value */
        if (sscanf(line, "%s = %s", var, val) == 2)
        {
            int r = allocateRegister(val);
            loadValue(r, val);
            storeValue(r, var);
            registers[r].inUse = 0;
            registers[r].tempVar[0] = '\0';
            continue;
        }
        
        /* Print statement: print value */
        if (sscanf(line, "print %s", val) == 1)
        {
            int r = allocateRegister(val);
            loadValue(r, val);
            sprintf(code, "  PRINT %s", registers[r].name);
            emitStack(code);
            registers[r].inUse = 0;
            registers[r].tempVar[0] = '\0';
            continue;
        }
        
        /* Conditional: ifFalse cond goto label */
        if (sscanf(line, "ifFalse %s goto %s", cond, label) == 2)
        {
            int r = allocateRegister(cond);
            loadValue(r, cond);
            sprintf(code, "  JFALSE %s, %s", registers[r].name, label);
            emitStack(code);
            registers[r].inUse = 0;
            registers[r].tempVar[0] = '\0';
            continue;
        }
        
        /* Unconditional jump: goto label */
        if (sscanf(line, "goto %s", label) == 1)
        {
            sprintf(code, "  JMP %s", label);
            emitStack(code);
            continue;
        }
    }
    
    emitStack("");
    emitStack("; Data section (local variables)");
    emitStack(".data:");
    for (int i = 0; i < varCount; i++)
    {
        sprintf(code, "  %s: .word 0", varStorage[i].name);
        emitStack(code);
    }
    
    printf("Stack machine code generated (%d instructions)\n", stackIndex);
}

void printStackMachineTAC()
{
    printf("\n=== STACK MACHINE CODE ===\n");
    for (int i = 0; i < stackIndex; i++)
        printf("%s\n", stackCode[i]);
}

void printStackMachineCode(FILE* file)
{
    fprintf(file, "\n=== STACK MACHINE CODE ===\n");
    for (int i = 0; i < stackIndex; i++)
        fprintf(file, "%s\n", stackCode[i]);
}
