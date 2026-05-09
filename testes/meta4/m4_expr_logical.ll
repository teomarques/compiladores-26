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

define void @_m4_expr_logical_main_StringArray() {
  %1 = alloca i1
  %2 = alloca i1
  %3 = add i1 1, 0
  store i1 %3, i1* %1
  %4 = add i1 0, 0
  store i1 %4, i1* %2
  %5 = load i1, i1* %1
  br label %L3
L3:
  br i1 %5, label %L1, label %L2
L1:
  %6 = load i1, i1* %2
  br label %L4
L4:
  br label %L2
L2:
  %7 = phi i1 [ %5, %L3 ], [ %6, %L4 ]
  %8 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %9 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %10 = select i1 %7, i8* %8, i8* %9
  %11 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %12 = call i32 (i8*, ...) @printf(i8* %11, i8* %10)
  %13 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %14 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %15 = call i32 (i8*, ...) @printf(i8* %14, i8* %13)
  %16 = load i1, i1* %1
  br label %L7
L7:
  br i1 %16, label %L6, label %L5
L5:
  %17 = load i1, i1* %2
  br label %L8
L8:
  br label %L6
L6:
  %18 = phi i1 [ %16, %L7 ], [ %17, %L8 ]
  %19 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %20 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %21 = select i1 %18, i8* %19, i8* %20
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i8* %21)
  %24 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %25 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %26 = call i32 (i8*, ...) @printf(i8* %25, i8* %24)
  %27 = load i1, i1* %1
  %28 = load i1, i1* %2
  %29 = xor i1 %27, %28
  %30 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %31 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %32 = select i1 %29, i8* %30, i8* %31
  %33 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %34 = call i32 (i8*, ...) @printf(i8* %33, i8* %32)
  %35 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %36 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %37 = call i32 (i8*, ...) @printf(i8* %36, i8* %35)
  %38 = load i1, i1* %2
  %39 = xor i1 %38, 1
  %40 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %41 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %42 = select i1 %39, i8* %40, i8* %41
  %43 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %44 = call i32 (i8*, ...) @printf(i8* %43, i8* %42)
  %45 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %46 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %47 = call i32 (i8*, ...) @printf(i8* %46, i8* %45)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_expr_logical_main_StringArray()
  ret i32 0
}
