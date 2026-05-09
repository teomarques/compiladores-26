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

define void @_m4_ctrl_while_main_StringArray() {
  %1 = alloca i32
  %2 = alloca i32
  %3 = add i32 0, 0
  store i32 %3, i32* %1
  %4 = add i32 0, 0
  store i32 %4, i32* %2
  br label %L1
L1:
  %5 = load i32, i32* %1
  %6 = add i32 5, 0
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %L2, label %L3
L2:
  %8 = load i32, i32* %2
  %9 = load i32, i32* %1
  %10 = add i32 %8, %9
  store i32 %10, i32* %2
  %11 = load i32, i32* %1
  %12 = add i32 1, 0
  %13 = add i32 %11, %12
  store i32 %13, i32* %1
  br label %L1
L3:
  %14 = load i32, i32* %2
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i32 %14)
  %17 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %18 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %19 = call i32 (i8*, ...) @printf(i8* %18, i8* %17)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_ctrl_while_main_StringArray()
  ret i32 0
}
