@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_test_b = global i1 0
@argc = global i32 0
@argv = global i8** null

define void @_test_main_StringArray() {
  %1 = add i1 1, 0
  %2 = xor i1 %1, 1
  store i1 %2, i1* @_test_b
  %3 = load i1, i1* @_test_b
  %4 = xor i1 %3, 1
  %5 = xor i1 %4, 1
  %6 = xor i1 %5, 1
  %7 = xor i1 %6, 1
  %8 = xor i1 %7, 1
  %9 = xor i1 %8, 1
  %10 = xor i1 %9, 1
  %11 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %12 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %13 = select i1 %10, i8* %11, i8* %12
  %14 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %15 = call i32 (i8*, ...) @printf(i8* %14, i8* %13)
  %16 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %17 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %18 = call i32 (i8*, ...) @printf(i8* %17, i8* %16)
  %19 = fadd double 4.200000e+02, 0.0
  %20 = fsub double 0.0, %19
  %21 = fsub double 0.0, %20
  %22 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, double %21)
  %24 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %25 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %26 = call i32 (i8*, ...) @printf(i8* %25, i8* %24)
  %27 = add i32 420, 0
  %28 = sub i32 0, %27
  %29 = sub i32 0, %28
  %30 = sub i32 0, %29
  %31 = sub i32 0, %30
  %32 = sub i32 0, %31
  %33 = add i32 421, 0
  %34 = sub i32 0, %33
  %35 = icmp sgt i32 %32, %34
  %36 = xor i1 %35, 1
  %37 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %38 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %39 = select i1 %36, i8* %37, i8* %38
  %40 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %41 = call i32 (i8*, ...) @printf(i8* %40, i8* %39)
  %42 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %43 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %44 = call i32 (i8*, ...) @printf(i8* %43, i8* %42)
  %45 = add i1 0, 0
  %46 = call i1 @_test_falseB()
  %47 = icmp eq i1 %45, %46
  %48 = xor i1 %47, 1
  %49 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %50 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %51 = select i1 %48, i8* %49, i8* %50
  %52 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %53 = call i32 (i8*, ...) @printf(i8* %52, i8* %51)
  %54 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %55 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %56 = call i32 (i8*, ...) @printf(i8* %55, i8* %54)
  %57 = add i32 1, 0
  %58 = add i32 1, 0
  %59 = sub i32 %57, %58
  %60 = add i32 0, 0
  %61 = add i32 %59, %60
  %62 = sub i32 0, %61
  %63 = sub i32 0, %62
  %64 = add i32 %63, 1
  %65 = load i8**, i8*** @argv
  %66 = getelementptr i8*, i8** %65, i32 %64
  %67 = load i8*, i8** %66
  %68 = call i32 @atoi(i8* %67)
  %69 = sub i32 0, %68
  %70 = sub i32 0, %69
  %71 = sub i32 0, %70
  %72 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %73 = call i32 (i8*, ...) @printf(i8* %72, i32 %71)
  %74 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %75 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %76 = call i32 (i8*, ...) @printf(i8* %75, i8* %74)
  ret void
}

define i1 @_test_falseB() {
  %1 = add i1 1, 0
  %2 = xor i1 %1, 1
  ret i1 %2
L1:
  ret i1 0
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_test_main_StringArray()
  ret i32 0
}
