#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tac.h"

extern char tacCode[300][120];
extern int tacIndex;

static int constant_folding_count = 0;
static int dead_code_count = 0;
static int redundant_temp_count = 0;

/* Check if a string is a number */
int isNumber(const char* str)
{
    if (!str || !*str) return 0;
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i])) return 0;
    return 1;
}

static int isTokenChar(char c)
{
    return (isalnum(c) || c == '_');
}

static int tokenMatch(const char* line, const char* token)
{
    const char* found = strchr(line, token[0]);
    while (found)
    {
        if (strncmp(found, token, strlen(token)) == 0)
        {
            char before = (found == line) ? '\0' : *(found - 1);
            char after = *(found + strlen(token));
            int ok_before = (before == '\0' || !isTokenChar(before));
            int ok_after = (after == '\0' || !isTokenChar(after));
            if (ok_before && ok_after)
                return 1;
        }
        found = strchr(found + 1, token[0]);
    }
    return 0;
}

/* Perform constant folding */
void constantFolding()
{
    char optimized[300][120];
    int newIndex = 0;
    int folded_in_pass = 0;
    
    for (int i = 0; i < tacIndex; i++)
    {
        char line[120];
        strcpy(line, tacCode[i]);
        
        /* Check if this is a constant folding opportunity: t = c1 op c2 */
        char temp[20], op1[20], op[10], op2[20];
        if (sscanf(line, "%s = %s %s %s", temp, op1, op, op2) == 4)
        {
            if (isNumber(op1) && isNumber(op2) && temp[0] == 't')
            {
                int val1 = atoi(op1);
                int val2 = atoi(op2);
                int result;
                int canFold = 1;
                
                if (strcmp(op, "+") == 0) result = val1 + val2;
                else if (strcmp(op, "-") == 0) result = val1 - val2;
                else if (strcmp(op, "*") == 0) result = val1 * val2;
                else if (strcmp(op, "/") == 0 && val2 != 0) result = val1 / val2;
                else canFold = 0;
                
                if (canFold && val2 != 0)
                {
                    /* Replace all uses of this temp with the constant */
                    for (int j = i + 1; j < tacIndex; j++)
                    {
                        const char* found = strchr(tacCode[j], temp[0]);
                        while (found)
                        {
                            if (strncmp(found, temp, strlen(temp)) == 0)
                            {
                                char before = (found == tacCode[j]) ? '\0' : *(found - 1);
                                char after = *(found + strlen(temp));
                                int ok_before = (before == '\0' || !isTokenChar(before));
                                int ok_after = (after == '\0' || !isTokenChar(after));
                                if (ok_before && ok_after)
                                {
                                    char beforeBuf[120];
                                    char afterBuf[120];
                                    int pos = found - tacCode[j];
                                    strncpy(beforeBuf, tacCode[j], pos);
                                    beforeBuf[pos] = '\0';
                                    sprintf(afterBuf, "%d", result);
                                    strcat(beforeBuf, afterBuf);
                                    strcat(beforeBuf, found + strlen(temp));
                                    strcpy(tacCode[j], beforeBuf);
                                    break;
                                }
                            }
                            found = strchr(found + 1, temp[0]);
                        }
                    }
                    
                    /* Don't add this line - constant is folded */
                    constant_folding_count++;
                    folded_in_pass++;
                    continue;
                }
            }
        }
        
        strcpy(optimized[newIndex++], line);
    }
    
    /* Copy back optimized code */
    tacIndex = newIndex;
    for (int i = 0; i < tacIndex; i++)
        strcpy(tacCode[i], optimized[i]);
}

/* Remove dead code (unused temporary assignments) */
void deadCodeElimination()
{
    char optimized[300][120];
    int newIndex = 0;
    
    for (int i = 0; i < tacIndex; i++)
    {
        char temp[20];
        
        /* Check if this line assigns a temporary: t = ... */
        if (sscanf(tacCode[i], "%s =", temp) == 1 && temp[0] == 't')
        {
            /* Count how many times this temp is used AFTER this line */
            int used_later = 0;
            for (int j = i + 1; j < tacIndex; j++)
            {
                if (tokenMatch(tacCode[j], temp))
                {
                    used_later++;
                    break;
                }
            }
            
            /* If not used, skip this line (dead code) */
            if (!used_later)
            {
                dead_code_count++;
                continue;
            }
        }
        
        strcpy(optimized[newIndex++], tacCode[i]);
    }
    
    /* Copy back optimized code */
    tacIndex = newIndex;
    for (int i = 0; i < tacIndex; i++)
        strcpy(tacCode[i], optimized[i]);
}

