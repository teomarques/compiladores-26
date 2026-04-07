# Relatorio de Falhas - Meta 2

Resumo da execucao mais recente via ./run_prof_tests.sh:
- PASS=25
- FAIL=11
- TOTAL=36

## Testes com FAIL

## all_errors

### Output obtido na execucao
```text
Line 6, col 23: syntax error: bacano
Line 7, col 22: syntax error: int
Line 8, col 16: syntax error: a
Line 10, col 21: syntax error: void
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 5, col 20: syntax error: =
  - Obtido: (nao identificado)
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/all_errors.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/all_errors.got	2026-04-07 17:52:27.764739657 +0100
@@ -1,4 +1,3 @@
-Line 5, col 20: syntax error: =
 Line 6, col 23: syntax error: bacano
 Line 7, col 22: syntax error: int
 Line 8, col 16: syntax error: a
```

### Programa Java testado
```java
// Compilar com a flag -e2

class assign_error {
  public static void main(String[] args) {
    f(i = j = a&&b = l);
    ola(skrttt, tiago bacano, to_incrivel);
    Integer.parseInt(int void());
    return (20 a comp);
  }
  public static int void mekie();
}

```

## assign_error

### Output obtido na execucao
```text
Program
..Identifier(assign_error)
..MethodDecl
....MethodHeader
......Void
......Identifier(main)
......MethodParams
........ParamDecl
..........StringArray
..........Identifier(args)
....MethodBody
......Call
........Identifier(f)
........Assign
..........Identifier(i)
..........Assign
............Identifier(j)
............And
..............Identifier(a)
..............Assign
................Identifier(b)
................Identifier(l)
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 3, col 20: syntax error: =
  - Obtido: Program
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/assign_error.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/assign_error.got	2026-04-07 17:52:27.789889969 +0100
@@ -1 +1,22 @@
-Line 3, col 20: syntax error: =
+Program
+..Identifier(assign_error)
+..MethodDecl
+....MethodHeader
+......Void
+......Identifier(main)
+......MethodParams
+........ParamDecl
+..........StringArray
+..........Identifier(args)
+....MethodBody
+......Call
+........Identifier(f)
+........Assign
+..........Identifier(i)
+..........Assign
+............Identifier(j)
+............And
+..............Identifier(a)
+..............Assign
+................Identifier(b)
+................Identifier(l)
```

### Programa Java testado
```java
class assign_error {
  public static void main(String[] args) {
    f(i = j = a&&b = l);
  }
}

```

## errorsAndAST

### Output obtido na execucao
```text
Line 8, col 5: unterminated string literal
Line 52, col 3: syntax error: }
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Program
  - Obtido: Line 52, col 3: syntax error: }
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/errorsAndAST.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/errorsAndAST.got	2026-04-07 17:52:27.875740470 +0100
@@ -1,75 +1,2 @@
 Line 8, col 5: unterminated string literal
-Program
-..Identifier(errorsAndAST)
-..MethodDecl
-....MethodHeader
-......Void
-......Identifier(main)
-......MethodParams
-........ParamDecl
-..........StringArray
-..........Identifier(args)
-....MethodBody
-......If
-........Identifier(A)
-........If
-..........Natural(0)
-..........Print
-............Natural(0)
-..........If
-............Identifier(x)
-............If
-..............Natural(1)
-..............Block
-..............Block
-............If
-..............Identifier(ABC)
-..............Block
-..............Assign
-................Identifier(x)
-................Natural(1)
-........Block
-......While
-........Eq
-..........Identifier(tiago_to)
-..........Identifier(manolos_simpaticos)
-........Print
-..........StrLit("Impossivel sair daqui eheh")
-......While
-........Identifier(a)
-........While
-..........Identifier(b)
-..........While
-............Identifier(espero)
-............While
-..............Identifier(isto)
-..............While
-................Identifier(funcione)
-................While
-..................Identifier(bem)
-..................Print
-....................StrLit("RUMO AOS 800!!")
-......If
-........Identifier(tiago_to)
-........Block
-........Block
-......If
-........BoolLit(true)
-........If
-..........BoolLit(true)
-..........If
-............BoolLit(false)
-............Block
-............Block
-..........If
-............Identifier(ABC)
-............Block
-............Block
-........Block
-......If
-........BoolLit(false)
-........Block
-........If
-..........BoolLit(false)
-..........Block
-..........Block
+Line 52, col 3: syntax error: }
```

