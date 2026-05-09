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

define i32 @_m4_methods_return_pick_boolean_int_int(i1 %param_0, i32 %param_1, i32 %param_2) {
  %1 = alloca i1
  store i1 %param_0, i1* %1
  %2 = alloca i32
  store i32 %param_1, i32* %2
  %3 = alloca i32
  store i32 %param_2, i32* %3
  %4 = load i1, i1* %1
  br i1 %4, label %L1, label %L2
L1:
  %5 = load i32, i32* %2
  ret i32 %5
L4:
  br label %L3
L2:
  %6 = load i32, i32* %3
  ret i32 %6
L5:
  br label %L3
L3:
  ret i32 0
}

define void @_m4_methods_return_main_StringArray() {
  %1 = add i1 1, 0
  %2 = add i32 11, 0
  %3 = add i32 22, 0
  %4 = call i32 @_m4_methods_return_pick_boolean_int_int(i1 %1, i32 %2, i32 %3)
  %5 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %6 = call i32 (i8*, ...) @printf(i8* %5, i32 %4)
  %7 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %8 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %9 = call i32 (i8*, ...) @printf(i8* %8, i8* %7)
  %10 = add i1 0, 0
  %11 = add i32 11, 0
  %12 = add i32 22, 0
  %13 = call i32 @_m4_methods_return_pick_boolean_int_int(i1 %10, i32 %11, i32 %12)
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
  call void @_m4_methods_return_main_StringArray()
  ret i32 0
}
