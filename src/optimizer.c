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
    int len = strlen(token);
    while (found)
    {
        if (strncmp(found, token, len) == 0)
        {
            char before = (found == line) ? '\0' : *(found - 1);
            char after = *(found + len);
            int ok_before = (before == '\0' || !isTokenChar(before));
            int ok_after = (after == '\0' || !isTokenChar(after));
            if (ok_before && ok_after)
                return 1;
        }
        found = strchr(found + 1, token[0]);
    }
    return 0;
}

static void replaceToken(char* line, const char* oldToken, const char* newToken)
{
    char buffer[240] = {0};
    const char* cur = line;
    char* out = buffer;
    int oldLen = strlen(oldToken);
    int newLen = strlen(newToken);

    while (*cur)
    {
        const char* found = strstr(cur, oldToken);
        if (!found)
        {
            strcpy(out, cur);
            break;
        }

        char before = (found == line) ? '\0' : *(found - 1);
        char after = *(found + oldLen);
        int ok_before = (found == line) || !isTokenChar(before);
        int ok_after = (after == '\0') || !isTokenChar(after);

        if (ok_before && ok_after)
        {
            int prefixLen = found - cur;
            memcpy(out, cur, prefixLen);
            out += prefixLen;
            memcpy(out, newToken, newLen);
            out += newLen;
            cur = found + oldLen;
            continue;
        }

        *out++ = *cur++;
    }
    *out = '\0';
    strcpy(line, buffer);
}

static int isLabelLine(const char* line)
{
    const char* colon = strchr(line, ':');
    if (!colon) return 0;
    for (const char* p = line; p < colon; p++)
        if (*p == ' ' || *p == '\t')
            return 0;
    return 1;
}

static int isSimpleCopy(const char* line, char* dest, char* src)
{
    char op1[20], op2[20];
    if (sscanf(line, "%s = %s %s %s", dest, src, op1, op2) == 2)
    {
        /* Only propagate redundant temporaries, not ordinary variable assignments or constants. */
        if (dest[0] != 't')
            return 0;
        if (isNumber(src))
            return 0;
        return 1;
    }
    return 0;
}

