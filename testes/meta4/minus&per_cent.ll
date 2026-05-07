@.str.1 = private unnamed_addr constant [6 x i8] c"%%%%\0A\00"
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

define void @_test_main_StringArray() {
  %1 = fadd double 0.000000e+00, 0.0
  %2 = fsub double 0.0, %1
  %3 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, double %2)
  %5 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %6 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* %6, i8* %5)
  %8 = add i32 0, 0
  %9 = sub i32 0, %8
  %10 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* %10, i32 %9)
  %12 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %13 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %14 = call i32 (i8*, ...) @printf(i8* %13, i8* %12)
  %15 = fadd double 3.000000e-01, 0.0
  %16 = fsub double 0.0, %15
  %17 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %18 = call i32 (i8*, ...) @printf(i8* %17, double %16)
  %19 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %20 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %21 = call i32 (i8*, ...) @printf(i8* %20, i8* %19)
  %22 = getelementptr [6 x i8], [6 x i8]* @.str.1, i32 0, i32 0
  %23 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %24 = call i32 (i8*, ...) @printf(i8* %23, i8* %22)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_test_main_StringArray()
  ret i32 0
}
