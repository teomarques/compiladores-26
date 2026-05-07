@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
