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

define void @_m4_ctrl_if_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 7, 0
  store i32 %2, i32* %1
  %3 = load i32, i32* %1
  %4 = add i32 10, 0
  %5 = icmp slt i32 %3, %4
  br i1 %5, label %L1, label %L2
L1:
  %6 = add i32 1, 0
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i32 %6)
  %9 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %10 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* %10, i8* %9)
  br label %L3
L2:
  %12 = add i32 2, 0
  %13 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %14 = call i32 (i8*, ...) @printf(i8* %13, i32 %12)
  %15 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %16 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %17 = call i32 (i8*, ...) @printf(i8* %16, i8* %15)
  br label %L3
L3:
  %18 = add i32 20, 0
  store i32 %18, i32* %1
  %19 = load i32, i32* %1
  %20 = add i32 10, 0
  %21 = icmp slt i32 %19, %20
  br i1 %21, label %L4, label %L5
L4:
  %22 = add i32 3, 0
  %23 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %24 = call i32 (i8*, ...) @printf(i8* %23, i32 %22)
  %25 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %26 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %27 = call i32 (i8*, ...) @printf(i8* %26, i8* %25)
  br label %L6
L5:
  %28 = add i32 4, 0
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i32 %28)
  %31 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %32 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %33 = call i32 (i8*, ...) @printf(i8* %32, i8* %31)
  br label %L6
L6:
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_ctrl_if_main_StringArray()
  ret i32 0
}
