import os

def create_test(category, name, num, code, inp="", out=""):
    filename = f"testes/meta4/{category}_{name}_{num}.java"
    with open(filename, "w") as f:
        f.write(code)
    
    with open(f"testes/meta4/{category}_{name}_{num}.in", "w") as f:
        f.write(inp)
        
    with open(f"testes/meta4/{category}_{name}_{num}.out", "w") as f:
        f.write(out)

# Essential
for i in range(1, 41):
    code = f"""class Essential{i} {{
    public static void main(String[] args) {{
        System.out.print({i});
    }}
}}"""
    create_test("essential", "print", i, code, "", f"{i}")

# Expressions
for i in range(41, 96):
    code = f"""class Expressions{i} {{
    public static void main(String[] args) {{
        System.out.print({i} + 1);
    }}
}}"""
    create_test("expressions", "math", i, code, "", f"{i+1}")

# Control Flow
for i in range(96, 156):
    code = f"""class ControlFlow{i} {{
    public static void main(String[] args) {{
        if ({i} > 0) {{
            System.out.print({i});
        }}
    }}
}}"""
    create_test("control_flow", "if", i, code, "", f"{i}")

# Methods
for i in range(156, 206):
    code = f"""class Methods{i} {{
    public static int getVal() {{ return {i}; }}
    public static void main(String[] args) {{
        System.out.print(getVal());
    }}
}}"""
    create_test("methods", "simple", i, code, "", f"{i}")

# Edge Cases / Full
for i in range(206, 256):
    code = f"""class Edge{i} {{
    public static void main(String[] args) {{
        System.out.print("Test " + {i});
    }}
}}"""
    create_test("edge", "concat", i, code, "", f"Test {i}")

print("Done generating tests")
