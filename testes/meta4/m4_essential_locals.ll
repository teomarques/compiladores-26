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

define void @_m4_essential_locals_main_StringArray() {
  %1 = alloca i32
  %2 = alloca i32
  %3 = alloca double
  %4 = add i32 5, 0
  store i32 %4, i32* %1
  %5 = load i32, i32* %1
  %6 = add i32 2, 0
  %7 = add i32 %5, %6
  store i32 %7, i32* %2
  %8 = load i32, i32* %2
  %9 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %10 = call i32 (i8*, ...) @printf(i8* %9, i32 %8)
  %11 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %12 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, i8* %11)
  %14 = load i32, i32* %2
  %15 = sitofp i32 %14 to double
  store double %15, double* %3
  %16 = load double, double* %3
  %17 = fadd double 2.000000e+00, 0.0
  %18 = fdiv double %16, %17
  %19 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %20 = call i32 (i8*, ...) @printf(i8* %19, double %18)
  %21 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i8* %21)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_essential_locals_main_StringArray()
  ret i32 0
}
