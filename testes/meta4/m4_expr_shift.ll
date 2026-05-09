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

define void @_m4_expr_shift_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 48, 0
  store i32 %2, i32* %1
  %3 = load i32, i32* %1
  %4 = add i32 2, 0
  %5 = ashr i32 %3, %4
  %6 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* %6, i32 %5)
  %8 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %9 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %10 = call i32 (i8*, ...) @printf(i8* %9, i8* %8)
  %11 = load i32, i32* %1
  %12 = add i32 1, 0
  %13 = shl i32 %11, %12
  %14 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %15 = call i32 (i8*, ...) @printf(i8* %14, i32 %13)
  %16 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %17 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %18 = call i32 (i8*, ...) @printf(i8* %17, i8* %16)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_expr_shift_main_StringArray()
  ret i32 0
}