### Programa Java testado
```java
// Se compilarem com a flag -e2, da erro do unterminated string literal
// Se compilarem com a flag -t, da o erro na mesma, mas a AST e impressa
// 

class errorsAndAST {
  public static void main(String[] args) {
    //a = "nao desistam amigos";
    "sdfsdfsdf
    if(A)
        if(0) System.out.print(0);
          else
            if(x) {
                if (1);
            } else
                if(ABC) {
                    {;}
                } else x=1;

    while (tiago_to == manolos_simpaticos){
        System.out.print("Impossivel sair daqui eheh");
    }
    
    while(a){
        while (b){
            while (espero){
                while (isto){
                    while (funcione){
                        while (bem){
                            System.out.print("RUMO AOS 800!!");
                        }
                    }
                }
            }
        }
    }


    if(tiago_to);     else ;

    if (true)
        if (true)
            if (false)
                {}
            else
                {}
        else
            if (ABC)
                {}
            else
                {}
    if (false) {} else if (false) {}
  }
}
```

## exemplo_erro

### Output obtido na execucao
```text
Line 1, col 1: unterminated comment
Line 1, col 3: syntax error: 
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 2, col 1: syntax error: 
  - Obtido: Line 1, col 3: syntax error: 
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/exemplo_erro.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/exemplo_erro.got	2026-04-07 17:52:27.944740975 +0100
@@ -1,2 +1,2 @@
 Line 1, col 1: unterminated comment
-Line 2, col 1: syntax error: 
+Line 1, col 3: syntax error: 
```

### Programa Java testado
```java
/*
```

## Factorial_corrumpido

### Output obtido na execucao
```text
Line 2, col 33: syntax error: in2t
Line 4, col 13: syntax error: return
Line 8, col 36: syntax error: 4
Line 9, col 17: syntax error: [
Line 10, col 20: illegal character (.)
Line 12, col 9: syntax error: XD
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: (nao identificado)
  - Obtido: Line 4, col 13: syntax error: return
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/Factorial_corrumpido.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/Factorial_corrumpido.got	2026-04-07 17:52:27.990086733 +0100
@@ -1 +1,6 @@
 Line 2, col 33: syntax error: in2t
+Line 4, col 13: syntax error: return
+Line 8, col 36: syntax error: 4
+Line 9, col 17: syntax error: [
+Line 10, col 20: illegal character (.)
+Line 12, col 9: syntax error: XD
```

### Programa Java testado
```java
class Factorial {
    public static int factorial(in2t n) {
        if (n == 500
            return 0;
        return n * factorial(n-1);
    }

    public static void main(String[4] args) {
        int args[1];
        argument = .parseInt(300);
        System.out.print(factorial(argument));CHUJ
        XD
        Integer = 3;
    }
}
```

## MethodFieldError2

### Output obtido na execucao
```text
Line 8, col 5: syntax error: }
Line 9, col 5: syntax error: public
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: (nao identificado)
  - Obtido: Line 9, col 5: syntax error: public
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/MethodFieldError2.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/MethodFieldError2.got	2026-04-07 17:52:28.175742666 +0100
@@ -1 +1,2 @@
 Line 8, col 5: syntax error: }
+Line 9, col 5: syntax error: public
```

### Programa Java testado
```java
//flag -e2

class Factorial {
    public static boolean b, a, c;
    public static boolean a(){
        int x, d;
        int a,
    };
    public static int factorial(int n) {
    }
}```

## MethodFieldError5

### Output obtido na execucao
```text
Line 6, col 9: unterminated comment
Line 10, col 1: syntax error: 
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 10, col 2: syntax error: 
  - Obtido: Line 10, col 1: syntax error: 
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/MethodFieldError5.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/MethodFieldError5.got	2026-04-07 17:52:28.226743039 +0100
@@ -1,2 +1,2 @@
 Line 6, col 9: unterminated comment
-Line 10, col 2: syntax error: 
+Line 10, col 1: syntax error: 
```

### Programa Java testado
```java
//flag -e2

class Factorial {
    public static boolean b, a, c;
    public static boolean a(){
        /*)
    }
    public static int factorial(int n) {
    }
}```

## MultipleErrors

### Output obtido na execucao
```text
Line 2, col 39: unterminated string literal
Line 4, col 4: syntax error: ++
Line 5, col 4: illegal character (~)
Line 7, col 8: syntax error: =
Line 7, col 19: illegal character (?)
Line 13, col 5: syntax error: else
Line 15, col 22: syntax error: =
Line 16, col 2: syntax error: String
Line 17, col 9: syntax error: System
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 17, col 15: illegal character (.)
  - Obtido: Line 17, col 9: syntax error: System
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/MultipleErrors.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/MultipleErrors.got	2026-04-07 17:52:28.289743500 +0100
@@ -6,6 +6,4 @@
 Line 13, col 5: syntax error: else
 Line 15, col 22: syntax error: =
 Line 16, col 2: syntax error: String
-Line 17, col 15: illegal character (.)
-Line 17, col 19: illegal character (.)
-Line 22, col 37: unterminated comment
+Line 17, col 9: syntax error: System
```

