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

define void @_m4_cli_parse_triple_main_StringArray() {
  %1 = add i32 0, 0
  %2 = add i32 %1, 1
  %3 = load i8**, i8*** @argv
  %4 = getelementptr i8*, i8** %3, i32 %2
  %5 = load i8*, i8** %4
  %6 = call i32 @atoi(i8* %5)
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i32 %6)
  %9 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %10 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* %10, i8* %9)
  %12 = add i32 1, 0
  %13 = add i32 %12, 1
  %14 = load i8**, i8*** @argv
  %15 = getelementptr i8*, i8** %14, i32 %13
  %16 = load i8*, i8** %15
  %17 = call i32 @atoi(i8* %16)
  %18 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %19 = call i32 (i8*, ...) @printf(i8* %18, i32 %17)
  %20 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %21 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %22 = call i32 (i8*, ...) @printf(i8* %21, i8* %20)
  %23 = add i32 2, 0
  %24 = add i32 %23, 1
  %25 = load i8**, i8*** @argv
  %26 = getelementptr i8*, i8** %25, i32 %24
  %27 = load i8*, i8** %26
  %28 = call i32 @atoi(i8* %27)
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i32 %28)
  %31 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %32 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %33 = call i32 (i8*, ...) @printf(i8* %32, i8* %31)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_cli_parse_triple_main_StringArray()
  ret i32 0
}
