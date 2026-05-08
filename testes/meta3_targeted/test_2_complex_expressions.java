// TEST 2: Complex expressions with various operators
// This is a VALID program - checks complete AST annotation

class TestComplexExpressions {
    public static int compute(int a, int b, int c) {
        return a * (b + c) - (a / b) % c;
    }

    public static boolean check(int x, int y) {
        return (x < y) && (x != y) || (x == y);
    }

    public static void main(String[] args) {
        int a;
        int b;
        int c;
        double d;
        double e;
        boolean x;
        boolean y;

        a = 1;
        b = 2;
        c = 3;
        d = 1.5;
        e = 2.5;
        x = true;
        y = false;

        // Unary operators
        a = -a;
        a = +b;
        x = !x;

        // Bitwise
        a = a << 2;
        a = a >> 1;
        a = a ^ b;
        x = x ^ y;

        // Mixed arithmetic with widening
        d = a + d;
        d = b * d;
        d = a / e;
        d = a % e;

        // Complex method call
        a = compute(a, b, c);
        x = check(a, b);

        // Length and parseInt
        a = args.length;
        a = Integer.parseInt(args[a - 1]);

        // Print various types
        System.out.print(a);
        System.out.print(d);
        System.out.print(x);
        System.out.print("hello");
        System.out.print(args.length);
        System.out.print(compute(a, b, c));
    }
}
