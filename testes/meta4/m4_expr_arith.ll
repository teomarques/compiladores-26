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

define void @_m4_expr_arith_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 17, 0
  store i32 %2, i32* %1
  %3 = load i32, i32* %1
  %4 = add i32 3, 0
  %5 = add i32 2, 0
  %6 = mul i32 %4, %5
  %7 = add i32 %3, %6
  %8 = add i32 8, 0
  %9 = add i32 5, 0
  %10 = srem i32 %8, %9
  %11 = sub i32 %7, %10
  %12 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, i32 %11)
  %14 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i8* %14)
  %17 = load i32, i32* %1
  %18 = add i32 4, 0
  %19 = sdiv i32 %17, %18
  %20 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %21 = call i32 (i8*, ...) @printf(i8* %20, i32 %19)
  %22 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %23 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %24 = call i32 (i8*, ...) @printf(i8* %23, i8* %22)
  %25 = load i32, i32* %1
  %26 = add i32 1, 0
  %27 = add i32 %25, %26
  %28 = add i32 5, 0
  %29 = srem i32 %27, %28
  %30 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %31 = call i32 (i8*, ...) @printf(i8* %30, i32 %29)
  %32 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %33 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %34 = call i32 (i8*, ...) @printf(i8* %33, i8* %32)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_expr_arith_main_StringArray()
  ret i32 0
}
