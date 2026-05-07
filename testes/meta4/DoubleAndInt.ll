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

define void @_Main_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 3, 0
  store i32 %2, i32* %1
  %3 = alloca double
  %4 = add i32 4, 0
  %5 = sitofp i32 %4 to double
  store double %5, double* %3
  %6 = load i32, i32* %1
  %7 = load double, double* %3
  %8 = sitofp i32 %6 to double
  %9 = fadd double %8, %7
  store double %9, double* %3
  %10 = load i32, i32* %1
  %11 = load double, double* %3
  %12 = sitofp i32 %10 to double
  %13 = fdiv double %12, %11
  store double %13, double* %3
  %14 = load double, double* %3
  %15 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, double %14)
  %17 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %18 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %19 = call i32 (i8*, ...) @printf(i8* %18, i8* %17)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Main_main_StringArray()
  ret i32 0
}
