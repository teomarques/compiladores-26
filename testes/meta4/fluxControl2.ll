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

define i32 @_fluxControl2_a_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = alloca i32
  %3 = add i32 0, 0
  store i32 %3, i32* %2
  br label %L1
L1:
  %4 = load i32, i32* %2
  %5 = add i32 10, 0
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %L2, label %L3
L2:
  %7 = load i32, i32* %1
  %8 = add i32 2, 0
  %9 = add i32 %7, %8
  store i32 %9, i32* %1
  %10 = load i32, i32* %2
  %11 = add i32 1, 0
  %12 = add i32 %10, %11
  store i32 %12, i32* %2
  br label %L1
L3:
  %13 = load i32, i32* %1
  ret i32 %13
L4:
  ret i32 0
}

define i32 @_fluxControl2_b_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = alloca i32
  %3 = add i32 0, 0
  store i32 %3, i32* %2
  br label %L1
L1:
  %4 = load i32, i32* %2
  %5 = add i32 5, 0
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %L2, label %L3
L2:
  %7 = load i32, i32* %2
  %8 = add i32 1, 0
  %9 = icmp eq i32 %7, %8
  br i1 %9, label %L4, label %L5
L4:
  %10 = load i32, i32* %1
  %11 = add i32 2, 0
  %12 = sub i32 %10, %11
  store i32 %12, i32* %1
  br label %L6
L5:
  %13 = load i32, i32* %2
  %14 = add i32 2, 0
  %15 = icmp eq i32 %13, %14
  br i1 %15, label %L7, label %L8
L7:
  %16 = load i32, i32* %1
  %17 = add i32 3, 0
  %18 = mul i32 %16, %17
  store i32 %18, i32* %1
  br label %L9
L8:
  br label %L9
L9:
  br label %L6
L6:
  %19 = load i32, i32* %2
  %20 = add i32 1, 0
  %21 = add i32 %19, %20
  store i32 %21, i32* %2
  br label %L1
L3:
  %22 = load i32, i32* %1
  ret i32 %22
L10:
  ret i32 0
}

define void @_fluxControl2_main_StringArray() {
  %1 = alloca i32
  %2 = alloca i32
  %3 = alloca i32
  %4 = add i32 0, 0
  store i32 %4, i32* %2
  %5 = add i32 0, 0
  store i32 %5, i32* %3
  %6 = add i32 0, 0
  %7 = add i32 %6, 1
  %8 = load i8**, i8*** @argv
  %9 = getelementptr i8*, i8** %8, i32 %7
  %10 = load i8*, i8** %9
  %11 = call i32 @atoi(i8* %10)
  store i32 %11, i32* %1
  %12 = load i32, i32* %1
  %13 = add i32 10, 0
  %14 = icmp slt i32 %12, %13
  br i1 %14, label %L1, label %L2
L1:
  %15 = load i32, i32* %1
  %16 = call i32 @_fluxControl2_a_int(i32 %15)
  store i32 %16, i32* %2
  br label %L3
L2:
  %17 = load i32, i32* %1
  %18 = call i32 @_fluxControl2_b_int(i32 %17)
  store i32 %18, i32* %3
  br label %L3
L3:
  %19 = load i32, i32* %2
  %20 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %21 = call i32 (i8*, ...) @printf(i8* %20, i32 %19)
  %22 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %23 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %24 = call i32 (i8*, ...) @printf(i8* %23, i8* %22)
  %25 = load i32, i32* %3
  %26 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %27 = call i32 (i8*, ...) @printf(i8* %26, i32 %25)
  %28 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i8* %28)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_fluxControl2_main_StringArray()
  ret i32 0
}
