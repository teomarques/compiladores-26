// TEST 9: If/While with errors in BOTH condition AND body
// Tests order of error reporting

class TestIfWhileWithBodyErrors {
    public static void main(String[] args) {
        int a;
        boolean x;

        // Valid condition, body with error
        if (true) {
            a = undef_var;          // ERROR in body
        }

        // Invalid condition, valid body
        if (a) {                    // ERROR: int condition
            a = 1;                  // OK
        }

        // Both invalid: condition AND body
        if (1 + 2) {                // ERROR: int condition
            a = undef2;             // ERROR: undefined
        }

        // While with both errors
        while (a + 1) {             // ERROR: int condition
            x = a + b;              // ERROR: b undefined
        }

        // Else block with errors
        if (x) {                    // OK
            a = 1;
        } else {
            a = undef3;             // ERROR
        }

        // Nested if/while
        if (a) {                    // ERROR: int
            if (a + 1) {            // ERROR: int
                a = undef4;         // ERROR
            }
        }
    }
}
