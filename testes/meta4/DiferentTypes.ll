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

define i1 @_Main_bool() {
  %1 = alloca i1
  %2 = add i1 1, 0
  store i1 %2, i1* %1
  %3 = load i1, i1* %1
  %4 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %5 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %6 = select i1 %3, i8* %4, i8* %5
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i8* %6)
  %9 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %10 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* %10, i8* %9)
  %12 = load i1, i1* %1
  ret i1 %12
L1:
  ret i1 0
}

define i32 @_Main_integer() {
  %1 = alloca i32
  %2 = add i32 1, 0
  store i32 %2, i32* %1
  %3 = load i32, i32* %1
  %4 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %5 = call i32 (i8*, ...) @printf(i8* %4, i32 %3)
  %6 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i8* %6)
  %9 = load i32, i32* %1
  ret i32 %9
L1:
  ret i32 0
}

define double @_Main_real() {
  %1 = alloca double
  %2 = fadd double 1.000000e+00, 0.0
  store double %2, double* %1
  %3 = load double, double* %1
  %4 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %5 = call i32 (i8*, ...) @printf(i8* %4, double %3)
  %6 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i8* %6)
  %9 = load double, double* %1
  ret double %9
L1:
  ret double 0.0
}

define void @_Main_main_StringArray() {
  %1 = call i1 @_Main_bool()
  %2 = call i32 @_Main_integer()
  %3 = call double @_Main_real()
  %4 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %5 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %6 = call i32 (i8*, ...) @printf(i8* %5, i8* %4)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Main_main_StringArray()
  ret i32 0
}
