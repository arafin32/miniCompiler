int a;
int b;
int c;
int d;
bool flag;

a = 2;
b = 3;

/* Constant folding */
c = (2 + 3) * (4 + 1);

/* Algebraic simplification */
c = c + 0;
c = c * 1;
c = c - 0;
c = c / 1;

/* Dead code elimination */
d = 10;
d = 20;
d = 30;

/* Arithmetic */
a = a + b;
b = a - 1;
c = b * 2;
d = c / 2;

/* Relational */
flag = a < d;

if (flag) {
    c = c + a;
} else {
    c = c - b;
}

/* Loop */
while (c > 10) {
    c = c - 1;
}

print(a);
print(b);
print(c);
print(d);