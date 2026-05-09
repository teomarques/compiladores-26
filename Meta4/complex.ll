@.fmt.d = private unnamed_addr constant [3 x i8] c"%d\00"
@.fmt.e = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.fmt.s = private unnamed_addr constant [3 x i8] c"%s\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"

@.str.0 = private unnamed_addr constant [20 x i8] c"I have two doubles\0A\00"
@.str.1 = private unnamed_addr constant [19 x i8] c"I have one double\0A\00"
@.str.2 = private unnamed_addr constant [16 x i8] c"I have one int\0A\00"
@.str.3 = private unnamed_addr constant [19 x i8] c"I'm the fake main\0A\00"
@.str.4 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.5 = private unnamed_addr constant [5 x i8] c"\0Aa:\0A\00"
@.str.6 = private unnamed_addr constant [11 x i8] c"Before if\0A\00"
@.str.7 = private unnamed_addr constant [11 x i8] c"After ifs\0A\00"
@.str.8 = private unnamed_addr constant [14 x i8] c"Before while\0A\00"

@overload = global i32 0

declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)

define i1 @overload__boolean(i1 %param.n) {
entry:
  %n = alloca i1
  store i1 %param.n, i1* %n
  %sc_tmp = alloca i1
  %t0 = getelementptr [20 x i8], [20 x i8]* @.str.0, i32 0, i32 0
  %t1 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t2 = call i32 (i8*, ...) @printf(i8* %t1, i8* %t0)
  %t3 = add i1 0, 0
  ret i1 %t3
}

define double @overload__double(double %param.n) {
entry:
  %n = alloca double
  store double %param.n, double* %n
  %sc_tmp = alloca i1
  %t0 = getelementptr [19 x i8], [19 x i8]* @.str.1, i32 0, i32 0
  %t1 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t2 = call i32 (i8*, ...) @printf(i8* %t1, i8* %t0)
  %t3 = fadd double 0.0, 2.20000000000000018e+00
  %t4 = add i32 0, 2
  %t5 = sitofp i32 %t4 to double
  %t6 = fmul double %t3, %t5
  %t7 = add i32 0, 2
  %t8 = call i32 @main__int(i32 %t7)
  %t9 = add i32 0, 5
  %t10 = sdiv i32 %t8, %t9
  %t11 = add i32 0, 6
  %t12 = mul i32 %t10, %t11
  %t13 = sitofp i32 %t12 to double
  %t14 = fadd double %t6, %t13
  %t15 = add i32 0, 2
  %t16 = add i32 0, 3
  %t17 = mul i32 %t15, %t16
  %t18 = sitofp i32 %t17 to double
  %t19 = fadd double %t14, %t18
  %t20 = add i32 0, 3
  %t21 = add i32 0, 4
  %t22 = mul i32 %t20, %t21
  %t23 = sitofp i32 %t22 to double
  %t24 = fadd double %t19, %t23
  %t25 = add i32 0, 2
  %t26 = add i32 0, 8
  %t27 = mul i32 %t25, %t26
  %t28 = sitofp i32 %t27 to double
  %t29 = fadd double %t24, %t28
  %t30 = add i32 0, 5
  %t31 = add i32 0, 9
  %t32 = mul i32 %t30, %t31
  %t33 = sitofp i32 %t32 to double
  %t34 = fadd double %t29, %t33
  %t35 = add i32 0, 2
  %t36 = add i32 0, 2
  %t37 = call i32 @overload__int__int(i32 %t35, i32 %t36)
  %t38 = sitofp i32 %t37 to double
  %t39 = fadd double %t34, %t38
  ret double %t39
}

define i32 @overload__double__double(double %param.b, double %param.b1) {
entry:
  %b = alloca double
  store double %param.b, double* %b
  %b1 = alloca double
  store double %param.b1, double* %b1
  %sc_tmp = alloca i1
  %overload = alloca double
  store double 0.0, double* %overload
  %t0 = add i32 0, 2
  %t1 = load i32, i32* %overload
  %t2 = add i32 %t0, %t1
  store i32 %t2, i32* %overload
  %t3 = getelementptr [16 x i8], [16 x i8]* @.str.2, i32 0, i32 0
  %t4 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t5 = call i32 (i8*, ...) @printf(i8* %t4, i8* %t3)
  %t6 = add i32 0, 5
  ret i32 %t6
}

