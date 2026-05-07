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
  %7 = add i32 1, 0
  %8 = add i32 %6, %7
  %9 = load double, double* %3
  %10 = sitofp i32 %8 to double
  %11 = fcmp oeq double %10, %9
  br i1 %11, label %L1, label %L2
L1:
  %12 = load double, double* %3
  %13 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %14 = call i32 (i8*, ...) @printf(i8* %13, double %12)
  br label %L3
L2:
  br label %L3
L3:
  %15 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %16 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %17 = call i32 (i8*, ...) @printf(i8* %16, i8* %15)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Main_main_StringArray()
  ret i32 0
}
