@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_a_global_var = global i32 0
@argc = global i32 0
@argv = global i8** null

define void @_a_main_StringArray() {
  %1 = alloca i32
  %2 = alloca i1
  %3 = alloca double
  %4 = add i32 1, 0
  store i32 %4, i32* %1
  %5 = load i32, i32* %1
  %6 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* %6, i32 %5)
  %8 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %9 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %10 = call i32 (i8*, ...) @printf(i8* %9, i8* %8)
  %11 = add i32 1, 0
  store i32 %11, i32* %1
  %12 = load i32, i32* %1
  %13 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %14 = call i32 (i8*, ...) @printf(i8* %13, i32 %12)
  %15 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %16 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %17 = call i32 (i8*, ...) @printf(i8* %16, i8* %15)
  %18 = add i32 1, 0
  %19 = sub i32 0, %18
  store i32 %19, i32* %1
  %20 = load i32, i32* %1
  %21 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %22 = call i32 (i8*, ...) @printf(i8* %21, i32 %20)
  %23 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %24 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %25 = call i32 (i8*, ...) @printf(i8* %24, i8* %23)
  %26 = add i1 1, 0
  store i1 %26, i1* %2
  %27 = load i1, i1* %2
  %28 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %29 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %30 = select i1 %27, i8* %28, i8* %29
  %31 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %32 = call i32 (i8*, ...) @printf(i8* %31, i8* %30)
  %33 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %34 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %35 = call i32 (i8*, ...) @printf(i8* %34, i8* %33)
  %36 = add i1 0, 0
  store i1 %36, i1* %2
  %37 = load i1, i1* %2
  %38 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %39 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %40 = select i1 %37, i8* %38, i8* %39
  %41 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %42 = call i32 (i8*, ...) @printf(i8* %41, i8* %40)
  %43 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %44 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %45 = call i32 (i8*, ...) @printf(i8* %44, i8* %43)
  %46 = fadd double 2.000000e+00, 0.0
  store double %46, double* %3
  %47 = load double, double* %3
  %48 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %49 = call i32 (i8*, ...) @printf(i8* %48, double %47)
  %50 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %51 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %52 = call i32 (i8*, ...) @printf(i8* %51, i8* %50)
  %53 = fadd double 2.200000e+00, 0.0
  store double %53, double* %3
  %54 = load double, double* %3
  %55 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %56 = call i32 (i8*, ...) @printf(i8* %55, double %54)
  %57 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %58 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %59 = call i32 (i8*, ...) @printf(i8* %58, i8* %57)
  %60 = add i32 2, 0
  %61 = sitofp i32 %60 to double
  store double %61, double* %3
  %62 = load double, double* %3
  %63 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %64 = call i32 (i8*, ...) @printf(i8* %63, double %62)
  %65 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %66 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %67 = call i32 (i8*, ...) @printf(i8* %66, i8* %65)
  %68 = add i32 1, 0
  store i32 %68, i32* %1
  %69 = load i32, i32* %1
  %70 = sitofp i32 %69 to double
  store double %70, double* %3
  %71 = load double, double* %3
  %72 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %73 = call i32 (i8*, ...) @printf(i8* %72, double %71)
  %74 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %75 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %76 = call i32 (i8*, ...) @printf(i8* %75, i8* %74)
  %77 = fadd double 2.200000e+00, 0.0
  store double %77, double* %3
  %78 = load double, double* %3
  %79 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %80 = call i32 (i8*, ...) @printf(i8* %79, double %78)
  %81 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %82 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %83 = call i32 (i8*, ...) @printf(i8* %82, i8* %81)
  %84 = fadd double 2.200000e+00, 0.0
  %85 = fsub double 0.0, %84
  store double %85, double* %3
  %86 = load double, double* %3
  %87 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %88 = call i32 (i8*, ...) @printf(i8* %87, double %86)
  %89 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %90 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %91 = call i32 (i8*, ...) @printf(i8* %90, i8* %89)
  %92 = load i32, i32* %1
  %93 = sitofp i32 %92 to double
  store double %93, double* %3
  %94 = load double, double* %3
  %95 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %96 = call i32 (i8*, ...) @printf(i8* %95, double %94)
  %97 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %98 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %99 = call i32 (i8*, ...) @printf(i8* %98, i8* %97)
  %100 = load i32, i32* %1
  %101 = sub i32 0, %100
  %102 = sitofp i32 %101 to double
  store double %102, double* %3
  %103 = load double, double* %3
  %104 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %105 = call i32 (i8*, ...) @printf(i8* %104, double %103)
  %106 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %107 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %108 = call i32 (i8*, ...) @printf(i8* %107, i8* %106)
  %109 = add i32 10, 0
  store i32 %109, i32* @_a_global_var
  %110 = load i32, i32* @_a_global_var
  %111 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %112 = call i32 (i8*, ...) @printf(i8* %111, i32 %110)
  %113 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %114 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %115 = call i32 (i8*, ...) @printf(i8* %114, i8* %113)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_a_main_StringArray()
  ret i32 0
}