define i32 @overload__int__int(i32 %param.b, i32 %param.b1) {
entry:
  %b = alloca i32
  store i32 %param.b, i32* %b
  %b1 = alloca i32
  store i32 %param.b1, i32* %b1
  %sc_tmp = alloca i1
  %t0 = add i32 0, 5
  ret i32 %t0
}

define i32 @main__int(i32 %param.n) {
entry:
  %n = alloca i32
  store i32 %param.n, i32* %n
  %sc_tmp = alloca i1
  %t0 = getelementptr [19 x i8], [19 x i8]* @.str.3, i32 0, i32 0
  %t1 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t2 = call i32 (i8*, ...) @printf(i8* %t1, i8* %t0)
  %t3 = add i32 0, 5
  ret i32 %t3
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %args.argc = alloca i32
  %args.argv = alloca i8**
  store i32 %argc, i32* %args.argc
  store i8** %argv, i8*** %args.argv
  %sc_tmp = alloca i1
  %a = alloca i32
  store i32 0, i32* %a
  %d = alloca double
  store double 0.0, double* %d
  %b = alloca i32
  store i32 0, i32* %b
  %b1 = alloca i1
  store i1 false, i1* %b1
  %d1 = alloca double
  store double 0.0, double* %d1
  %i = alloca i32
  store i32 0, i32* %i
  %j = alloca i32
  store i32 0, i32* %j
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
  %t10 = sitofp i32 %t9 to double
  %t11 = call double @overload__double(double %t10)
  br label %L2
L1:
  %t12 = add i32 0, 5
  %t13 = sitofp i32 %t12 to double
  %t14 = call double @overload__double(double %t13)
  br label %L2
L2:
  %t15 = add i1 0, 0
  %t16 = xor i1 %t15, 1
  br i1 %t16, label %L3, label %L4
L3:
  %t17 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %t18 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t19 = call i32 (i8*, ...) @printf(i8* %t18, i8* %t17)
  br label %L5
L4:
  %t20 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %t21 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t22 = call i32 (i8*, ...) @printf(i8* %t21, i8* %t20)
  br label %L5
L5:
  %t23 = getelementptr [2 x i8], [2 x i8]* @.str.4, i32 0, i32 0
  %t24 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t25 = call i32 (i8*, ...) @printf(i8* %t24, i8* %t23)
  %t26 = add i32 0, 2222222
  store i32 %t26, i32* %a
  %t27 = fadd double 0.0, 2.32323320000000000e+08
  store double %t27, double* %d
  %t28 = add i32 0, 3
  store i32 %t28, i32* %b
  %t29 = load i32, i32* %a
  %t30 = getelementptr [3 x i8], [3 x i8]* @.fmt.d, i32 0, i32 0
  %t31 = call i32 (i8*, ...) @printf(i8* %t30, i32 %t29)
  %t32 = add i1 0, 1
  store i1 %t32, i1* %b1
  %t33 = load i32, i32* %a
  %t34 = load i32, i32* %a
  %t35 = add i32 %t33, %t34
  %t36 = load i32, i32* %a
  %t37 = add i32 %t35, %t36
  %t38 = add i32 0, 8
  %t39 = add i32 0, 2
  %t40 = srem i32 %t38, %t39
  %t41 = add i32 %t37, %t40
  %t42 = load double, double* %d
  %t43 = add i32 0, 3
  %t44 = sitofp i32 %t43 to double
  %t45 = fdiv double %t42, %t44
  %t46 = sitofp i32 %t41 to double
  %t47 = fadd double %t46, %t45
  %t48 = load i32, i32* %a
  %t49 = load double, double* %d
  %t50 = sitofp i32 %t48 to double
  %t51 = fmul double %t50, %t49
  %t52 = fadd double %t47, %t51
  %t53 = load double, double* %d
  %t54 = fadd double %t52, %t53
  %t55 = load i32, i32* %a
  %t56 = sitofp i32 %t55 to double
  %t57 = fsub double %t54, %t56
  %t58 = load i32, i32* %a
  %t59 = load i32, i32* %b
  %t60 = mul i32 %t58, %t59
  %t61 = load double, double* %d
  %t62 = sitofp i32 %t60 to double
  %t63 = fdiv double %t62, %t61
  %t64 = load i32, i32* %a
  %t65 = load i32, i32* %b
  %t66 = mul i32 %t64, %t65
  %t67 = load double, double* %d
  %t68 = sitofp i32 %t66 to double
  %t69 = fdiv double %t68, %t67
  %t70 = fmul double %t63, %t69
  %t71 = fadd double %t57, %t70
  %t72 = fadd double 0.0, 2.20000000000000018e+00
  %t73 = call double @overload__double(double %t72)
  %t74 = fadd double %t71, %t73
  %t75 = add i32 0, 1
  %t76 = add i32 0, 1
  %t77 = call i32 @overload__int__int(i32 %t75, i32 %t76)
  %t78 = sitofp i32 %t77 to double
  store double %t78, double* %d
  %t79 = fadd double %t74, %t78
  %t80 = add i32 0, 5
  %t81 = call i32 @main__int(i32 %t80)
  %t82 = sitofp i32 %t81 to double
  %t83 = fadd double %t79, %t82
  %t84 = load i32, i32* %args.argc
  %t85 = sub i32 %t84, 1
  %t86 = add i32 0, 1
  %t87 = sub i32 %t85, %t86
  %t88 = add i32 %t87, 1
  %t89 = load i8**, i8*** %args.argv
  %t90 = getelementptr i8*, i8** %t89, i32 %t88
  %t91 = load i8*, i8** %t90
  %t92 = call i32 @atoi(i8* %t91)
  %t93 = sitofp i32 %t92 to double
  %t94 = fadd double %t83, %t93
  %t95 = getelementptr [6 x i8], [6 x i8]* @.fmt.e, i32 0, i32 0
  %t96 = call i32 (i8*, ...) @printf(i8* %t95, double %t94)
  %t97 = getelementptr [5 x i8], [5 x i8]* @.str.5, i32 0, i32 0
  %t98 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t99 = call i32 (i8*, ...) @printf(i8* %t98, i8* %t97)
  %t100 = load i32, i32* %a
  %t101 = getelementptr [3 x i8], [3 x i8]* @.fmt.d, i32 0, i32 0
  %t102 = call i32 (i8*, ...) @printf(i8* %t101, i32 %t100)
  %t103 = getelementptr [11 x i8], [11 x i8]* @.str.6, i32 0, i32 0
  %t104 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t105 = call i32 (i8*, ...) @printf(i8* %t104, i8* %t103)
  %t106 = load i32, i32* %a
  %t107 = add i32 0, 2
  %t108 = add i32 0, 2
  %t109 = mul i32 %t107, %t108
  %t110 = add i32 0, 3
  %t111 = add i32 0, 5
  %t112 = sdiv i32 %t110, %t111
  %t113 = add i32 %t109, %t112
  %t114 = add i32 0, 5
  %t115 = call i32 @main__int(i32 %t114)
  %t116 = add i32 %t113, %t115
  %t117 = icmp sge i32 %t106, %t116
  br i1 %t117, label %L6, label %L7
L6:
  %t118 = load i32, i32* %a
  %t119 = add i32 0, 2
  %t120 = icmp sle i32 %t118, %t119
  br i1 %t120, label %L9, label %L10
L9:
  %t121 = add i32 0, 2
  %t122 = load i32, i32* %a
  %t123 = icmp sge i32 %t121, %t122
  br i1 %t123, label %L12, label %L13
L12:
  %t124 = add i32 0, 2
  %t125 = load i32, i32* %a
  %t126 = icmp sle i32 %t124, %t125
  br i1 %t126, label %L15, label %L16
L15:
  %t127 = load i32, i32* %a
  %t128 = add i32 0, 2
  %t129 = icmp eq i32 %t127, %t128
  br i1 %t129, label %L18, label %L19
L18:
  %t130 = add i32 0, 2
  %t131 = load i32, i32* %a
  %t132 = icmp eq i32 %t130, %t131
  br i1 %t132, label %L21, label %L22
L21:
  %t133 = load i32, i32* %a
  %t134 = add i32 0, 2
  %t135 = icmp sgt i32 %t133, %t134
  br i1 %t135, label %L24, label %L25
L24:
  %t136 = load i32, i32* %a
  %t137 = add i32 0, 2
  %t138 = icmp slt i32 %t136, %t137
  br i1 %t138, label %L27, label %L28
L27:
  %t139 = add i32 0, 2
  %t140 = load i32, i32* %a
  %t141 = icmp sgt i32 %t139, %t140
  br i1 %t141, label %L30, label %L31
L30:
  %t142 = add i32 0, 2
  %t143 = load i32, i32* %a
  %t144 = icmp slt i32 %t142, %t143
  br i1 %t144, label %L33, label %L34
L33:
  %t145 = load i32, i32* %a
  %t146 = add i32 0, 2
  %t147 = icmp ne i32 %t145, %t146
  br i1 %t147, label %L36, label %L37
L36:
  %t148 = add i32 0, 2
  %t149 = load i32, i32* %a
  %t150 = icmp ne i32 %t148, %t149
  br i1 %t150, label %L39, label %L40
L39:
  br label %L41
L40:
  br label %L41
L41:
  br label %L38
L37:
  br label %L38
L38:
  br label %L35
L34:
  br label %L35
L35:
  br label %L32
L31:
  br label %L32
L32:
  br label %L29
L28:
  br label %L29
L29:
  br label %L26
L25:
  br label %L26
L26:
  br label %L23
L22:
  br label %L23
L23:
  br label %L20
L19:
  br label %L20
L20:
  br label %L17
L16:
  br label %L17
L17:
  br label %L14
L13:
  br label %L14
L14:
  br label %L11
L10:
  br label %L11
L11:
  br label %L8
L7:
  br label %L8
L8:
  %t151 = getelementptr [11 x i8], [11 x i8]* @.str.7, i32 0, i32 0
  %t152 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t153 = call i32 (i8*, ...) @printf(i8* %t152, i8* %t151)
  %t154 = load double, double* %d
  %t155 = add i32 0, 2
  %t156 = sitofp i32 %t155 to double
  %t157 = fcmp oge double %t154, %t156
  br i1 %t157, label %L42, label %L43
L42:
  %t158 = load double, double* %d
  %t159 = add i32 0, 2
  %t160 = sitofp i32 %t159 to double
  %t161 = fcmp ole double %t158, %t160
  br i1 %t161, label %L45, label %L46
L45:
  %t162 = add i32 0, 2
  %t163 = load double, double* %d
  %t164 = sitofp i32 %t162 to double
  %t165 = fcmp oge double %t164, %t163
  br i1 %t165, label %L48, label %L49
L48:
  %t166 = add i32 0, 2
  %t167 = load double, double* %d
  %t168 = sitofp i32 %t166 to double
  %t169 = fcmp ole double %t168, %t167
  br i1 %t169, label %L51, label %L52
L51:
  %t170 = load double, double* %d
  %t171 = add i32 0, 2
  %t172 = sitofp i32 %t171 to double
  %t173 = fcmp oeq double %t170, %t172
  br i1 %t173, label %L54, label %L55
L54:
  %t174 = add i32 0, 2
  %t175 = load double, double* %d
  %t176 = sitofp i32 %t174 to double
  %t177 = fcmp oeq double %t176, %t175
  br i1 %t177, label %L57, label %L58
L57:
  %t178 = load double, double* %d
  %t179 = add i32 0, 2
  %t180 = sitofp i32 %t179 to double
  %t181 = fcmp ogt double %t178, %t180
  br i1 %t181, label %L60, label %L61
L60:
  %t182 = load double, double* %d
  %t183 = add i32 0, 2
  %t184 = sitofp i32 %t183 to double
  %t185 = fcmp olt double %t182, %t184
  br i1 %t185, label %L63, label %L64
L63:
  %t186 = add i32 0, 2
  %t187 = load double, double* %d
  %t188 = sitofp i32 %t186 to double
  %t189 = fcmp ogt double %t188, %t187
  br i1 %t189, label %L66, label %L67
L66:
  %t190 = add i32 0, 2
  %t191 = load double, double* %d
  %t192 = sitofp i32 %t190 to double
  %t193 = fcmp olt double %t192, %t191
  br i1 %t193, label %L69, label %L70
L69:
  %t194 = load double, double* %d
  %t195 = add i32 0, 2
  %t196 = sitofp i32 %t195 to double
  %t197 = fcmp one double %t194, %t196
  br i1 %t197, label %L72, label %L73
L72:
  %t198 = add i32 0, 2
  %t199 = load double, double* %d
  %t200 = sitofp i32 %t198 to double
  %t201 = fcmp one double %t200, %t199
  br i1 %t201, label %L75, label %L76
L75:
  br label %L77
L76:
  br label %L77
L77:
  br label %L74
L73:
  br label %L74
L74:
  br label %L71
L70:
  br label %L71
L71:
  br label %L68
L67:
  br label %L68
L68:
  br label %L65
L64:
  br label %L65
L65:
  br label %L62
L61:
  br label %L62
L62:
  br label %L59
L58:
  br label %L59
L59:
  br label %L56
L55:
  br label %L56
L56:
  br label %L53
L52:
  br label %L53
L53:
  br label %L50
L49:
  br label %L50
L50:
  br label %L47
L46:
  br label %L47
L47:
  br label %L44
L43:
  br label %L44
L44:
  %t202 = load double, double* %d
  %t203 = fadd double 0.0, 2.20000000000000018e+00
  %t204 = fcmp oge double %t202, %t203
  br i1 %t204, label %L78, label %L79
L78:
  %t205 = load double, double* %d
  %t206 = fadd double 0.0, 2.20000000000000018e+00
  %t207 = fcmp ole double %t205, %t206
  br i1 %t207, label %L81, label %L82
L81:
  %t208 = fadd double 0.0, 2.20000000000000018e+00
  %t209 = load double, double* %d
  %t210 = fcmp oge double %t208, %t209
  br i1 %t210, label %L84, label %L85
L84:
  %t211 = fadd double 0.0, 2.20000000000000018e+00
  %t212 = load double, double* %d
  %t213 = fcmp ole double %t211, %t212
  br i1 %t213, label %L87, label %L88
L87:
  %t214 = load double, double* %d
  %t215 = fadd double 0.0, 2.20000000000000018e+00
  %t216 = fcmp oeq double %t214, %t215
  br i1 %t216, label %L90, label %L91
L90:
  %t217 = fadd double 0.0, 2.20000000000000018e+00
  %t218 = load double, double* %d
  %t219 = fcmp oeq double %t217, %t218
  br i1 %t219, label %L93, label %L94
L93:
  %t220 = load double, double* %d
  %t221 = fadd double 0.0, 2.20000000000000018e+00
  %t222 = fcmp ogt double %t220, %t221
  br i1 %t222, label %L96, label %L97
L96:
  %t223 = load double, double* %d
  %t224 = fadd double 0.0, 2.20000000000000018e+00
  %t225 = fcmp olt double %t223, %t224
  br i1 %t225, label %L99, label %L100
L99:
  %t226 = fadd double 0.0, 2.20000000000000018e+00
  %t227 = load double, double* %d
  %t228 = fcmp ogt double %t226, %t227
  br i1 %t228, label %L102, label %L103
L102:
  %t229 = fadd double 0.0, 2.20000000000000018e+00
  %t230 = load double, double* %d
  %t231 = fcmp olt double %t229, %t230
  br i1 %t231, label %L105, label %L106
L105:
  %t232 = load double, double* %d
  %t233 = fadd double 0.0, 2.20000000000000018e+00
  %t234 = fcmp one double %t232, %t233
  br i1 %t234, label %L108, label %L109
L108:
  %t235 = fadd double 0.0, 2.20000000000000018e+00
  %t236 = load double, double* %d
  %t237 = fcmp one double %t235, %t236
  br i1 %t237, label %L111, label %L112
L111:
  br label %L113
L112:
  br label %L113
L113:
  br label %L110
L109:
  br label %L110
L110:
  br label %L107
L106:
  br label %L107
L107:
  br label %L104
L103:
  br label %L104
L104:
  br label %L101
L100:
  br label %L101
L101:
  br label %L98
L97:
  br label %L98
L98:
  br label %L95
L94:
  br label %L95
L95:
  br label %L92
L91:
  br label %L92
L92:
  br label %L89
L88:
  br label %L89
L89:
  br label %L86
L85:
  br label %L86
L86:
  br label %L83
L82:
  br label %L83
L83:
  br label %L80
L79:
  br label %L80
L80:
  %t238 = load i1, i1* %b1
  %t239 = add i1 0, 1
  %t240 = icmp eq i1 %t238, %t239
  br i1 %t240, label %L123, label %L124
L123:
  %t241 = load double, double* %d
  %t242 = fadd double 0.0, 2.20000000000000018e+00
  %t243 = fcmp oge double %t241, %t242
  br label %L125
L124:
  br label %L125
L125:
  %t244 = phi i1 [ %t243, %L123 ], [ false, %L124 ]
  br i1 %t244, label %L120, label %L121
L120:
  %t245 = add i32 0, 2
  %t246 = load double, double* %d
  %t247 = sitofp i32 %t245 to double
  %t248 = fcmp oge double %t247, %t246
  br label %L122
L121:
  br label %L122
L122:
  %t249 = phi i1 [ %t248, %L120 ], [ false, %L121 ]
  br i1 %t249, label %L117, label %L118
L117:
  %t250 = fadd double 0.0, 2.20000000000000018e+00
  %t251 = load double, double* %d
  %t252 = fcmp ole double %t250, %t251
  br label %L119
L118:
  br label %L119
L119:
  %t253 = phi i1 [ %t252, %L117 ], [ false, %L118 ]
  br i1 %t253, label %L114, label %L115
L115:
  %t254 = fadd double 0.0, 2.20000000000000018e+00
  %t255 = load double, double* %d
  %t256 = fadd double 0.0, 2.29999999999999982e+00
  %t257 = add i32 0, 2
  store i32 %t257, i32* %a
  %t258 = sitofp i32 %t257 to double
  %t259 = fadd double %t256, %t258
  %t260 = add i32 0, 2
  %t261 = sitofp i32 %t260 to double
  store double %t261, double* %d
  %t262 = fadd double %t259, %t261
  store double %t262, double* %d
  %t263 = fadd double %t255, %t262
  %t264 = fcmp oeq double %t254, %t263
  br label %L116
L114:
  br label %L116
L116:
  %t265 = phi i1 [ %t264, %L115 ], [ true, %L114 ]
  br i1 %t265, label %L126, label %L127
L126:
  %t266 = add i1 0, 1
  %t267 = load i1, i1* %b1
  %t268 = icmp eq i1 %t266, %t267
  br i1 %t268, label %L129, label %L130
L129:
  %t269 = load i1, i1* %b1
  %t270 = add i1 0, 1
  %t271 = icmp ne i1 %t269, %t270
  br i1 %t271, label %L132, label %L133
L132:
  %t272 = add i1 0, 1
  %t273 = load i1, i1* %b1
  %t274 = icmp ne i1 %t272, %t273
  br i1 %t274, label %L135, label %L136
L135:
  br label %L137
L136:
  br label %L137
L137:
  br label %L134
L133:
  br label %L134
L134:
  br label %L131
L130:
  br label %L131
L131:
  br label %L128
L127:
  br label %L128
L128:
  %t275 = load i1, i1* %b1
  br i1 %t275, label %L144, label %L145
L144:
  %t276 = add i1 0, 1
  br label %L146
L145:
  br label %L146
L146:
  %t277 = phi i1 [ %t276, %L144 ], [ false, %L145 ]
  br i1 %t277, label %L141, label %L142
L142:
  %t278 = load i1, i1* %b1
  br label %L143
L141:
  br label %L143
L143:
  %t279 = phi i1 [ %t278, %L142 ], [ true, %L141 ]
  br i1 %t279, label %L138, label %L139
L139:
  %t280 = add i1 0, 1
  br i1 %t280, label %L147, label %L148
L147:
  %t281 = load i1, i1* %b1
  %t282 = xor i1 %t281, 1
  %t283 = add i1 0, 0
  %t284 = xor i1 %t282, %t283
  br label %L149
L148:
  br label %L149
L149:
  %t285 = phi i1 [ %t284, %L147 ], [ false, %L148 ]
  br label %L140
L138:
  br label %L140
L140:
  %t286 = phi i1 [ %t285, %L149 ], [ true, %L138 ]
  br i1 %t286, label %L150, label %L151
L150:
  %t287 = add i1 0, 1
  br i1 %t287, label %L159, label %L160
L159:
  %t288 = load i1, i1* %b1
  br label %L161
L160:
  br label %L161
L161:
  %t289 = phi i1 [ %t288, %L159 ], [ false, %L160 ]
  br i1 %t289, label %L156, label %L157
L157:
  %t290 = add i1 0, 1
  br label %L158
L156:
  br label %L158
L158:
  %t291 = phi i1 [ %t290, %L157 ], [ true, %L156 ]
  br i1 %t291, label %L153, label %L154
L154:
  %t292 = load i1, i1* %b1
  br i1 %t292, label %L162, label %L163
L162:
  %t293 = add i1 0, 0
  %t294 = load i1, i1* %b1
  %t295 = xor i1 %t294, 1
  %t296 = xor i1 %t293, %t295
  br label %L164
L163:
  br label %L164
L164:
  %t297 = phi i1 [ %t296, %L162 ], [ false, %L163 ]
  br label %L155
L153:
  br label %L155
L155:
  %t298 = phi i1 [ %t297, %L164 ], [ true, %L153 ]
  br i1 %t298, label %L165, label %L166
L165:
  br label %L167
L166:
  br label %L167
L167:
  br label %L152
L151:
  br label %L152
L152:
  %t299 = getelementptr [11 x i8], [11 x i8]* @.str.7, i32 0, i32 0
  %t300 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t301 = call i32 (i8*, ...) @printf(i8* %t300, i8* %t299)
  %t302 = load i32, i32* %a
  %t303 = load double, double* %d
  %t304 = fsub double -0.0, %t303
  %t305 = sitofp i32 %t302 to double
  %t306 = fmul double %t305, %t304
  %t307 = getelementptr [6 x i8], [6 x i8]* @.fmt.e, i32 0, i32 0
  %t308 = call i32 (i8*, ...) @printf(i8* %t307, double %t306)
  %t309 = getelementptr [2 x i8], [2 x i8]* @.str.4, i32 0, i32 0
  %t310 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t311 = call i32 (i8*, ...) @printf(i8* %t310, i8* %t309)
  %t312 = add i1 0, 0
  %t313 = load i1, i1* %b1
  %t314 = xor i1 %t313, 1
  %t315 = xor i1 %t312, %t314
  br i1 %t315, label %L168, label %L169
L168:
  %t316 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %t317 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t318 = call i32 (i8*, ...) @printf(i8* %t317, i8* %t316)
  br label %L170
L169:
  %t319 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %t320 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t321 = call i32 (i8*, ...) @printf(i8* %t320, i8* %t319)
  br label %L170
L170:
  %t322 = getelementptr [2 x i8], [2 x i8]* @.str.4, i32 0, i32 0
  %t323 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t324 = call i32 (i8*, ...) @printf(i8* %t323, i8* %t322)
  %t325 = load i32, i32* %args.argc
  %t326 = sub i32 %t325, 1
  %t327 = fadd double 0.0, 1.10000000000000009e+00
  %t328 = sitofp i32 %t326 to double
  %t329 = fadd double %t328, %t327
  %t330 = add i32 0, 3
  %t331 = sitofp i32 %t330 to double
  %t332 = call double @overload__double(double %t331)
  %t333 = fadd double %t329, %t332
  %t334 = add i32 0, 3
  %t335 = add i32 0, 3
  %t336 = mul i32 %t334, %t335
  %t337 = sitofp i32 %t336 to double
  %t338 = fadd double %t333, %t337
  %t339 = add i32 0, 2
  %t340 = sitofp i32 %t339 to double
  %t341 = fadd double %t338, %t340
  %t342 = add i32 0, 2
  %t343 = sitofp i32 %t342 to double
  %t344 = fadd double %t341, %t343
  %t345 = add i32 0, 2
  %t346 = sub i32 0, %t345
  %t347 = sitofp i32 %t346 to double
  %t348 = fadd double %t344, %t347
  %t349 = add i32 0, 3
  %t350 = add i32 0, 3
  %t351 = sdiv i32 %t349, %t350
  %t352 = sitofp i32 %t351 to double
  %t353 = fadd double %t348, %t352
  store double %t353, double* %d1
  %t354 = add i32 0, 0
  store i32 %t354, i32* %i
  %t355 = add i32 0, 0
  store i32 %t355, i32* %j
  %t356 = getelementptr [14 x i8], [14 x i8]* @.str.8, i32 0, i32 0
  %t357 = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0
  %t358 = call i32 (i8*, ...) @printf(i8* %t357, i8* %t356)
  br label %L171
L171:
  %t359 = load i32, i32* %i
  %t360 = add i32 0, 10
  %t361 = icmp slt i32 %t359, %t360
  br i1 %t361, label %L172, label %L173
L172:
  br label %L174
L174:
  %t362 = add i32 0, 10
  %t363 = load i32, i32* %i
  %t364 = icmp sgt i32 %t362, %t363
  br i1 %t364, label %L175, label %L176
L175:
  br label %L177
L177:
  %t365 = add i32 0, 20
  %t366 = load i32, i32* %j
  %t367 = icmp sgt i32 %t365, %t366
  br i1 %t367, label %L180, label %L181
L180:
  %t368 = load i32, i32* %i
  %t369 = add i32 0, 10
  %t370 = icmp slt i32 %t368, %t369
  br label %L182
L181:
  br label %L182
L182:
  %t371 = phi i1 [ %t370, %L180 ], [ false, %L181 ]
  br i1 %t371, label %L178, label %L179
L178:
  %t372 = load i32, i32* %i
  %t373 = add i32 0, 1
  %t374 = add i32 %t372, %t373
  store i32 %t374, i32* %i
  %t375 = load i32, i32* %j
  %t376 = add i32 0, 1
  %t377 = add i32 %t375, %t376
  store i32 %t377, i32* %j
  %t378 = load i32, i32* %i
  %t379 = add i32 0, 10
  %t380 = icmp eq i32 %t378, %t379
  br i1 %t380, label %L183, label %L184
L183:
  %t381 = load i32, i32* %j
  %t382 = add i32 0, 9
  %t383 = icmp eq i32 %t381, %t382
  br i1 %t383, label %L186, label %L187
L186:
  br label %L188
L187:
  br label %L188
L188:
  br label %L185
L184:
  br label %L185
L185:
  br label %L177
L179:
  br label %L174
L176:
  br label %L171
L173:
  ret i32 0
}

