@.str.2 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.1 = private unnamed_addr constant [25 x i8] c"I don't have a parameter\00"
@.str.0 = private unnamed_addr constant [19 x i8] c"I have a parameter\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_Main_overload = global i32 0
@argc = global i32 0
@argv = global i8** null

define void @_Main_overload_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = getelementptr [19 x i8], [19 x i8]* @.str.0, i32 0, i32 0
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i8* %2)
  ret void
}

define void @_Main_overload() {
  %1 = getelementptr [25 x i8], [25 x i8]* @.str.1, i32 0, i32 0
  %2 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %3 = call i32 (i8*, ...) @printf(i8* %2, i8* %1)
  ret void
}

define void @_Main_main_StringArray() {
  %1 = load i32, i32* @argc
  %2 = sub i32 %1, 1
  %3 = add i32 0, 0
  %4 = icmp sgt i32 %2, %3
  br i1 %4, label %L1, label %L2
L1:
  %5 = add i32 0, 0
  %6 = add i32 %5, 1
  %7 = load i8**, i8*** @argv
  %8 = getelementptr i8*, i8** %7, i32 %6
  %9 = load i8*, i8** %8
  %10 = call i32 @atoi(i8* %9)
  call void @_Main_overload_int(i32 %10)
  br label %L3
L2:
  call void @_Main_overload()
  br label %L3
L3:
  %11 = getelementptr [2 x i8], [2 x i8]* @.str.2, i32 0, i32 0
  %12 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, i8* %11)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Main_main_StringArray()
  ret i32 0
}