### Programa Java testado
```java
class AY {
    public static int ay(boolean b) { "comma missing
        if (n == 0){
		a++ //Make sure the columns are still correct
		b~;
	}
	a = 2 = b||7 = 1 ? = 2; 

	return c;
    }

    public static void main(String[] args) { 
	if else a && s;

        int argument = Integer.parseInt(args[0]);;;;
	String 2 = { () () if if if else else else a ]; }
        System.out.;
	int a = b;
	b = c;
	c*d;

                                    /*print(ay([v}));
    }


Don't
		Forget
				That
					Columns are still incrementing```

## string_error2

### Output obtido na execucao
```text
Line 3, col 19: invalid escape sequence (\|)
Program
..Identifier(Teste)
..MethodDecl
....MethodHeader
......Int
......Identifier(main)
......MethodParams
........ParamDecl
..........StringArray
..........Identifier(args)
....MethodBody
......Call
........Identifier(call)
......Print
........StrLit("String string")
......Call
........Identifier(call)
........StrLit("valid string")
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 5, col 10: syntax error: "valid string"
  - Obtido: Program
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/string_error2.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/string_error2.got	2026-04-07 17:52:28.355109343 +0100
@@ -1,2 +1,19 @@
 Line 3, col 19: invalid escape sequence (\|)
-Line 5, col 10: syntax error: "valid string"
+Program
+..Identifier(Teste)
+..MethodDecl
+....MethodHeader
+......Int
+......Identifier(main)
+......MethodParams
+........ParamDecl
+..........StringArray
+..........Identifier(args)
+....MethodBody
+......Call
+........Identifier(call)
+......Print
+........StrLit("String string")
+......Call
+........Identifier(call)
+........StrLit("valid string")
```

### Programa Java testado
```java
class Teste {
  public static int main(String[] args){
    call("invalid \| string");
    System.out.print("String string");
    call("valid string");
  }
}
```

## teste_ast_erros

### Output obtido na execucao
```text
Line 6, col 12: syntax error: boolean
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Line 13, col 21: syntax error: ;
  - Obtido: (nao identificado)
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/teste_ast_erros.out	2026-04-07 14:59:00.401453860 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/teste_ast_erros.got	2026-04-07 17:52:28.390744240 +0100
@@ -1,7 +1 @@
 Line 6, col 12: syntax error: boolean
-Line 13, col 21: syntax error: ;
-Line 22, col 13: syntax error: "we out here"
-Line 27, col 13: syntax error: "dat boi dead"
-Line 34, col 27: syntax error: {
-Line 36, col 34: syntax error: !
-Line 40, col 9: syntax error: passar
```

### Programa Java testado
```java
//compilar com flag -e2

class Teste_Erros {
    public static int a, b, c;
    public static double preco;
    public boolean vivo;

    ;;;;;;;;;;

    public static int getNumero() {
        while (preco < 100000) {
            preco = a + b + c;
            a = a + ;
            b = b + 1.5;
            c = c + 10;
        }
        return preco / 10;
    }

    public static boolean estaVivo() {
        if (vivo == true && preco != 0) {
            "we out here";
        }
        else {
            preco = getNumero();
            preco = preco >> 2;
            "dat boi dead";
        }
        return vivo;
    }

    public static void main(String[] args) {
        tamanho = a.length;
        if (a == (b == c) {
            Integer.parseInt(a[a^a]);   //ParseArgs com xor
            Integer.parseInt(b[-b!c]);    //ParseArgs com menos
            Integer.parseInt(c[!c]);    //ParseArgs com not
        }
        
        passar(cg,comp(meta2),iia,lpa,si,scc);


    }
}```

## varios_testes

### Output obtido na execucao
```text
Line 27, col 9: syntax error: a
Line 28, col 9: syntax error: b
Line 29, col 9: syntax error: c
Line 30, col 9: syntax error: c1
Line 31, col 9: syntax error: d
Line 32, col 9: syntax error: Integer.parseInt
Line 35, col 9: syntax error: passarAComp
Line 37, col 5: syntax error: }
```

### Analise breve da falha
- Primeira divergencia relevante no diff:
  - Esperado: Program
  - Obtido: Line 27, col 9: syntax error: a
- Impacto: a saida gerada diverge do esperado a partir desta primeira discrepancia.

