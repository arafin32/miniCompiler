# MiniCompiler - 100% COMPLETE ✅

A complete compiler for **MiniLang**, a minimal C-like programming language. This project implements a full compiler pipeline with lexical analysis, syntax analysis, semantic analysis, code optimization, intermediate code generation, and target code generation.

**Current Status:** 100% COMPLETE ✅ - All compiler phases fully implemented and tested

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Language Syntax](#language-syntax)
- [Architecture](#architecture)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [Examples](#examples)
- [Project Structure](#project-structure)
- [Implementation Details](#implementation-details)

---

## Overview

**MiniCompiler** is a complete educational compiler that translates MiniLang source code through all compilation phases to produce **stack machine pseudo-assembly code**. The compiler implements all major compilation phases:

1. **Lexical Analysis** - Tokenization using Flex (100%)
2. **Syntax Analysis** - Parsing using Bison (100%)
3. **Abstract Syntax Tree (AST)** - Building the program's structure (100%)
4. **Semantic Analysis** - Type checking and symbol table management (100%)
5. **Code Optimization** - Constant folding, dead code elimination (100%)
6. **Intermediate Code Generation** - Three-Address Code (TAC) (100%)
7. **Target Code Generation** - Stack machine pseudo-assembly (100%)

This project demonstrates complete compiler design with advanced optimization techniques.

---

## Features

### Language Features (Complete)
- ✅ **Variable declarations** - `int` and `bool` types
- ✅ **Arithmetic expressions** - Addition, subtraction, multiplication, division
- ✅ **Comparison operators** - Equal to (`==`), not equal (`!=`), greater than (`>`), less than (`<`)
- ✅ **Control flow** - `if`, `else`, and `while` statements
- ✅ **Variable assignments** - Simple and compound assignments
- ✅ **I/O operations** - `print` statement for output
- ✅ **Comments** - Single-line comments using `//`
- ✅ **Block scoping** - Nested block scopes with `{ }`

### Compiler Features (Complete)
- ✅ **AST visualization** - Prints the parsed Abstract Syntax Tree
- ✅ **TAC generation** - Produces Three-Address Code intermediate representation
- ✅ **Type checking** - Complete type inference and compatibility checking
- ✅ **Error detection** - Undeclared variables, duplicate declarations, type errors
- ✅ **Code optimization** - Multiple optimization passes
- ✅ **Stack machine code** - Pseudo-assembly target code generation

---

## Language Syntax

### Basic Elements

**Data Types:**
```minilang
int x;      // Integer variable
bool flag;  // Boolean variable
```

**Variables and Constants:**
- Identifiers: `[a-zA-Z_][a-zA-Z0-9_]*`
- Integers: `[0-9]+`
- Booleans: `true`, `false`

### Operators

**Arithmetic Operators:**
```minilang
a + b       // Addition
a - b       // Subtraction
a * b       // Multiplication
a / b       // Division
```

**Comparison Operators:**
```minilang
a == b      // Equal to
a != b      // Not equal to
a > b       // Greater than
a < b       // Less than
```

### Control Structures

**If Statement:**
```minilang
if (x > 5)
    x = x - 1;
```

**If-Else Statement:**
```minilang
if (x == 0)
    x = 1;
else
    x = 2;
```

**While Loop:**
```minilang
while (i < 10)
    i = i + 1;
```

### Output

**Print Statement:**
```minilang
print x;    // Print the value of x
```

### Comments

```minilang
// This is a single-line comment
x = 5;  // Comments can be inline too
```

---

## Recent Fixes & Test Suite

- Date: July 20, 2026 — small post-completion maintenance and bug fixes.
- Fixes applied:
    - Conservative dead-store elimination (preserve named-variable stores/initializations).
    - Stronger semantic/type checks (undeclared identifiers in expressions, if/while condition typing, division-by-zero detection).
    - Boolean literal lowering to constants in TAC (`true` → `1`, `false` → `0`).
    - Stack-machine codegen fixes: label emission, register-spill safety, and duplicate header removal.
    - Added a comprehensive error-handling test suite under `tests/` (error01..error20).

Run the new error-suite with:
```bash
for f in tests/error*.ml; do echo "=== $f ==="; ./build/minicompiler < "$f"; done
```


## Architecture

The MiniCompiler implements a complete 7-phase compilation pipeline:

```
Source Code (.ml)
       ↓
┌──────────────────────────────────────┐
│  1. LEXICAL ANALYSIS (Flex)          │
│     Tokenizes input with line tracking
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  2. SYNTAX ANALYSIS (Bison)          │
│     Parses tokens into AST           │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  3. SEMANTIC ANALYSIS                │
│     Type checking, scope validation  │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  4. THREE-ADDRESS CODE GENERATION    │
│     Generates intermediate TAC       │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  5. CODE OPTIMIZATION (NEW)          │
│     Constant folding, dead code elim │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  6. STACK MACHINE CODE GENERATION    │
│     Pseudo-assembly generation       │
└──────────────────────────────────────┘
       ↓
    output.tac (Complete compiler output)
    - TAC code
    - Optimization statistics
    - Stack machine code
```

### Key Modules

| Module | File | Responsibility | Status |
|--------|------|------------------|---------|
| **Lexer** | `lexer.l` | Tokenizes input using Flex | ✅ 100% |
| **Parser** | `parser.y` | Parses tokens and builds AST using Bison | ✅ 100% |
| **AST** | `ast.c/h` | Manages Abstract Syntax Tree nodes | ✅ 100% |
| **Symbol Table** | `symbol_table.c/h` | Stores variable declarations and scope info | ✅ 100% |
| **Semantic Analysis** | `semantic.c/h` | Type checking and semantic validation | ✅ 100% |
| **TAC Generation** | `tac.c/h` | Generates Three-Address Code | ✅ 100% |
| **Code Optimizer** | `optimizer.c/h` | Constant folding, dead code elimination | ✅ 100% (NEW) |
| **Stack Machine** | `codegen_extended.c/h` | Generates pseudo-assembly code | ✅ 100% (NEW) |
| **Main Driver** | `main.c` | Orchestrates the compilation pipeline | ✅ 100% |

---

## Building the Project

### Prerequisites

You need the following tools installed:
- **gcc** - GNU C Compiler
- **flex** - Lexical analyzer generator
- **bison** - Parser generator
- **make** - Build automation tool

On Ubuntu/Debian:
```bash
sudo apt-get install gcc flex bison make
```

On macOS (with Homebrew):
```bash
brew install gcc flex bison make
```

### Compilation

Build the compiler using the provided Makefile:

```bash
make
```

This generates the build artifacts in `build/`:
- `build/lex.yy.c` - Lexer source (from `src/lexer.l`)
- `build/parser.tab.c` / `build/parser.tab.h` - Parser source (from `src/parser.y`)
- `build/*.o` - Compiled object files
- `build/minicompiler` - The compiler executable

### Clean Up

Remove generated build files and rebuild:

```bash
make clean
make
```

---

## Usage

### Basic Syntax

```bash
./build/minicompiler < tests/testcases/test1.ml
```

The compiler reads MiniLang source code from standard input and outputs:
1. Parsing status
2. Abstract Syntax Tree (AST)
3. Generated Three-Address Code (TAC)

### Example Execution

```bash
./build/minicompiler < tests/testcases/test1.ml
```

### Interactive Mode

You can also pipe code directly:

```bash
echo "x = 5 + 3;" | ./build/minicompiler
```

---

## Examples

### Example 1: Simple Arithmetic

**Input** (`test1.ml`):
```minilang
x = 5 + 3;
```

**Output:**
```
MiniLang Compiler Started...

Parsing successful!

=== AST ===
[AST visualization showing the expression tree]

=== TAC ===
[Three-Address Code instructions]
```

### Example 2: Operator Precedence

**Input** (`test2.ml`):
```minilang
x = 5 + 3 * 2;
```

The compiler respects operator precedence: multiplication is evaluated before addition.

### Example 3: Conditional Statement

**Input** (`test3.ml`):
```minilang
if (x > 5)
    x = 1;
```

### Example 4: Control Flow

**Input** (`test4.ml`):
```minilang
i = 0;
while (i < 10)
    i = i + 1;
```

---

## Project Structure

```
MiniCompiler/
├── src/                    # Compiler source code
│   ├── ast.c
│   ├── ast.h
│   ├── codegen.c
│   ├── codegen.h
│   ├── codegen_extended.c
│   ├── codegen_extended.h
│   ├── lexer.l
│   ├── parser.y
│   ├── main.c
│   ├── optimizer.c
│   ├── optimizer.h
│   ├── semantic.c
│   ├── semantic.h
│   ├── symbol_table.c
│   ├── symbol_table.h
│   ├── tac.c
│   └── tac.h
├── build/                  # Generated build artifacts
│   ├── lex.yy.c
│   ├── parser.tab.c
│   ├── parser.tab.h
│   ├── ast.o
│   ├── symbol_table.o
│   ├── semantic.o
│   ├── tac.o
│   ├── optimizer.o
│   ├── codegen_extended.o
│   ├── main.o
│   ├── minicompiler
│   └── output.tac
├── docs/                   # Documentation and reports
│   ├── FINAL_REPORT_100_PERCENT.md
│   └── Compiler_Lab_guidelines.pdf
├── tests/                  # All test programs and cases
│   ├── final_demo.ml
│   ├── ifelse_test.ml
│   ├── simple_test.ml
│   ├── test_complex.ml
│   ├── test_comprehensive.ml
│   ├── test_comprehensive2.ml
│   ├── test_control.ml
│   ├── test_dup.ml
│   ├── test_ifelse.ml
│   ├── test_prog.ml
│   ├── test_while.ml
│   └── testcases/
│       ├── test1.ml
│       ├── test2.ml
│       ├── test3.ml
│       └── test4.ml
├── .gitignore
├── Makefile
└── README.md
```

### Generated Files (after `make`)

- `build/lex.yy.c` - Lexer implementation (generated from `src/lexer.l`)
- `build/parser.tab.c` / `build/parser.tab.h` - Parser implementation (generated from `src/parser.y`)
- `build/*.o` - Compiled object files
- `build/minicompiler` - The executable compiler
- `build/output.tac` - Output TAC / generated code

---

## Implementation Details

### Lexical Analysis (Flex)

The lexer (`lexer.l`) tokenizes the input by recognizing:
- **Keywords**: `if`, `else`, `while`, `int`, `bool`, `print`
- **Operators**: `+`, `-`, `*`, `/`, `==`, `!=`, `>`, `<`, `=`
- **Delimiters**: `(`, `)`, `{`, `}`, `;`
- **Identifiers**: Variable names and function identifiers
- **Numbers**: Integer literals
- **Comments**: Lines starting with `//`

### Syntax Analysis (Bison)

The parser (`parser.y`) builds an Abstract Syntax Tree using:
- **Grammar rules** for statements, expressions, and control flow
- **Operator precedence** declarations for correct evaluation order
- **Type system** supporting `int` and `bool`
- **Semantic actions** that create AST nodes during parsing

### Three-Address Code (TAC)

TAC is an intermediate representation where:
- Each instruction has at most 3 operands
- Form: `result = operand1 op operand2`
- Used for control flow representation
- Simplifies optimization and code generation

Example TAC instructions:
```
t1 = 5
t2 = 3
t3 = t1 + t2
x = t3
```

### Code Optimization (NEW - 100%)

The optimizer performs 4 independent optimization passes:

1. **Constant Folding**
   - Detects: `t = const1 op const2`
   - Result: `t = result_value`
   - Example: `t1 = 5 + 3` → `t1 = 8`

2. **Algebraic Simplification**
   - `a + 0` → `a`
   - `a * 1` → `a`
   - `a / a` → `1`
   - And more identity eliminations

3. **Dead Code Elimination**
   - Removes assignments to unused temporaries
   - Reduces code size and execution time

4. **Redundant Temporary Removal**
   - Detects: `t1 = t2` patterns
   - Bypasses unnecessary copies
   - Reduces temporary variables

### Stack Machine Code Generation (NEW - 100%)

Generates pseudo-assembly for a simple stack machine:
- 4 registers (r0, r1, r2, r3)
- Memory-based variable storage
- Linear variable allocation
- Support for arithmetic, comparison, jumps, and I/O

Example output:
```asm
LOAD r0, #5       ; Load constant 5
STORE [bp-0], r0  ; Store to variable x
LOAD r0, [bp-0]   ; Load variable x
PRINT r0          ; Print value
```

### Symbol Table

Maintains information about:
- Variable names and types
- Scope information
- Declaration locations
- Type validation

### Type System

The compiler supports:
- **`int`** - Integer type (32-bit)
- **`bool`** - Boolean type (true/false)
- Type checking during semantic analysis

---

## Contributing

This is an educational project. If you'd like to extend it, consider adding:
- Function declarations and calls
- Arrays and pointers
- String type
- More operators (modulo, bitwise operations)
- Optimization passes
- Backend code generation (x86, LLVM)
- Better error messages with line numbers

---

## License

This project is provided as-is for educational purposes.

---

## Related Resources

- **Flex Documentation**: https://westes.github.io/flex/manual/
- **Bison Documentation**: https://www.gnu.org/software/bison/manual/
- **Compiler Design**: "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- **Three-Address Code**: https://en.wikipedia.org/wiki/Three-address_code
