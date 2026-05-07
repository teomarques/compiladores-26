@.str.2 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.1 = private unnamed_addr constant [3 x i8] c"|\0A\00"
@.str.0 = private unnamed_addr constant [10 x i8] c"Is pair: \00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@argc = global i32 0
@argv = global i8** null

define i32 @_fluxControl3_b_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = alloca i32
  %3 = add i32 0, 0
  store i32 %3, i32* %2
  %4 = alloca i32
  %5 = alloca i32
  %6 = add i32 0, 0
  store i32 %6, i32* %4
  %7 = add i32 0, 0
  store i32 %7, i32* %5
  %8 = alloca i32
  %9 = add i32 0, 0
  store i32 %9, i32* %8
  br label %L1
L1:
  %10 = load i32, i32* %2
  %11 = add i32 5, 0
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %L2, label %L3
L2:
  %13 = load i32, i32* %2
  %14 = add i32 1, 0
  %15 = icmp eq i32 %13, %14
  br i1 %15, label %L4, label %L5
L4:
  %16 = load i32, i32* %1
  %17 = add i32 2, 0
  %18 = sub i32 %16, %17
  store i32 %18, i32* %1
  %19 = add i32 0, 0
  store i32 %19, i32* %4
  %20 = add i32 0, 0
  store i32 %20, i32* %5
  br label %L7
L7:
  %21 = load i32, i32* %4
  %22 = add i32 100, 0
  %23 = icmp slt i32 %21, %22
  br i1 %23, label %L8, label %L9
L8:
  br label %L10
L10:
  %24 = load i32, i32* %5
  %25 = add i32 50, 0
  %26 = icmp slt i32 %24, %25
  br i1 %26, label %L11, label %L12
L11:
  %27 = load i32, i32* %1
  %28 = add i32 2, 0
  %29 = add i32 %27, %28
  store i32 %29, i32* %1
  %30 = load i32, i32* %5
  %31 = add i32 2, 0
  %32 = srem i32 %30, %31
  %33 = add i32 0, 0
  %34 = icmp eq i32 %32, %33
  br i1 %34, label %L13, label %L14
L13:
  %35 = load i32, i32* %8
  %36 = add i32 1, 0
  %37 = add i32 %35, %36
  store i32 %37, i32* %8
  br label %L15
L14:
  br label %L15
L15:
  %38 = load i32, i32* %5
  %39 = add i32 1, 0
  %40 = add i32 %38, %39
  store i32 %40, i32* %5
  br label %L10
L12:
  %41 = add i32 0, 0
  store i32 %41, i32* %5
  %42 = load i32, i32* %4
  %43 = add i32 1, 0
  %44 = add i32 %42, %43
  store i32 %44, i32* %4
  br label %L7
L9:
  br label %L6
L5:
  %45 = load i32, i32* %2
  %46 = add i32 2, 0
  %47 = icmp eq i32 %45, %46
  br i1 %47, label %L16, label %L17
L16:
  %48 = load i32, i32* %1
  %49 = add i32 3, 0
  %50 = mul i32 %48, %49
  store i32 %50, i32* %1
  br label %L18
L17:
  br label %L18
L18:
  br label %L6
L6:
  %51 = load i32, i32* %2
  %52 = add i32 1, 0
  %53 = add i32 %51, %52
  store i32 %53, i32* %2
  br label %L1
L3:
  %54 = getelementptr [10 x i8], [10 x i8]* @.str.0, i32 0, i32 0
  %55 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %56 = call i32 (i8*, ...) @printf(i8* %55, i8* %54)
  %57 = load i32, i32* %8
  %58 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %59 = call i32 (i8*, ...) @printf(i8* %58, i32 %57)
  %60 = getelementptr [3 x i8], [3 x i8]* @.str.1, i32 0, i32 0
  %61 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %62 = call i32 (i8*, ...) @printf(i8* %61, i8* %60)
  %63 = load i32, i32* %1
  ret i32 %63
L19:
  ret i32 0
}

define void @_fluxControl3_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 40, 0
  store i32 %2, i32* %1
  %3 = load i32, i32* %1
  %4 = call i32 @_fluxControl3_b_int(i32 %3)
  store i32 %4, i32* %1
  %5 = add i32 0, 0
  %6 = add i32 %5, 1
  %7 = load i8**, i8*** @argv
  %8 = getelementptr i8*, i8** %7, i32 %6
  %9 = load i8*, i8** %8
  %10 = call i32 @atoi(i8* %9)
  %11 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %12 = call i32 (i8*, ...) @printf(i8* %11, i32 %10)
  %13 = getelementptr [2 x i8], [2 x i8]* @.str.2, i32 0, i32 0
  %14 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %15 = call i32 (i8*, ...) @printf(i8* %14, i8* %13)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_fluxControl3_main_StringArray()
  ret i32 0
}
