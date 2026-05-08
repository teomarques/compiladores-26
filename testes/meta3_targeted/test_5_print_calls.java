// TEST 5: Tests print and method calls with various combinations

class TestPrintCalls {
    public static void voidF() {
    }

    public static int intF(int x) {
        return x;
    }

    public static double doubleF() {
        return 1.0;
    }

    public static boolean boolF() {
        return true;
    }

    public static void main(String[] args) {
        int a;
        double b;
        boolean x;

        a = 1;
        b = 2.0;
        x = true;

        // Valid prints
        System.out.print(a);            // int OK
        System.out.print(b);            // double OK
        System.out.print(x);            // bool OK
        System.out.print("hello");      // String literal OK
        System.out.print(args.length);  // int (length) OK
        System.out.print(intF(a));      // int (call) OK
        System.out.print(doubleF());    // double (call) OK
        System.out.print(boolF());      // bool (call) OK

        // Invalid prints
        System.out.print(args);         // String[] ERROR
        System.out.print(voidF());      // void ERROR

        // Calls with type errors
        a = intF(b);                    // intF expects int, got double ERROR
        a = intF(x);                    // intF expects int, got bool ERROR
        a = intF(args);                 // intF expects int, got String[] ERROR
        a = intF();                     // intF expects 1 arg, got 0 ERROR
        a = intF(a, a);                 // intF expects 1 arg, got 2 ERROR
        a = nonExistent();              // ERROR
        a = intF(nonExistent());        // ERROR (cascading)
    }
}
