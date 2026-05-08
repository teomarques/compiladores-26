// TEST 6: Field and method with same name, plus shadowing

class TestFieldMethodInteraction {
    public static int x;
    public static double y;
    public static boolean z;

    // Method with same name as field
    public static int x(int param) {
        return param;
    }

    // Method called y, same as field
    public static double y(double param) {
        return param;
    }

    public static void useFields() {
        // Use fields
        x = 5;          // field x
        y = 1.5;        // field y
        z = true;       // field z

        // Cross-type assignments (errors)
        x = y;          // double->int ERROR
        y = z;          // bool->double ERROR
        z = x;          // int->bool ERROR
    }

    public static void main(String[] args) {
        int local;

        // Use field x
        local = x;              // OK (field access)

        // Call method x
        local = x(local);       // OK (method call)

        // Method calls
        useFields();            // OK
        local = x(5);           // OK (method)

        // Shadowing: local var with same name as field
        int x;                  // ERROR: x already defined? or shadowing?
    }
}
