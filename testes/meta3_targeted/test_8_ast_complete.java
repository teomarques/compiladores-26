// TEST 8: Comprehensive valid program that exercises ALL annotation cases

class TestASTComplete {
    public static int counter;
    public static double rate;
    public static boolean active;

    public static int factorial(int n) {
        int r;
        if (n <= 1) {
            r = 1;
        } else {
            r = n * factorial(n - 1);
        }
        return r;
    }

    public static double power(double base, int exp) {
        double r;
        int i;
        r = 1.0;
        i = 0;
        while (i < exp) {
            r = r * base;
            i = i + 1;
        }
        return r;
    }

    public static boolean isEven(int n) {
        return n - (n / 2) * 2 == 0;
    }

    public static void main(String[] args) {
        int a;
        int b;
        double d;
        boolean x;

        a = Integer.parseInt(args[0]);
        b = Integer.parseInt(args[1]);

        // Various expressions to annotate
        counter = a + b;
        rate = power(2.0, a);
        active = isEven(a);

        // Boolean expressions
        x = active && (a > 0);
        x = !active || (b < 0);

        // Method calls in print
        System.out.print(factorial(a));
        System.out.print(power(rate, b));
        System.out.print(isEven(counter));

        // Assign with method calls
        a = factorial(b);
        d = power(2.5, a);

        // Length and parseInt with calculation
        if (args.length > 0) {
            a = Integer.parseInt(args[args.length - 1]);
        }

        // Nested calls
        a = factorial(factorial(b));
        d = power(power(2.0, 1), 2);

        // Mixed type expressions (widening)
        d = a + b;
        d = a * 1.5;
        d = (a + b) * 2.0;
    }
}
