@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_Main_d = global double 0.0
@argc = global i32 0
@argv = global i8** null

define double @_Main_test_double(double %param_0) {
  %1 = alloca double
  store double %param_0, double* %1
  %2 = load double, double* %1
  ret double %2
L1:
  ret double 0.0
}

define void @_Main_main_StringArray() {
  %1 = add i32 4, 0
  %2 = sitofp i32 %1 to double
  %3 = call double @_Main_test_double(double %2)
  %4 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %5 = call i32 (i8*, ...) @printf(i8* %4, double %3)
  %6 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i8* %6)
  %9 = add i32 2, 0
  %10 = sitofp i32 %9 to double
  store double %10, double* @_Main_d
  %11 = load double, double* @_Main_d
  %12 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, double %11)
  %14 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i8* %14)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Main_main_StringArray()
  ret i32 0
}
