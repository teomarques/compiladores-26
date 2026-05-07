@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@argc = global i32 0
@argv = global i8** null

define void @_return_types_main_StringArray() {
  %1 = alloca double
  %2 = call double @_return_types_fun_double()
  store double %2, double* %1
  %3 = load double, double* %1
  %4 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %5 = call i32 (i8*, ...) @printf(i8* %4, double %3)
  %6 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i8* %6)
  %9 = call i32 @_return_types_fun_int()
  %10 = sitofp i32 %9 to double
  store double %10, double* %1
  %11 = load double, double* %1
  %12 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, double %11)
  %14 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i8* %14)
  %17 = call double @_return_types_fun_double_from_int()
  store double %17, double* %1
  %18 = load double, double* %1
  %19 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %20 = call i32 (i8*, ...) @printf(i8* %19, double %18)
  %21 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i8* %21)
  %24 = call double @_return_types_fun_double_from_int_fun()
  store double %24, double* %1
  %25 = load double, double* %1
  %26 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %27 = call i32 (i8*, ...) @printf(i8* %26, double %25)
  %28 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i8* %28)
  %31 = call double @_return_types_fun_double_from_int_var()
  store double %31, double* %1
  %32 = load double, double* %1
  %33 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %34 = call i32 (i8*, ...) @printf(i8* %33, double %32)
  %35 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %36 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %37 = call i32 (i8*, ...) @printf(i8* %36, i8* %35)
  %38 = call double @_return_types_fun_double_from_args_StringArray(i8** %-1)
  store double %38, double* %1
  %39 = load double, double* %1
  %40 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %41 = call i32 (i8*, ...) @printf(i8* %40, double %39)
  %42 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %43 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %44 = call i32 (i8*, ...) @printf(i8* %43, i8* %42)
  %45 = call double @_return_types_fun_double_from_unary_plus_StringArray(i8** %-1)
  store double %45, double* %1
  %46 = load double, double* %1
  %47 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %48 = call i32 (i8*, ...) @printf(i8* %47, double %46)
  %49 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %50 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %51 = call i32 (i8*, ...) @printf(i8* %50, i8* %49)
  %52 = fadd double 1.300000e+00, 0.0
  %53 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %54 = call i32 (i8*, ...) @printf(i8* %53, double %52)
  %55 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %56 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %57 = call i32 (i8*, ...) @printf(i8* %56, i8* %55)
  ret void
}

define double @_return_types_fun_double() {
  %1 = fadd double 1.100000e+00, 0.0
  ret double %1
L1:
  ret double 0.0
}

define i32 @_return_types_fun_int() {
  %1 = add i32 1, 0
  ret i32 %1
L1:
  ret i32 0
}

define double @_return_types_fun_double_from_int() {
  %1 = add i32 2, 0
  %2 = sitofp i32 %1 to double
  ret double %2
L1:
  ret double 0.0
}

define double @_return_types_fun_double_from_int_fun() {
  %1 = call i32 @_return_types_fun_int()
  %2 = sitofp i32 %1 to double
  ret double %2
L1:
  ret double 0.0
}

define double @_return_types_fun_double_from_int_var() {
  %1 = alloca double
  %2 = add i32 0, 0
  %3 = sitofp i32 %2 to double
  store double %3, double* %1
  %4 = load double, double* %1
  %5 = call i32 @_return_types_fun_int()
  %6 = sitofp i32 %5 to double
  %7 = fadd double %4, %6
  %8 = call double @_return_types_fun_double()
  %9 = fadd double %7, %8
  ret double %9
L1:
  ret double 0.0
}

define double @_return_types_fun_double_from_args_StringArray(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = load i32, i32* @argc
  %3 = sub i32 %2, 1
  %4 = sitofp i32 %3 to double
  ret double %4
L1:
  ret double 0.0
}

define double @_return_types_fun_double_from_unary_plus_StringArray(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = add i32 1, 0
  %3 = sitofp i32 %2 to double
  ret double %3
L1:
  ret double 0.0
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_return_types_main_StringArray()
  ret i32 0
}
