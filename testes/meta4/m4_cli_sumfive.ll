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

define void @_m4_cli_sumfive_main_StringArray() {
  %1 = add i32 0, 0
  %2 = add i32 %1, 1
  %3 = load i8**, i8*** @argv
  %4 = getelementptr i8*, i8** %3, i32 %2
  %5 = load i8*, i8** %4
  %6 = call i32 @atoi(i8* %5)
  %7 = add i32 1, 0
  %8 = add i32 %7, 1
  %9 = load i8**, i8*** @argv
  %10 = getelementptr i8*, i8** %9, i32 %8
  %11 = load i8*, i8** %10
  %12 = call i32 @atoi(i8* %11)
  %13 = add i32 %6, %12
  %14 = add i32 2, 0
  %15 = add i32 %14, 1
  %16 = load i8**, i8*** @argv
  %17 = getelementptr i8*, i8** %16, i32 %15
  %18 = load i8*, i8** %17
  %19 = call i32 @atoi(i8* %18)
  %20 = add i32 %13, %19
  %21 = add i32 3, 0
  %22 = add i32 %21, 1
  %23 = load i8**, i8*** @argv
  %24 = getelementptr i8*, i8** %23, i32 %22
  %25 = load i8*, i8** %24
  %26 = call i32 @atoi(i8* %25)
  %27 = add i32 %20, %26
  %28 = add i32 4, 0
  %29 = add i32 %28, 1
  %30 = load i8**, i8*** @argv
  %31 = getelementptr i8*, i8** %30, i32 %29
  %32 = load i8*, i8** %31
  %33 = call i32 @atoi(i8* %32)
  %34 = add i32 %27, %33
  %35 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %36 = call i32 (i8*, ...) @printf(i8* %35, i32 %34)
  %37 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %38 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %39 = call i32 (i8*, ...) @printf(i8* %38, i8* %37)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_cli_sumfive_main_StringArray()
  ret i32 0
}
