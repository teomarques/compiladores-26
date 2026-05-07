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

define void @_test_main_StringArray() {
  %1 = alloca double
  %2 = alloca double
  %3 = add i32 11, 0
  %4 = sub i32 0, %3
  %5 = sitofp i32 %4 to double
  store double %5, double* %1
  %6 = add i32 53, 0
  %7 = sub i32 0, %6
  %8 = sitofp i32 %7 to double
  store double %8, double* %2
  %9 = load double, double* %2
  %10 = load double, double* %1
  %11 = fdiv double %9, %10
  %12 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, double %11)
  %14 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i8* %14)
  %17 = alloca i32
  %18 = alloca i32
  %19 = add i32 11, 0
  %20 = sub i32 0, %19
  store i32 %20, i32* %17
  %21 = add i32 53, 0
  %22 = sub i32 0, %21
  store i32 %22, i32* %18
  %23 = load i32, i32* %18
  %24 = load i32, i32* %17
  %25 = sdiv i32 %23, %24
  %26 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %27 = call i32 (i8*, ...) @printf(i8* %26, i32 %25)
  %28 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %29 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %30 = call i32 (i8*, ...) @printf(i8* %29, i8* %28)
  %31 = add i32 11, 0
  store i32 %31, i32* %17
  %32 = add i32 53, 0
  %33 = sub i32 0, %32
  store i32 %33, i32* %18
  %34 = load i32, i32* %18
  %35 = load i32, i32* %17
  %36 = sdiv i32 %34, %35
  %37 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %38 = call i32 (i8*, ...) @printf(i8* %37, i32 %36)
  %39 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %40 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %41 = call i32 (i8*, ...) @printf(i8* %40, i8* %39)
  %42 = add i32 2, 0
  store i32 %42, i32* %17
  %43 = add i32 85, 0
  %44 = sub i32 0, %43
  store i32 %44, i32* %18
  %45 = load i32, i32* %18
  %46 = load i32, i32* %17
  %47 = srem i32 %45, %46
  %48 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %49 = call i32 (i8*, ...) @printf(i8* %48, i32 %47)
  %50 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %51 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %52 = call i32 (i8*, ...) @printf(i8* %51, i8* %50)
  %53 = add i32 2, 0
  %54 = sub i32 0, %53
  store i32 %54, i32* %17
  %55 = add i32 85, 0
  store i32 %55, i32* %18
  %56 = load i32, i32* %18
  %57 = load i32, i32* %17
  %58 = srem i32 %56, %57
  %59 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %60 = call i32 (i8*, ...) @printf(i8* %59, i32 %58)
  %61 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %62 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %63 = call i32 (i8*, ...) @printf(i8* %62, i8* %61)
  %64 = add i32 2, 0
  store i32 %64, i32* %17
  %65 = add i32 85, 0
  store i32 %65, i32* %18
  %66 = load i32, i32* %18
  %67 = load i32, i32* %17
  %68 = srem i32 %66, %67
  %69 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %70 = call i32 (i8*, ...) @printf(i8* %69, i32 %68)
  %71 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %72 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %73 = call i32 (i8*, ...) @printf(i8* %72, i8* %71)
  %74 = add i32 0, 0
  %75 = sitofp i32 %74 to double
  store double %75, double* %1
  store double %75, double* %2
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_test_main_StringArray()
  ret i32 0
}
