@.fmt.d = private unnamed_addr constant [3 x i8] c"%d\00"
@.fmt.e = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.fmt.s = private unnamed_addr constant [3 x i8] c"%s\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"


declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)

define i32 @factorial__int(i32 %param.n) {
entry:
  %n = alloca i32
  store i32 %param.n, i32* %n
  %sc_tmp = alloca i1
  %t0 = load i32, i32* %n
  %t1 = add i32 0, 0
  %t2 = icmp eq i32 %t0, %t1
  br i1 %t2, label %L0, label %L1
L0:
  %t3 = add i32 0, 1
  ret i32 %t3
L1:
  br label %L2
L2:
  %t4 = load i32, i32* %n
  %t5 = load i32, i32* %n
  %t6 = add i32 0, 1
  %t7 = sub i32 %t5, %t6
  %t8 = call i32 @factorial__int(i32 %t7)
  %t9 = mul i32 %t4, %t8
  ret i32 %t9
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %args.argc = alloca i32
  %args.argv = alloca i8**
  store i32 %argc, i32* %args.argc
  store i8** %argv, i8*** %args.argv
  %sc_tmp = alloca i1
  %argument = alloca i32
  store i32 0, i32* %argument
  %t0 = add i32 0, 7
  store i32 %t0, i32* %argument
  %t1 = load i32, i32* %argument
  %t2 = call i32 @factorial__int(i32 %t1)
  %t3 = getelementptr [3 x i8], [3 x i8]* @.fmt.d, i32 0, i32 0
  %t4 = call i32 (i8*, ...) @printf(i8* %t3, i32 %t2)
  ret i32 0
}