static int isValueToken(const char* token)
{
    return token && token[0] != '\0' && !isNumber(token);
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

static void addLive(char live[][20], int* liveCount, const char* token)
{
    if (!token || !*token || isNumber(token)) return;
    for (int i = 0; i < *liveCount; i++)
        if (strcmp(live[i], token) == 0) return;
    strcpy(live[*liveCount], token);
    (*liveCount)++;
}

static int isLive(char live[][20], int liveCount, const char* token)
{
    if (!token || !*token || isNumber(token)) return 0;
    for (int i = 0; i < liveCount; i++)
        if (strcmp(live[i], token) == 0) return 1;
    return 0;
}

static void removeLive(char live[][20], int* liveCount, const char* token)
{
    if (!token || !*token || isNumber(token)) return;
    for (int i = 0; i < *liveCount; i++)
    {
        if (strcmp(live[i], token) == 0)
        {
            for (int j = i; j < *liveCount - 1; j++)
                strcpy(live[j], live[j + 1]);
            (*liveCount)--;
            return;
        }
    }
}

/* Remove dead code using backward liveness analysis */
void deadCodeElimination()
{
    int keep[300] = {0};
    char live[300][20];
    int liveCount = 0;

    for (int i = tacIndex - 1; i >= 0; i--)
    {
        char line[120];
        strcpy(line, tacCode[i]);
        char dest[20], op1[20], op[10], op2[20];

        if (isLabelLine(line))
        {
            keep[i] = 1;
            continue;
        }

        if (sscanf(line, "print %s", op1) == 1)
        {
            addLive(live, &liveCount, op1);
            keep[i] = 1;
            continue;
        }

        if (sscanf(line, "ifFalse %s goto %s", op1, op2) == 2)
        {
            addLive(live, &liveCount, op1);
            keep[i] = 1;
            liveCount = 0;
            continue;
        }

        if (sscanf(line, "goto %s", op1) == 1)
        {
            keep[i] = 1;
            liveCount = 0;
            continue;
        }

        int count = sscanf(line, "%s = %s %s %s", dest, op1, op, op2);
        if (count == 4)
        {
            if (isLive(live, liveCount, dest))
            {
                removeLive(live, &liveCount, dest);
                addLive(live, &liveCount, op1);
                addLive(live, &liveCount, op2);
                keep[i] = 1;
            }
            else
            {
                /* Only remove dead temporaries here. Do not drop stores to named variables
                   because they may be observable (initialization/side-effects). */
                if (dest[0] == 't')
                {
                    dead_code_count++;
                }
                else
                {
                    keep[i] = 1;
                }
            }
            continue;
        }

        if (count == 2)
        {
            if (isLive(live, liveCount, dest))
            {
                removeLive(live, &liveCount, dest);
                addLive(live, &liveCount, op1);
                keep[i] = 1;
            }
            else
            {
                /* Only remove dead temporaries; keep assignments to variables. */
                if (dest[0] == 't')
                {
                    dead_code_count++;
                }
                else
                {
                    keep[i] = 1;
                }
            }
            continue;
        }

        keep[i] = 1;
    }

    char optimized[300][120];
    int newIndex = 0;
    for (int i = 0; i < tacIndex; i++)
    {
        if (keep[i])
            strcpy(optimized[newIndex++], tacCode[i]);
    }

    tacIndex = newIndex;
    for (int i = 0; i < tacIndex; i++)
        strcpy(tacCode[i], optimized[i]);
}

/* Remove redundant assignments and propagate copies */
void redundantTemporaryRemoval()
{
    for (int i = 0; i < tacIndex; i++)
    {
        char line[120];
        strcpy(line, tacCode[i]);
        char dest[20], src[20];

        if (!isSimpleCopy(line, dest, src))
            continue;

        if (strcmp(dest, src) == 0)
            continue;

        int replaced = 0;
        for (int j = i + 1; j < tacIndex; j++)
        {
            char nextDest[20], nextOp1[20], nextOp[10], nextOp2[20];
            if (sscanf(tacCode[j], "%s = %s %s %s", nextDest, nextOp1, nextOp, nextOp2) >= 2)
            {
                if (strcmp(nextDest, dest) == 0)
                    break;
            }

            if (tokenMatch(tacCode[j], dest))
            {
                char before[120];
                strcpy(before, tacCode[j]);
                replaceToken(tacCode[j], dest, src);
                if (strcmp(before, tacCode[j]) != 0)
                {
                    replaced = 1;
                    redundant_temp_count++;
                }
            }
        }

        if (replaced)
        {
            for (int k = i; k < tacIndex - 1; k++)
                strcpy(tacCode[k], tacCode[k + 1]);
            tacIndex--;
            i--;
        }
    }
}

static void removeNoOps()
{
    char optimized[300][120];
    int newIndex = 0;

    for (int i = 0; i < tacIndex; i++)
    {
        char line[120];
        strcpy(line, tacCode[i]);
        char dest[20], src[20];

        if (sscanf(line, "%s = %s", dest, src) == 2 && strcmp(dest, src) == 0)
        {
            redundant_temp_count++;
            continue;
        }

        strcpy(optimized[newIndex++], tacCode[i]);
    }

    tacIndex = newIndex;
    for (int i = 0; i < tacIndex; i++)
        strcpy(tacCode[i], optimized[i]);
}

static int isBranchBoundary(const char* line)
{
    if (isLabelLine(line))
        return 1;
    if (tokenMatch(line, "goto"))
        return 1;
    if (tokenMatch(line, "ifFalse"))
        return 1;
    return 0;
}

static int isVariableAssignment(const char* line, char* dest, char* src)
{
    char op1[20], op2[20];
    if (sscanf(line, "%s = %s %s %s", dest, src, op1, op2) == 2 && dest[0] != 't')
        return 1;
    return 0;
}

void deadStoreElimination()
{
    char optimized[300][120];
    int newIndex = 0;

    for (int i = 0; i < tacIndex; i++)
    {
        char line[120];
        strcpy(line, tacCode[i]);
        char dest[20], src[20];

        if (isVariableAssignment(line, dest, src))
        {
            int canDrop = 0;
            for (int j = i + 1; j < tacIndex; j++)
            {
                if (isBranchBoundary(tacCode[j]))
                    break;

                char nextDest[20], nextSrc[20], nextOp[10], nextOp2[20];
                if (sscanf(tacCode[j], "%s = %s %s %s", nextDest, nextSrc, nextOp, nextOp2) >= 2)
                {
                    if (strcmp(nextDest, dest) == 0)
                    {
                        canDrop = 1;
                        break;
                    }
                }

                if (tokenMatch(tacCode[j], dest))
                    break;
            }

            if (canDrop)
            {
                dead_code_count++;
                continue;
            }
        }

        strcpy(optimized[newIndex++], tacCode[i]);
    }

    tacIndex = newIndex;
    for (int i = 0; i < tacIndex; i++)
        strcpy(tacCode[i], optimized[i]);
}

static void removeDeadTemps()
{
    char optimized[300][120];
    int newIndex = 0;

    for (int i = 0; i < tacIndex; i++)
    {
        char line[120];
        strcpy(line, tacCode[i]);
        char dest[20], src[20], op[10], op2[20];

        if (sscanf(line, "%s = %s %s %s", dest, src, op, op2) == 2 && dest[0] == 't')
        {
            int used = 0;
            for (int j = i + 1; j < tacIndex; j++)
            {
                if (tokenMatch(tacCode[j], dest))
                {
                    used = 1;
                    break;
                }
            }
            if (!used)
            {
                redundant_temp_count++;
                continue;
            }
        }

        strcpy(optimized[newIndex++], tacCode[i]);
    }

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
    }
}

static int containsControlFlow()
{
    for (int i = 0; i < tacIndex; i++)
    {
        if (isLabelLine(tacCode[i]) || tokenMatch(tacCode[i], "goto") || tokenMatch(tacCode[i], "ifFalse"))
            return 1;
    }
    return 0;
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
    
    printf("Removing dead stores...\n");
    deadStoreElimination();
    
    if (containsControlFlow())
    {
        printf("Skipping dead code elimination due to control-flow safety...\n");
    }
    else
    {
        printf("Removing dead code...\n");
        deadCodeElimination();
    }
    
    printf("Removing redundant temporaries...\n");
    redundantTemporaryRemoval();
    
    printf("Cleaning up no-op assignments...\n");
    removeNoOps();
    
    printf("Removing dead temporaries...\n");
    removeDeadTemps();
    
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
