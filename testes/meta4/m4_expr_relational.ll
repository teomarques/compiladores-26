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

define void @_m4_expr_relational_main_StringArray() {
  %1 = add i32 3, 0
  %2 = add i32 5, 0
  %3 = icmp slt i32 %1, %2
  %4 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %5 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %6 = select i1 %3, i8* %4, i8* %5
  %7 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i8* %6)
  %9 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %10 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* %10, i8* %9)
  %12 = add i32 5, 0
  %13 = add i32 4, 0
  %14 = icmp sgt i32 %12, %13
  %15 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %16 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %17 = select i1 %14, i8* %15, i8* %16
  %18 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %19 = call i32 (i8*, ...) @printf(i8* %18, i8* %17)
  %20 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %21 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %22 = call i32 (i8*, ...) @printf(i8* %21, i8* %20)
  %23 = add i32 3, 0
  %24 = add i32 3, 0
  %25 = icmp sle i32 %23, %24
  %26 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %27 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %28 = select i1 %25, i8* %26, i8* %27
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i8* %28)
  %31 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %32 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %33 = call i32 (i8*, ...) @printf(i8* %32, i8* %31)
  %34 = add i32 4, 0
  %35 = add i32 5, 0
  %36 = icmp sge i32 %34, %35
  %37 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %38 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %39 = select i1 %36, i8* %37, i8* %38
  %40 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %41 = call i32 (i8*, ...) @printf(i8* %40, i8* %39)
  %42 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %43 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %44 = call i32 (i8*, ...) @printf(i8* %43, i8* %42)
  %45 = add i32 2, 0
  %46 = add i32 2, 0
  %47 = icmp eq i32 %45, %46
  %48 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %49 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %50 = select i1 %47, i8* %48, i8* %49
  %51 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %52 = call i32 (i8*, ...) @printf(i8* %51, i8* %50)
  %53 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %54 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %55 = call i32 (i8*, ...) @printf(i8* %54, i8* %53)
  %56 = add i32 2, 0
  %57 = add i32 3, 0
  %58 = icmp ne i32 %56, %57
  %59 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %60 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %61 = select i1 %58, i8* %59, i8* %60
  %62 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %63 = call i32 (i8*, ...) @printf(i8* %62, i8* %61)
  %64 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %65 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %66 = call i32 (i8*, ...) @printf(i8* %65, i8* %64)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_expr_relational_main_StringArray()
  ret i32 0
}
