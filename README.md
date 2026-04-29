# MiniCompiler

A compiler for **MiniLang**, a minimal C-like programming language. This project implements a complete compiler frontend with lexical analysis, syntax analysis, semantic analysis, and Three-Address Code (TAC) generation.

**Current Status:** 50% Complete ✅ - All core compiler phases working

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

**MiniCompiler** is an educational compiler that translates MiniLang source code into **Three-Address Code (TAC)**, an intermediate representation commonly used in real-world compilers. The compiler performs all major compilation phases:

1. **Lexical Analysis** - Tokenization using Flex
2. **Syntax Analysis** - Parsing using Bison
3. **Abstract Syntax Tree (AST)** - Building the program's structure
4. **Semantic Analysis** - Type checking and symbol table management
5. **Code Generation** - Producing Three-Address Code (TAC)

This project serves as a learning tool for understanding how compilers work internally.

---

## Features

- ✅ **Variable declarations** - `int` and `bool` types
- ✅ **Arithmetic expressions** - Addition, subtraction, multiplication, division
- ✅ **Comparison operators** - Equal to (`==`), not equal (`!=`), greater than (`>`), less than (`<`)
- ✅ **Control flow** - `if`, `else`, and `while` statements
- ✅ **Variable assignments** - Simple and compound assignments
- ✅ **I/O operations** - `print` statement for output
- ✅ **Comments** - Single-line comments using `//`
- ✅ **AST visualization** - Prints the parsed Abstract Syntax Tree
- ✅ **TAC generation** - Produces Three-Address Code intermediate representation

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

## Architecture

The MiniCompiler is structured as a multi-phase compiler pipeline:

```
Source Code (.ml)
       ↓
┌──────────────────────────────────────┐
│  1. LEXICAL ANALYSIS (Flex)          │
│     Tokenizes input                  │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  2. SYNTAX ANALYSIS (Bison)          │
│     Parses tokens into AST           │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  3. SEMANTIC ANALYSIS                │
│     Type checking & symbol table     │
└──────────────────────────────────────┘
       ↓
┌──────────────────────────────────────┐
│  4. CODE GENERATION                  │
│     Generates TAC (Three-Address Code)
└──────────────────────────────────────┘
       ↓
    TAC Output
```

### Key Modules

| Module | File | Responsibility |
|--------|------|-----------------|
| **Lexer** | `lexer.l` | Tokenizes input using Flex |
| **Parser** | `parser.y` | Parses tokens and builds AST using Bison |
| **AST** | `ast.c/h` | Manages Abstract Syntax Tree nodes |
| **Symbol Table** | `symbol_table.c/h` | Stores variable declarations and scope info |
| **Semantic Analysis** | `semantic.c/h` | Validates types and checks semantic rules |
| **Code Generation** | `tac.c/h` | Generates Three-Address Code |
| **Main Driver** | `main.c` | Orchestrates the compilation pipeline |

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

This generates:
- `lex.yy.c` - Lexer source (from `lexer.l`)
- `parser.tab.c` - Parser source (from `parser.y`)
- `*.o` - Object files
- `minicompiler` - The final executable

### Clean Up

Remove generated files and rebuild:

```bash
make clean
make
```

---

## Usage

### Basic Syntax

```bash
./minicompiler < input_file.ml
```

The compiler reads MiniLang source code from standard input and outputs:
1. Parsing status
2. Abstract Syntax Tree (AST)
3. Generated Three-Address Code (TAC)

### Example Execution

```bash
./minicompiler < testcases/test1.ml
```

### Interactive Mode

You can also pipe code directly:

```bash
echo "x = 5 + 3;" | ./minicompiler
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
├── ast.c              # AST node creation and printing
├── ast.h              # AST data structures
├── codegen.c          # Code generation utilities
├── codegen.h          # Code generation headers
├── lexer.l            # Flex lexical analyzer specification
├── parser.y           # Bison parser specification
├── main.c             # Compiler driver
├── semantic.c         # Semantic analysis implementation
├── semantic.h         # Semantic analysis headers
├── symbol_table.c     # Symbol table implementation
├── symbol_table.h     # Symbol table definitions
├── tac.c              # Three-Address Code generation
├── tac.h              # TAC definitions
├── Makefile           # Build configuration
├── testcases/         # Test input files
│   ├── test1.ml       # Simple arithmetic
│   ├── test2.ml       # Operator precedence
│   ├── test3.ml       # Conditional statement
│   └── test4.ml       # Loop statement
└── README.md          # This file
```

### Generated Files (after `make`)

- `lex.yy.c` - Lexer implementation (generated from `lexer.l`)
- `parser.tab.c` / `parser.tab.h` - Parser implementation (generated from `parser.y`)
- `*.o` - Compiled object files
- `minicompiler` - The executable compiler

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
