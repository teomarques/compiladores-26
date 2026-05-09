@.fmt.d = private unnamed_addr constant [3 x i8] c"%d\00"
@.fmt.e = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.fmt.s = private unnamed_addr constant [3 x i8] c"%s\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"

@.str.0 = private unnamed_addr constant [19 x i8] c"I have a parameter\00"
@.str.1 = private unnamed_addr constant [25 x i8] c"I don't have a parameter\00"
@.str.2 = private unnamed_addr constant [2 x i8] c"\0A\00"

@overload = global i32 0

declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)

define void @overload__int(i32 %param.n) {
entry:
  %n = alloca i32
  store i32 %param.n, i32* %n
  %t0 = getelementptr [19 x i8], [19 x i8]* @.str.0, i32 0, i32 0
  %t1 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t2 = call i32 (i8*, ...) @printf(i8* %t1, i8* %t0)
  ret void
}

define void @overload__() {
entry:
  %t0 = getelementptr [25 x i8], [25 x i8]* @.str.1, i32 0, i32 0
  %t1 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t2 = call i32 (i8*, ...) @printf(i8* %t1, i8* %t0)
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %args.argc = alloca i32
  %args.argv = alloca i8**
  store i32 %argc, i32* %args.argc
  store i8** %argv, i8*** %args.argv
  %t0 = load i32, i32* %args.argc
  %t1 = sub i32 %t0, 1
  %t2 = add i32 0, 0
  %t3 = icmp sgt i32 %t1, %t2
  br i1 %t3, label %L0, label %L1
L0:
  %t4 = add i32 0, 0
  %t5 = add i32 %t4, 1
  %t6 = load i8**, i8*** %args.argv
  %t7 = getelementptr i8*, i8** %t6, i32 %t5
  %t8 = load i8*, i8** %t7
  %t9 = call i32 @atoi(i8* %t8)
  call void @overload__int(i32 %t9)
  br label %L2
L1:
  call void @overload__()
  br label %L2
L2:
  %t10 = getelementptr [2 x i8], [2 x i8]* @.str.2, i32 0, i32 0
  %t11 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t12 = call i32 (i8*, ...) @printf(i8* %t11, i8* %t10)
  ret i32 0
}

