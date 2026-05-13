#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <stdio.h>

/* Code optimization module */

/* Optimize the TAC code:
   - Constant folding
   - Dead code elimination
   - Redundant temporary removal
   - Algebraic simplifications */
void optimizeTAC();

/* Print statistics about optimizations applied */
void printOptimizationStats();

#endif
