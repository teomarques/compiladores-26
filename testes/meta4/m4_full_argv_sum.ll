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

define void @_m4_full_argv_sum_main_StringArray() {
  %1 = alloca i32
  %2 = alloca i32
  %3 = add i32 0, 0
  store i32 %3, i32* %1
  %4 = add i32 0, 0
  store i32 %4, i32* %2
  br label %L1
L1:
  %5 = load i32, i32* %1
  %6 = load i32, i32* @argc
  %7 = sub i32 %6, 1
  %8 = icmp slt i32 %5, %7
  br i1 %8, label %L2, label %L3
L2:
  %9 = load i32, i32* %2
  %10 = load i32, i32* %1
  %11 = add i32 %10, 1
  %12 = load i8**, i8*** @argv
  %13 = getelementptr i8*, i8** %12, i32 %11
  %14 = load i8*, i8** %13
  %15 = call i32 @atoi(i8* %14)
  %16 = add i32 %9, %15
  store i32 %16, i32* %2
  %17 = load i32, i32* %1
  %18 = add i32 1, 0
  %19 = add i32 %17, %18
  store i32 %19, i32* %1
  br label %L1
L3:
  %20 = load i32, i32* %2
  %21 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %22 = call i32 (i8*, ...) @printf(i8* %21, i32 %20)
  %23 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %24 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %25 = call i32 (i8*, ...) @printf(i8* %24, i8* %23)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_full_argv_sum_main_StringArray()
  ret i32 0
}