/* Remove redundant assignments: t1 = t2; t3 = t1; becomes t3 = t2; */
void redundantTemporaryRemoval()
{
    char optimized[300][120];
    int newIndex = 0;
    char tempMap[100][20];  /* map from temp to source */
    int mapSize = 0;
    
    for (int i = 0; i < tacIndex; i++)
    {
        char dest[20], src[20];
        
        /* Check for: t_dest = t_src pattern */
        if (sscanf(tacCode[i], "%s = %s", dest, src) == 2 && 
            dest[0] == 't' && src[0] == 't' && strcmp(dest, src) != 0)
        {
            /* Check if src maps to something else */
            for (int j = 0; j < mapSize; j++)
            {
                if (strcmp(tempMap[j], src) == 0)
                {
                    /* Found mapping, create new assignment */
                    char newLine[120];
                    sprintf(newLine, "%s = %s", dest, tempMap[j + 100]);
                    strcpy(tacCode[i], newLine);
                    redundant_temp_count++;
                    break;
                }
            }
            
            /* Register this mapping */
            strcpy(tempMap[mapSize], dest);
            strcpy(tempMap[mapSize + 100], src);
            mapSize++;
        }
        
        strcpy(optimized[newIndex++], tacCode[i]);
    }
    
    /* Copy back optimized code */
    tacIndex = newIndex;
    for (int i = 0; i < tacIndex; i++)
        strcpy(tacCode[i], optimized[i]);
}

/* Algebraic simplifications: a + 0, a * 0, a * 1, etc. */
void algebraicSimplification()
{
    for (int i = 0; i < tacIndex; i++)
    {
        char line[120];
        char temp[20], op1[20], op[10], op2[20];
        
        strcpy(line, tacCode[i]);
        
        if (sscanf(line, "%s = %s %s %s", temp, op1, op, op2) != 4)
            continue;
        
        int modified = 0;
        
        /* a + 0 = a */
        if (strcmp(op, "+") == 0 && strcmp(op2, "0") == 0)
        {
            sprintf(tacCode[i], "%s = %s", temp, op1);
            modified = 1;
        }
        /* 0 + a = a */
        else if (strcmp(op, "+") == 0 && strcmp(op1, "0") == 0)
        {
            sprintf(tacCode[i], "%s = %s", temp, op2);
            modified = 1;
        }
        /* a - 0 = a */
        else if (strcmp(op, "-") == 0 && strcmp(op2, "0") == 0)
        {
            sprintf(tacCode[i], "%s = %s", temp, op1);
            modified = 1;
        }
        /* a * 0 = 0 */
        else if (strcmp(op, "*") == 0 && (strcmp(op1, "0") == 0 || strcmp(op2, "0") == 0))
        {
            sprintf(tacCode[i], "%s = 0", temp);
            modified = 1;
        }
        /* a * 1 = a */
        else if (strcmp(op, "*") == 0 && strcmp(op2, "1") == 0)
        {
            sprintf(tacCode[i], "%s = %s", temp, op1);
            modified = 1;
        }
        /* 1 * a = a */
        else if (strcmp(op, "*") == 0 && strcmp(op1, "1") == 0)
        {
            sprintf(tacCode[i], "%s = %s", temp, op2);
            modified = 1;
        }
        /* a / 1 = a */
        else if (strcmp(op, "/") == 0 && strcmp(op2, "1") == 0)
        {
            sprintf(tacCode[i], "%s = %s", temp, op1);
            modified = 1;
        }
        /* a / a = 1 */
        else if (strcmp(op, "/") == 0 && strcmp(op1, op2) == 0)
        {
            sprintf(tacCode[i], "%s = 1", temp);
            modified = 1;
        }
    }
}

/* Main optimization function */
void optimizeTAC()
{
    printf("\n=== OPTIMIZATION PHASE ===\n");
    
    /* Apply optimizations in order */
    printf("Applying constant folding...\n");
    constantFolding();
    
    printf("Applying algebraic simplification...\n");
    algebraicSimplification();
    
    printf("Removing dead code...\n");
    deadCodeElimination();
    
    printf("Removing redundant temporaries...\n");
    redundantTemporaryRemoval();
    
    printf("Optimization complete\n");
}

/* Print optimization statistics */
void printOptimizationStats()
{
    printf("\n=== OPTIMIZATION STATISTICS ===\n");
    printf("Constant folding optimizations: %d\n", constant_folding_count);
    printf("Dead code eliminations: %d\n", dead_code_count);
    printf("Redundant temporary removals: %d\n", redundant_temp_count);
    printf("Total optimizations: %d\n", 
           constant_folding_count + dead_code_count + redundant_temp_count);
}