### Diff vs esperado
```diff
--- /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/../repo-prof/java/meta2/varios_testes.out	2026-04-07 14:59:00.402237068 +0100
+++ /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/prof-results/meta2/varios_testes.got	2026-04-07 17:52:28.444744635 +0100
@@ -1,124 +1,8 @@
-Program
-..Identifier(Teste)
-..FieldDecl
-....Int
-....Identifier(a)
-..FieldDecl
-....Int
-....Identifier(b)
-..FieldDecl
-....Int
-....Identifier(c)
-..FieldDecl
-....Int
-....Identifier(d)
-..MethodDecl
-....MethodHeader
-......Void
-......Identifier(main)
-......MethodParams
-........ParamDecl
-..........Int
-..........Identifier(a)
-........ParamDecl
-..........Double
-..........Identifier(c)
-........ParamDecl
-..........Bool
-..........Identifier(d)
-........ParamDecl
-..........Int
-..........Identifier(h)
-....MethodBody
-......VarDecl
-........Int
-........Identifier(x)
-......VarDecl
-........Int
-........Identifier(y)
-......VarDecl
-........Int
-........Identifier(z)
-......VarDecl
-........Int
-........Identifier(k)
-......VarDecl
-........Int
-........Identifier(l)
-......VarDecl
-........Int
-........Identifier(m)
-......VarDecl
-........Int
-........Identifier(o)
-......VarDecl
-........Int
-........Identifier(p)
-......If
-........Identifier(x1)
-........Block
-........Block
-......If
-........Identifier(x2)
-........Assign
-..........Identifier(a)
-..........Natural(1)
-........Block
-......If
-........Identifier(x3)
-........Block
-..........Assign
-............Identifier(b)
-............Natural(2)
-..........Assign
-............Identifier(c)
-............Natural(3)
-........Assign
-..........Identifier(d)
-..........Natural(4)
-......If
-........Identifier(x4)
-........Block
-........Block
-......If
-........BoolLit(true)
-........Block
-........If
-..........BoolLit(false)
-..........Block
-..........Block
-......Assign
-........Identifier(a)
-........Length
-..........Identifier(b)
-......Assign
-........Identifier(b)
-........Natural(1)
-......Assign
-........Identifier(c)
-........BoolLit(false)
-......Assign
-........Identifier(c1)
-........BoolLit(true)
-......Assign
-........Identifier(d)
-........Decimal(1.0)
-......ParseArgs
-........Identifier(abc)
-........Assign
-..........Identifier(a)
-..........Natural(1)
-......Call
-........Identifier(passarAComp)
-........Identifier(Zbording)
-........Call
-..........Identifier(passarAComp)
-..........Identifier(Epossivel)
-........Call
-..........Identifier(compPassar)
-..........Identifier(ouChumbar)
-........Call
-..........Identifier(chumbar)
-..........Call
-............Identifier(tambemSefaz)
-............Identifier(ESoFazer)
+Line 27, col 9: syntax error: a
+Line 28, col 9: syntax error: b
+Line 29, col 9: syntax error: c
+Line 30, col 9: syntax error: c1
+Line 31, col 9: syntax error: d
+Line 32, col 9: syntax error: Integer.parseInt
+Line 35, col 9: syntax error: passarAComp
+Line 37, col 5: syntax error: }
```

### Programa Java testado
```java
class Teste{
    // varios fielddecl seguidos
    public static int a,b,c,d;
    // varios inputs na funcao
    public static void main(int a, double c, boolean d, int h){
        // varios var decl seguidos
        int x,y,z,k,l,m,o,p;
        // if sem nada la dentro, 2 blocks
        if (x1) {}
        // if com 1 statement, 1 block no final
        if (x2) {
            a = 1;
        }
        if (x3){ // if com 2 statements, block no dps da Expr do if
            b = 2;
            c = 3;
        }
        else{
            d=4;
        }
        if (x4); // mesma ideia que ter duas brackets, 2 blocks

        if (true) {} else if (false) {}
        // 1 block para o if(1 caso de cima), e depois 1 block também para o proximo if
        // NOTA: aqui o block do 1 if tem que vir antes do segundo if, senao vai dar erro, isto é testado!!

        a = b.length; // teste para dotlength
        b = 1; // declit
        c = false; // boollit
        c1 = true;
        d = 1.0; // reallit
        Integer.parseInt(abc[a=1]); // parseArgs

        // teste de calls sem nada, calls la dentro
        passarAComp(Zbording,passarAComp(Epossivel),compPassar(ouChumbar),chumbar(tambemSefaz(ESoFazer)));

    }
}```

