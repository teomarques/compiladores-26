// TEST 3: Various semantic errors in expressions and statements
// Tests EVERY error type we need to detect

class TestExpressionErrors {
    public static void main(String[] args) {
        int a;
        double b;
        boolean x;

        // Type narrowing errors (assignment)
        a = b;          // double -> int (narrowing) ERROR
        a = x;          // bool -> int ERROR
        x = a;          // int -> bool ERROR
        x = b;          // double -> bool ERROR
        b = x;          // bool -> double ERROR

        // Arithmetic with bool
        a = x + 1;      // bool + int ERROR
        a = x * b;      // bool * double ERROR
        a = 1 - x;      // int - bool ERROR
        a = 1 / x;      // int / bool ERROR
        a = 1 % x;      // int % bool ERROR

        // Logical with non-bool
        x = a && b;     // int && double ERROR
        x = a || x;     // int || bool ERROR
        x = !a;         // !int ERROR

        // Shifts with non-int
        a = a << b;     // int << double ERROR
        a = b >> 1;     // double >> int ERROR
        a = x << 1;     // bool << int ERROR

        // Comparison with bool
        x = x < 1;      // bool < int ERROR
        x = x > b;      // bool > double ERROR
        x = x <= a;     // bool <= int ERROR

        // Equality with String[]
        x = args == args;   // String[] == String[] ERROR

        // Length on wrong types
        a = a.length;       // int.length ERROR
        a = b.length;       // double.length ERROR
        a = x.length;       // bool.length ERROR

        // Print errors
        System.out.print(args);     // String[] in print ERROR

        // If with non-bool
        if (a) {}           // int in if ERROR
        if (b) {}           // double in if ERROR

        // While with non-bool
        while (a) {}        // int in while ERROR
        while (b) {}        // double in while ERROR

        // Return error in void main
        return 5;           // returning int from void ERROR
    }
}
