@.str.0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_m4_essential_fields_counter = global i32 0
@_m4_essential_fields_ratio = global double 0.0
@argc = global i32 0
@argv = global i8** null

define void @_m4_essential_fields_main_StringArray() {
  %1 = add i32 7, 0
  store i32 %1, i32* @_m4_essential_fields_counter
  %2 = load i32, i32* @_m4_essential_fields_counter
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i32 %2)
  %5 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %6 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* %6, i8* %5)
  %8 = load i32, i32* @_m4_essential_fields_counter
  %9 = add i32 3, 0
  %10 = add i32 %8, %9
  store i32 %10, i32* @_m4_essential_fields_counter
  %11 = load i32, i32* @_m4_essential_fields_counter
  %12 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %13 = call i32 (i8*, ...) @printf(i8* %12, i32 %11)
  %14 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %15 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %16 = call i32 (i8*, ...) @printf(i8* %15, i8* %14)
  %17 = fadd double 2.500000e+00, 0.0
  store double %17, double* @_m4_essential_fields_ratio
  %18 = load double, double* @_m4_essential_fields_ratio
  %19 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %20 = call i32 (i8*, ...) @printf(i8* %19, double %18)
  %21 = getelementptr [2 x i8], [2 x i8]* @.str.0, i32 0, i32 0
  %22 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %23 = call i32 (i8*, ...) @printf(i8* %22, i8* %21)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_m4_essential_fields_main_StringArray()
  ret i32 0
}
