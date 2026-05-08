// TEST 1: Tests method overloading with type widening
// All calls should resolve correctly without errors
// The AST should annotate the method identifier with its full signature

class TestOverloadingWidening {
    public static int f(int x) {
        return x;
    }

    public static double f(double x) {
        return x;
    }

    public static int g(int x, int y) {
        return x + y;
    }

    public static double g(double x, double y) {
        return x + y;
    }

    public static void main(String[] args) {
        int a;
        double b;

        a = 5;
        b = 3.14;

        // These should all be exact matches
        a = f(a);       // f(int) -> int
        b = f(b);       // f(double) -> double
        a = g(a, a);    // g(int,int) -> int
        b = g(b, b);    // g(double,double) -> double

        // These should use widening
        b = f(a);       // f(double) called with int -> widening
        b = g(a, b);    // g(double,double) called with int,double
        b = g(b, a);    // g(double,double) called with double,int
    }
}
