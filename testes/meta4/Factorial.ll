@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@argc = global i32 0
@argv = global i8** null

define i32 @_Factorial_factorial_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = load i32, i32* %1
  %3 = add i32 0, 0
  %4 = icmp eq i32 %2, %3
  br i1 %4, label %L1, label %L2
L1:
  %5 = add i32 1, 0
  ret i32 %5
L4:
  br label %L3
L2:
  br label %L3
L3:
  %6 = load i32, i32* %1
  %7 = load i32, i32* %1
  %8 = add i32 1, 0
  %9 = sub i32 %7, %8
  %10 = call i32 @_Factorial_factorial_int(i32 %9)
  %11 = mul i32 %6, %10
  ret i32 %11
L5:
  ret i32 0
}

define void @_Factorial_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 7, 0
  store i32 %2, i32* %1
  %3 = load i32, i32* %1
  %4 = call i32 @_Factorial_factorial_int(i32 %3)
  %5 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %6 = call i32 (i8*, ...) @printf(i8* %5, i32 %4)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Factorial_main_StringArray()
  ret i32 0
}
