@.str.5 = private unnamed_addr constant [16 x i8] c"No arguments :(\00"
@.str.4 = private unnamed_addr constant [4 x i8] c"): \00"
@.str.3 = private unnamed_addr constant [6 x i8] c"Arg (\00"
@.str.2 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.1 = private unnamed_addr constant [22 x i8] c"Number of arguments:\09\00"
@.str.0 = private unnamed_addr constant [7 x i8] c"\0D\5C\22\0C\09\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_strings_i = global i32 0
@argc = global i32 0
@argv = global i8** null

define void @_strings_main_StringArray() {
  %1 = getelementptr [7 x i8], [7 x i8]* @.str.0, i32 0, i32 0
  %2 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %3 = call i32 (i8*, ...) @printf(i8* %2, i8* %1)
  %4 = alloca i32
  %5 = load i32, i32* @argc
  %6 = sub i32 %5, 1
  store i32 %6, i32* %4
  %7 = getelementptr [22 x i8], [22 x i8]* @.str.1, i32 0, i32 0
  %8 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %9 = call i32 (i8*, ...) @printf(i8* %8, i8* %7)
  %10 = load i32, i32* %4
  %11 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %12 = call i32 (i8*, ...) @printf(i8* %11, i32 %10)
  %13 = getelementptr [2 x i8], [2 x i8]* @.str.2, i32 0, i32 0
  %14 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %15 = call i32 (i8*, ...) @printf(i8* %14, i8* %13)
  %16 = load i32, i32* %4
  %17 = add i32 0, 0
  %18 = icmp sgt i32 %16, %17
  br i1 %18, label %L1, label %L2
L1:
  br label %L4
L4:
  %19 = load i32, i32* %4
  %20 = add i32 1, 0
  %21 = add i32 %19, %20
  %22 = load i32, i32* @_strings_i
  %23 = add i32 1, 0
  %24 = add i32 %22, %23
  store i32 %24, i32* @_strings_i
  %25 = icmp sgt i32 %21, %24
  br i1 %25, label %L5, label %L6
L5:
  %26 = getelementptr [6 x i8], [6 x i8]* @.str.3, i32 0, i32 0
  %27 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %28 = call i32 (i8*, ...) @printf(i8* %27, i8* %26)
  %29 = load i32, i32* @_strings_i
  %30 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %31 = call i32 (i8*, ...) @printf(i8* %30, i32 %29)
  %32 = getelementptr [4 x i8], [4 x i8]* @.str.4, i32 0, i32 0
  %33 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %34 = call i32 (i8*, ...) @printf(i8* %33, i8* %32)
  %35 = load i32, i32* @_strings_i
  %36 = add i32 1, 0
  %37 = sub i32 %35, %36
  %38 = add i32 %37, 1
  %39 = load i8**, i8*** @argv
  %40 = getelementptr i8*, i8** %39, i32 %38
  %41 = load i8*, i8** %40
  %42 = call i32 @atoi(i8* %41)
  %43 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %44 = call i32 (i8*, ...) @printf(i8* %43, i32 %42)
  %45 = getelementptr [2 x i8], [2 x i8]* @.str.2, i32 0, i32 0
  %46 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %47 = call i32 (i8*, ...) @printf(i8* %46, i8* %45)
  br label %L4
L6:
  br label %L3
L2:
  %48 = getelementptr [16 x i8], [16 x i8]* @.str.5, i32 0, i32 0
  %49 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %50 = call i32 (i8*, ...) @printf(i8* %49, i8* %48)
  br label %L3
L3:
  %51 = getelementptr [2 x i8], [2 x i8]* @.str.2, i32 0, i32 0
  %52 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %53 = call i32 (i8*, ...) @printf(i8* %52, i8* %51)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_strings_main_StringArray()
  ret i32 0
}
