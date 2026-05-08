// TEST 4: Tests return statement errors in various contexts

class TestReturnErrors {
    // void method
    public static void voidMethod() {
        return;             // OK
    }

    public static void voidMethod2() {
        return 5;           // ERROR: returning int from void
    }

    public static void voidMethod3() {
        int x;
        return x;           // ERROR: returning int from void
    }

    // int method
    public static int intMethod() {
        return;             // ERROR: missing value in non-void method
    }

    public static int intMethod2() {
        return 5;           // OK
    }

    public static int intMethod3() {
        return 5.5;         // ERROR: double to int (narrowing)
    }

    public static int intMethod4() {
        return true;        // ERROR: bool to int
    }

    // double method
    public static double doubleMethod() {
        return 5;           // OK (widening)
    }

    public static double doubleMethod2() {
        return 5.5;         // OK
    }

    public static double doubleMethod3() {
        return true;        // ERROR: bool to double
    }

    // boolean method
    public static boolean boolMethod() {
        return true;        // OK
    }

    public static boolean boolMethod2() {
        return 1;           // ERROR: int to bool
    }

    public static boolean boolMethod3() {
        return 1.0;         // ERROR: double to bool
    }

    // void return value
    public static int callsVoid() {
        return voidMethod();    // ERROR: returning void expression
    }

    public static void main(String[] args) {
    }
}
