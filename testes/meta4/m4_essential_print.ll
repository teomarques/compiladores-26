@.str.1 = private unnamed_addr constant [3 x i8] c"ok\00"
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

define void @_m4_essential_print_main_StringArray() {
  %1 = add i32 42, 0
  %2 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %3 = call i32 (i8*, ...) @printf(i8* %2, i32 %1)
  %4 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %5 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %6 = call i32 (i8*, ...) @printf(i8* %5, i8* %4)
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.1, i32 0, i32 0
  %8 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %9 = call i32 (i8*, ...) @printf(i8* %8, i8* %7)
  %10 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %11 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %12 = call i32 (i8*, ...) @printf(i8* %11, i8* %10)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_essential_print_main_StringArray()
  ret i32 0
}
