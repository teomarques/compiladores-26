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

define i32 @_m4_methods_call_triple_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = load i32, i32* %1
  %3 = load i32, i32* %1
  %4 = add i32 %2, %3
  %5 = load i32, i32* %1
  %6 = add i32 %4, %5
  ret i32 %6
L1:
  ret i32 0
}

define void @_m4_methods_call_main_StringArray() {
  %1 = add i32 4, 0
  %2 = call i32 @_m4_methods_call_triple_int(i32 %1)
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i32 %2)
  %5 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %6 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* %6, i8* %5)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_methods_call_main_StringArray()
  ret i32 0
}
