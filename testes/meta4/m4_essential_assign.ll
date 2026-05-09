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

define void @_m4_essential_assign_main_StringArray() {
  %1 = alloca i32
  %2 = alloca i32
  %3 = alloca i32
  %4 = add i32 2, 0
  store i32 %4, i32* %1
  %5 = load i32, i32* %1
  store i32 %5, i32* %2
  %6 = load i32, i32* %2
  %7 = add i32 1, 0
  %8 = add i32 %6, %7
  store i32 %8, i32* %3
  %9 = load i32, i32* %1
  %10 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* %10, i32 %9)
  %12 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %13 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %14 = call i32 (i8*, ...) @printf(i8* %13, i8* %12)
  %15 = load i32, i32* %2
  %16 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %17 = call i32 (i8*, ...) @printf(i8* %16, i32 %15)
  %18 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %19 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %20 = call i32 (i8*, ...) @printf(i8* %19, i8* %18)
  %21 = load i32, i32* %3
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i32 %21)
  %24 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %25 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %26 = call i32 (i8*, ...) @printf(i8* %25, i8* %24)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_essential_assign_main_StringArray()
  ret i32 0
}
