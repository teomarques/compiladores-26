@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_m4_full_mini_acc = global i32 0
@argc = global i32 0
@argv = global i8** null

define i32 @_m4_full_mini_bump_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = load i32, i32* @_m4_full_mini_acc
  %3 = load i32, i32* %1
  %4 = add i32 %2, %3
  store i32 %4, i32* @_m4_full_mini_acc
  %5 = load i32, i32* @_m4_full_mini_acc
  ret i32 %5
L1:
  ret i32 0
}

define void @_m4_full_mini_main_StringArray() {
  %1 = alloca i32
  %2 = add i32 0, 0
  store i32 %2, i32* @_m4_full_mini_acc
  %3 = add i32 0, 0
  store i32 %3, i32* %1
  br label %L1
L1:
  %4 = load i32, i32* %1
  %5 = add i32 4, 0
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %L2, label %L3
L2:
  %7 = load i32, i32* %1
  %8 = add i32 2, 0
  %9 = srem i32 %7, %8
  %10 = add i32 0, 0
  %11 = icmp eq i32 %9, %10
  br i1 %11, label %L4, label %L5
L4:
  %12 = load i32, i32* %1
  %13 = add i32 2, 0
  %14 = mul i32 %12, %13
  %15 = call i32 @_m4_full_mini_bump_int(i32 %14)
  br label %L6
L5:
  %16 = add i32 1, 0
  %17 = sub i32 0, %16
  %18 = call i32 @_m4_full_mini_bump_int(i32 %17)
  br label %L6
L6:
  %19 = load i32, i32* %1
  %20 = add i32 1, 0
  %21 = add i32 %19, %20
  store i32 %21, i32* %1
  br label %L1
L3:
  %22 = load i32, i32* @_m4_full_mini_acc
  %23 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %24 = call i32 (i8*, ...) @printf(i8* %23, i32 %22)
  %25 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %26 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %27 = call i32 (i8*, ...) @printf(i8* %26, i8* %25)
  %28 = add i1 1, 0
  br label %L11
L11:
  br i1 %28, label %L10, label %L9
L9:
  %29 = add i1 0, 0
  br label %L12
L12:
  br label %L10
L10:
  %30 = phi i1 [ %28, %L11 ], [ %29, %L12 ]
  br label %L13
L13:
  br i1 %30, label %L7, label %L8
L7:
  %31 = add i1 0, 0
  %32 = add i1 0, 0
  %33 = xor i1 %31, %32
  %34 = xor i1 %33, 1
  br label %L14
L14:
  br label %L8
L8:
  %35 = phi i1 [ %30, %L13 ], [ %34, %L14 ]
  %36 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %37 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %38 = select i1 %35, i8* %36, i8* %37
  %39 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %40 = call i32 (i8*, ...) @printf(i8* %39, i8* %38)
  %41 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %42 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %43 = call i32 (i8*, ...) @printf(i8* %42, i8* %41)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_full_mini_main_StringArray()
  ret i32 0
}
