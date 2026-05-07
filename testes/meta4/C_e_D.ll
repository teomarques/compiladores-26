@.str.1 = private unnamed_addr constant [22 x i8] c"Teste\09de \0Cstring\5C\22\0D%%\00"
@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_CeD_a = global i32 0
@_CeD_b = global i32 0
@_CeD_c = global i32 0
@_CeD_d = global i32 0
@argc = global i32 0
@argv = global i8** null

define void @_CeD_main_StringArray() {
  %1 = add i32 2, 0
  store i32 %1, i32* @_CeD_a
  %2 = load i32, i32* @_CeD_a
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i32 %2)
  %5 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %6 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* %6, i8* %5)
  %8 = alloca i32
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  %13 = alloca double
  %14 = add i32 0, 0
  %15 = add i32 3, 0
  %16 = add i32 %14, %15
  store i32 %16, i32* %9
  %17 = sitofp i32 %16 to double
  store double %17, double* %13
  %18 = load i32, i32* %9
  %19 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %20 = call i32 (i8*, ...) @printf(i8* %19, i32 %18)
  %21 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i8* %21)
  %24 = load double, double* %13
  %25 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %26 = call i32 (i8*, ...) @printf(i8* %25, double %24)
  %27 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %28 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %29 = call i32 (i8*, ...) @printf(i8* %28, i8* %27)
  %30 = getelementptr [22 x i8], [22 x i8]* @.str.1, i32 0, i32 0
  %31 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %32 = call i32 (i8*, ...) @printf(i8* %31, i8* %30)
  %33 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %34 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %35 = call i32 (i8*, ...) @printf(i8* %34, i8* %33)
  %36 = load i32, i32* %9
  %37 = add i32 1, 0
  %38 = add i32 %36, %37
  store i32 %38, i32* %9
  %39 = add i32 0, 0
  %40 = icmp sgt i32 %38, %39
  br label %L3
L3:
  br i1 %40, label %L2, label %L1
L1:
  %41 = load i32, i32* %9
  %42 = add i32 3, 0
  %43 = add i32 %41, %42
  store i32 %43, i32* %9
  %44 = add i32 0, 0
  %45 = icmp sgt i32 %43, %44
  br label %L4
L4:
  br label %L2
L2:
  %46 = phi i1 [ %40, %L3 ], [ %45, %L4 ]
  %47 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %48 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %49 = select i1 %46, i8* %47, i8* %48
  %50 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %51 = call i32 (i8*, ...) @printf(i8* %50, i8* %49)
  %52 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %53 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %54 = call i32 (i8*, ...) @printf(i8* %53, i8* %52)
  %55 = load i32, i32* %9
  %56 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %57 = call i32 (i8*, ...) @printf(i8* %56, i32 %55)
  %58 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %59 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %60 = call i32 (i8*, ...) @printf(i8* %59, i8* %58)
  %61 = load i32, i32* %9
  %62 = add i32 1, 0
  %63 = add i32 %61, %62
  store i32 %63, i32* %9
  %64 = add i32 0, 0
  %65 = icmp slt i32 %63, %64
  br label %L7
L7:
  br i1 %65, label %L5, label %L6
L5:
  %66 = load i32, i32* %9
  %67 = add i32 3, 0
  %68 = add i32 %66, %67
  store i32 %68, i32* %9
  %69 = add i32 0, 0
  %70 = icmp slt i32 %68, %69
  br label %L8
L8:
  br label %L6
L6:
  %71 = phi i1 [ %65, %L7 ], [ %70, %L8 ]
  %72 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %73 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %74 = select i1 %71, i8* %72, i8* %73
  %75 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %76 = call i32 (i8*, ...) @printf(i8* %75, i8* %74)
  %77 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %78 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %79 = call i32 (i8*, ...) @printf(i8* %78, i8* %77)
  %80 = load i32, i32* %9
  %81 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %82 = call i32 (i8*, ...) @printf(i8* %81, i32 %80)
  %83 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %84 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %85 = call i32 (i8*, ...) @printf(i8* %84, i8* %83)
  %86 = fadd double 1.230000e-08, 0.0
  store double %86, double* %13
  %87 = load double, double* %13
  %88 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %89 = call i32 (i8*, ...) @printf(i8* %88, double %87)
  %90 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %91 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %92 = call i32 (i8*, ...) @printf(i8* %91, i8* %90)
  %93 = fadd double 0.000000e+00, 0.0
  store double %93, double* %13
  %94 = load double, double* %13
  %95 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %96 = call i32 (i8*, ...) @printf(i8* %95, double %94)
  %97 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %98 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %99 = call i32 (i8*, ...) @printf(i8* %98, i8* %97)
  %100 = fadd double 1.000000e+01, 0.0
  store double %100, double* %13
  %101 = load double, double* %13
  %102 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %103 = call i32 (i8*, ...) @printf(i8* %102, double %101)
  %104 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %105 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %106 = call i32 (i8*, ...) @printf(i8* %105, i8* %104)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_CeD_main_StringArray()
  ret i32 0
}
