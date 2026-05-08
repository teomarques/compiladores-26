// TEST 10: Method signature annotation in various contexts
// Tests that the method identifier in calls is annotated with correct signature

class TestMethodSignatureAnnotation {
    public static int noArgs() {
        return 1;
    }

    public static int oneArg(int x) {
        return x;
    }

    public static int twoArgs(int x, int y) {
        return x + y;
    }

    public static int manyArgs(int a, double b, boolean c) {
        return a;
    }

    // Overloaded
    public static int over(int x) {
        return x;
    }

    public static double over(double x) {
        return x;
    }

    public static int over(int x, int y) {
        return x + y;
    }

    // Recursive
    public static int recur(int n) {
        if (n <= 1) {
            return 1;
        }
        return n * recur(n - 1);
    }

    public static void main(String[] args) {
        int a;
        double b;
        boolean x;

        a = 1;
        b = 1.5;
        x = true;

        // No args call - annotated as ()
        a = noArgs();

        // Single arg
        a = oneArg(a);

        // Two args
        a = twoArgs(a, a);

        // Many args with mixed types
        a = manyArgs(a, b, x);

        // Overloaded calls
        a = over(a);                // (int) -> int
        b = over(b);                // (double) -> double
        a = over(a, a);             // (int,int) -> int
        b = over(a);                // (int) -> int, widened to double for assign

        // Recursive call
        a = recur(a);

        // Calls in expressions
        a = oneArg(twoArgs(1, 2));
        a = oneArg(over(a));
        a = manyArgs(oneArg(a), b * 1.0, !x);
    }
}
