// TEST 7: Nested assignments and complex expressions
// Tests AST annotation in deeply nested contexts

class TestNestedAssignments {
    public static int identity(int x) {
        return x;
    }

    public static void main(String[] args) {
        int a;
        int b;
        int c;
        double d;
        double e;
        boolean x;

        // Chained assignments
        a = b = c = 5;          // all should be int

        // Assignment in expression (boolean context)
        if ((a = 5) == 5) {     // valid: assign returns int, == returns bool
        }

        // Assignment as condition
        if (a = 10) {           // ERROR: int condition (assign returns int)
        }

        // Method call in assignment
        a = identity(b = 5);    // valid: b=5 is int, identity returns int

        // Print with assignment
        System.out.print(a = b = c = 1);

        // Complex nested
        a = identity(identity(identity(b)));    // triple nested
        a = identity(b + identity(c) * 2);

        // Boolean contexts
        x = (a = b) == (b = c);     // OK
        x = (a = 5) > (c = 3);      // OK

        // Mixed valid/invalid
        d = a = 5;              // ERROR: a=5 is int, d is double (widening OK)
                                // Actually this should be OK due to widening
        e = a = b = c = 1;      // chain ending in int, e is double, widening OK
    }
}
