# Testing Guide - 50% Completion

This guide shows how to test the MiniCompiler at its current 50% completion state.

## Quick Start

### Build the compiler
```bash
cd /home/arafin/MiniCompiler
make clean
make
```

### Test a simple program
```bash
echo "int x; x = 5;" | ./minicompiler
cat output.tac
```

---

## Test Cases

### Test 1: Basic Variables and Arithmetic
**File:** `test1.ml`
```minilang
int x;
int y;
x = 5;
y = 3;
x = x + y;
```

**Run:**
```bash
./minicompiler < test1.ml
```

**Expected Output:**
- AST showing block structure
- "Semantic analysis completed successfully"
- `output.tac` contains TAC for arithmetic

**TAC Output:**
```
x = 5
y = 3
t1 = x + y
x = t1
```

---

### Test 2: If-Else Control Flow
**File:** `test2.ml`
```minilang
int x;
x = 5;
if (x > 3)
    x = 1;
else
    x = 0;
```

**Run:**
```bash
./minicompiler < test2.ml
```

**Expected Output:**
- Control flow with labels (L1, L2)
- `ifFalse` and `goto` instructions

**TAC Output:**
```
x = 5
t1 = x > 3
ifFalse t1 goto L1
x = 1
goto L2
L1:
x = 0
L2:
```

---

### Test 3: While Loop
**File:** `test3.ml`
```minilang
int i;
i = 0;
while (i < 5)
    i = i + 1;
```

**Run:**
```bash
./minicompiler < test3.ml
```

**Expected Output:**
- Loop labels with backward jump
- Condition checking with `ifFalse`

**TAC Output:**
```
i = 0
L1:
t1 = i < 5
ifFalse t1 goto L2
t2 = i + 1
i = t2
goto L1
L2:
```

---

### Test 4: Semantic Error - Undeclared Variable
**File:** `test_undeclared.ml`
```minilang
x = 5;
```

**Run:**
```bash
./minicompiler < test_undeclared.ml
```

**Expected Output:**
```
SEMANTIC ERROR: Undeclared variable 'x'
```

---

### Test 5: Semantic Error - Duplicate Declaration
**File:** `test_duplicate.ml`
```minilang
int x;
int x;
```

**Run:**
```bash
./minicompiler < test_duplicate.ml
```

**Expected Output:**
```
SEMANTIC ERROR: Variable 'x' already declared in current scope
```

---

### Test 6: Block Scoping
**File:** `test_scope.ml`
```minilang
int x;
x = 5;
{
    int y;
    y = 3;
    x = x + y;
}
```

**Run:**
```bash
./minicompiler < test_scope.ml
```

**Expected Output:**
- No errors (scope management works)
- TAC generated correctly

---

### Test 7: Complex Expression
**File:** `test_expr.ml`
```minilang
int a;
int b;
int c;
a = 5;
b = 3;
c = 2;
a = a + b * c;
```

**Run:**
```bash
./minicompiler < test_expr.ml
```

**Expected Output:**
- Respects operator precedence (* before +)
- Temporaries generated correctly

**TAC Output:**
```
a = 5
b = 3
c = 2
t1 = b * c
t2 = a + t1
a = t2
```

---

## Testing Workflow

### 1. Verify Compilation
```bash
make clean && make
```
Should complete with no errors.

### 2. Run a Test
```bash
./minicompiler < testcases/test1.ml
```

### 3. Check Output File
```bash
cat output.tac
```

### 4. Verify Error Handling
```bash
echo "y = 5;" | ./minicompiler
# Should show SEMANTIC ERROR
```

---

## What Works at 50%

| Feature | Status |
|---------|--------|
| Integer variables | ✅ |
| Boolean variables | ✅ |
| Assignments | ✅ |
| Arithmetic operations | ✅ |
| Comparison operators | ✅ |
| If-else statements | ✅ |
| While loops | ✅ |
| Block scoping | ✅ |
| Variable declarations | ✅ |
| Semantic error detection | ✅ |
| TAC generation | ✅ |
| Output to file | ✅ |
| Line number error reporting | ✅ |

---

## What's NOT Yet Implemented

| Feature | Status |
|---------|--------|
| Code optimization | ⏳ In progress |
| Target code generation | ⏳ In progress |
| Type checking (strict) | ⏳ In progress |
| Arrays | ❌ Not planned for Phase 1 |
| Functions | ❌ Not planned for Phase 1 |
| Floating-point | ❌ Not supported |

---

## Debugging

### View the AST
The compiler prints the Abstract Syntax Tree for every compilation:
```
=== AST ===
(block (decl x int) (assign x 5))
```

### Check Symbol Table
The compiler validates all variable declarations and usages.

### View Generated TAC
Always inspect `output.tac`:
```bash
cat output.tac
```

### Enable Verbose Output
The compiler shows parsing status and semantic analysis progress.

---

## Common Issues

### Issue: "SYNTAX ERROR at line X"
**Cause:** Grammar doesn't support the syntax  
**Solution:** Check the expression follows MiniLang rules

### Issue: "SEMANTIC ERROR: Undeclared variable"
**Cause:** Variable used before declaration  
**Solution:** Add `int varname;` before using the variable

### Issue: "output.tac" not created
**Cause:** Compilation failed (check for parsing errors)  
**Solution:** Fix all syntax and semantic errors first

---

## Progress Report

See `PROGRESS_50_PERCENT.md` for detailed completion status.

---

## Next Phases

- **Phase 2 (51-75%):** Code optimization and type checking
- **Phase 3 (76-90%):** Target code generation
- **Phase 4 (91-100%):** Documentation and report writing
