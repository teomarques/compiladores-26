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

define void @_m4_essential_unary_main_StringArray() {
  %1 = alloca i32
  %2 = alloca double
  %3 = add i32 5, 0
  store i32 %3, i32* %1
  %4 = load i32, i32* %1
  %5 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %6 = call i32 (i8*, ...) @printf(i8* %5, i32 %4)
  %7 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %8 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %9 = call i32 (i8*, ...) @printf(i8* %8, i8* %7)
  %10 = load i32, i32* %1
  %11 = sub i32 0, %10
  %12 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, i32 %11)
  %14 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i8* %14)
  %17 = fadd double 3.500000e+00, 0.0
  store double %17, double* %2
  %18 = load double, double* %2
  %19 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %20 = call i32 (i8*, ...) @printf(i8* %19, double %18)
  %21 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i8* %21)
  %24 = load double, double* %2
  %25 = fsub double 0.0, %24
  %26 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %27 = call i32 (i8*, ...) @printf(i8* %26, double %25)
  %28 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i8* %28)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_essential_unary_main_StringArray()
  ret i32 0
}
