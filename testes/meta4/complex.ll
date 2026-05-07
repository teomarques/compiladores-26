@.str.8 = private unnamed_addr constant [14 x i8] c"Before while\0A\00"
@.str.7 = private unnamed_addr constant [11 x i8] c"After ifs\0A\00"
@.str.6 = private unnamed_addr constant [11 x i8] c"Before if\0A\00"
@.str.5 = private unnamed_addr constant [5 x i8] c"\0Aa:\0A\00"
@.str.4 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str.3 = private unnamed_addr constant [19 x i8] c"I'm the fake main\0A\00"
@.str.2 = private unnamed_addr constant [16 x i8] c"I have one int\0A\00"
@.str.1 = private unnamed_addr constant [19 x i8] c"I have one double\0A\00"
@.str.0 = private unnamed_addr constant [20 x i8] c"I have two doubles\0A\00"
@.str.true = private unnamed_addr constant [5 x i8] c"true\00"
@.str.false = private unnamed_addr constant [6 x i8] c"false\00"
@.str.fmt.int = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.fmt.double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.str.fmt.str = private unnamed_addr constant [3 x i8] c"%s\00"
declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@_Main_overload = global i32 0
@argc = global i32 0
@argv = global i8** null

define i1 @_Main_overload_boolean(i1 %param_0) {
  %1 = alloca i1
  store i1 %param_0, i1* %1
  %2 = getelementptr [20 x i8], [20 x i8]* @.str.0, i32 0, i32 0
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i8* %2)
  %5 = add i1 0, 0
  ret i1 %5
L1:
  ret i1 0
}

define double @_Main_overload_double(double %param_0) {
  %1 = alloca double
  store double %param_0, double* %1
  %2 = getelementptr [19 x i8], [19 x i8]* @.str.1, i32 0, i32 0
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i8* %2)
  %5 = fadd double 2.200000e+00, 0.0
  %6 = add i32 2, 0
  %7 = sitofp i32 %6 to double
  %8 = fmul double %5, %7
  %9 = add i32 2, 0
  %10 = call i32 @_Main_main_int(i32 %9)
  %11 = add i32 5, 0
  %12 = sdiv i32 %10, %11
  %13 = add i32 6, 0
  %14 = mul i32 %12, %13
  %15 = sitofp i32 %14 to double
  %16 = fadd double %8, %15
  %17 = add i32 2, 0
  %18 = add i32 3, 0
  %19 = mul i32 %17, %18
  %20 = sitofp i32 %19 to double
  %21 = fadd double %16, %20
  %22 = add i32 3, 0
  %23 = add i32 4, 0
  %24 = mul i32 %22, %23
  %25 = sitofp i32 %24 to double
  %26 = fadd double %21, %25
  %27 = add i32 2, 0
  %28 = add i32 8, 0
  %29 = mul i32 %27, %28
  %30 = sitofp i32 %29 to double
  %31 = fadd double %26, %30
  %32 = add i32 5, 0
  %33 = add i32 9, 0
  %34 = mul i32 %32, %33
  %35 = sitofp i32 %34 to double
  %36 = fadd double %31, %35
  %37 = add i32 2, 0
  %38 = add i32 2, 0
  %39 = call i32 @_Main_overload_int_int(i32 %37, i32 %38)
  %40 = sitofp i32 %39 to double
  %41 = fadd double %36, %40
  ret double %41
L1:
  ret double 0.0
}

define i32 @_Main_overload_double_double(double %param_0, double %param_1) {
  %1 = alloca double
  store double %param_0, double* %1
  %2 = alloca double
  store double %param_1, double* %2
  %3 = add i32 2, 0
  %4 = load i32, i32* @_Main_overload
  %5 = add i32 %3, %4
  store i32 %5, i32* @_Main_overload
  %6 = alloca double
  %7 = getelementptr [16 x i8], [16 x i8]* @.str.2, i32 0, i32 0
  %8 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %9 = call i32 (i8*, ...) @printf(i8* %8, i8* %7)
  %10 = add i32 5, 0
  ret i32 %10
L1:
  ret i32 0
}

define i32 @_Main_overload_int_int(i32 %param_0, i32 %param_1) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = alloca i32
  store i32 %param_1, i32* %2
  %3 = add i32 5, 0
  ret i32 %3
L1:
  ret i32 0
}

define i32 @_Main_main_int(i32 %param_0) {
  %1 = alloca i32
  store i32 %param_0, i32* %1
  %2 = getelementptr [19 x i8], [19 x i8]* @.str.3, i32 0, i32 0
  %3 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %4 = call i32 (i8*, ...) @printf(i8* %3, i8* %2)
  %5 = add i32 5, 0
  ret i32 %5
L1:
  ret i32 0
}

define void @_Main_main_StringArray() {
  %1 = load i32, i32* @argc
  %2 = sub i32 %1, 1
  %3 = add i32 0, 0
  %4 = icmp sgt i32 %2, %3
  br i1 %4, label %L1, label %L2
L1:
  %5 = add i32 0, 0
  %6 = add i32 %5, 1
  %7 = load i8**, i8*** @argv
  %8 = getelementptr i8*, i8** %7, i32 %6
  %9 = load i8*, i8** %8
  %10 = call i32 @atoi(i8* %9)
  %11 = sitofp i32 %10 to double
  %12 = call double @_Main_overload_double(double %11)
  br label %L3
L2:
  %13 = add i32 5, 0
  %14 = sitofp i32 %13 to double
  %15 = call double @_Main_overload_double(double %14)
  br label %L3
L3:
  %16 = add i1 0, 0
  %17 = xor i1 %16, 1
  %18 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %19 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %20 = select i1 %17, i8* %18, i8* %19
  %21 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %22 = call i32 (i8*, ...) @printf(i8* %21, i8* %20)
  %23 = getelementptr [2 x i8], [2 x i8]* @.str.4, i32 0, i32 0
  %24 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %25 = call i32 (i8*, ...) @printf(i8* %24, i8* %23)
  %26 = alloca i32
  %27 = add i32 2222222, 0
  store i32 %27, i32* %26
  %28 = alloca double
  %29 = fadd double 2.323233e+08, 0.0
  store double %29, double* %28
  %30 = alloca i32
  %31 = add i32 3, 0
  store i32 %31, i32* %30
  %32 = load i32, i32* %26
  %33 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %34 = call i32 (i8*, ...) @printf(i8* %33, i32 %32)
  %35 = alloca i1
  %36 = add i1 1, 0
  store i1 %36, i1* %35
  %37 = load i32, i32* %26
  %38 = load i32, i32* %26
  %39 = add i32 %37, %38
  %40 = load i32, i32* %26
  %41 = add i32 %39, %40
  %42 = add i32 8, 0
  %43 = add i32 2, 0
  %44 = srem i32 %42, %43
  %45 = add i32 %41, %44
  %46 = load double, double* %28
  %47 = add i32 3, 0
  %48 = sitofp i32 %47 to double
  %49 = fdiv double %46, %48
  %50 = sitofp i32 %45 to double
  %51 = fadd double %50, %49
  %52 = load i32, i32* %26
  %53 = load double, double* %28
  %54 = sitofp i32 %52 to double
  %55 = fmul double %54, %53
  %56 = fadd double %51, %55
  %57 = load double, double* %28
  %58 = fadd double %56, %57
  %59 = load i32, i32* %26
  %60 = sitofp i32 %59 to double
  %61 = fsub double %58, %60
  %62 = load i32, i32* %26
  %63 = load i32, i32* %30
  %64 = mul i32 %62, %63
  %65 = load double, double* %28
  %66 = sitofp i32 %64 to double
  %67 = fdiv double %66, %65
  %68 = load i32, i32* %26
  %69 = load i32, i32* %30
  %70 = mul i32 %68, %69
  %71 = load double, double* %28
  %72 = sitofp i32 %70 to double
  %73 = fdiv double %72, %71
  %74 = fmul double %67, %73
  %75 = fadd double %61, %74
  %76 = fadd double 2.200000e+00, 0.0
  %77 = call double @_Main_overload_double(double %76)
  %78 = fadd double %75, %77
  %79 = add i32 1, 0
  %80 = add i32 1, 0
  %81 = call i32 @_Main_overload_int_int(i32 %79, i32 %80)
  %82 = sitofp i32 %81 to double
  store double %82, double* %28
  %83 = fadd double %78, %82
  %84 = add i32 5, 0
  %85 = call i32 @_Main_main_int(i32 %84)
  %86 = sitofp i32 %85 to double
  %87 = fadd double %83, %86
  %88 = load i32, i32* @argc
  %89 = sub i32 %88, 1
  %90 = add i32 1, 0
  %91 = sub i32 %89, %90
  %92 = add i32 %91, 1
  %93 = load i8**, i8*** @argv
  %94 = getelementptr i8*, i8** %93, i32 %92
  %95 = load i8*, i8** %94
  %96 = call i32 @atoi(i8* %95)
  %97 = sitofp i32 %96 to double
  %98 = fadd double %87, %97
  %99 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %100 = call i32 (i8*, ...) @printf(i8* %99, double %98)
  %101 = getelementptr [5 x i8], [5 x i8]* @.str.5, i32 0, i32 0
  %102 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %103 = call i32 (i8*, ...) @printf(i8* %102, i8* %101)
  %104 = load i32, i32* %26
  %105 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0
  %106 = call i32 (i8*, ...) @printf(i8* %105, i32 %104)
  %107 = getelementptr [11 x i8], [11 x i8]* @.str.6, i32 0, i32 0
  %108 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %109 = call i32 (i8*, ...) @printf(i8* %108, i8* %107)
  %110 = load i32, i32* %26
  %111 = add i32 2, 0
  %112 = add i32 2, 0
  %113 = mul i32 %111, %112
  %114 = add i32 3, 0
  %115 = add i32 5, 0
  %116 = sdiv i32 %114, %115
  %117 = add i32 %113, %116
  %118 = add i32 5, 0
  %119 = call i32 @_Main_main_int(i32 %118)
  %120 = add i32 %117, %119
  %121 = icmp sge i32 %110, %120
  br i1 %121, label %L4, label %L5
L4:
  %122 = load i32, i32* %26
  %123 = add i32 2, 0
  %124 = icmp sle i32 %122, %123
  br i1 %124, label %L7, label %L8
L7:
  %125 = add i32 2, 0
  %126 = load i32, i32* %26
  %127 = icmp sge i32 %125, %126
  br i1 %127, label %L10, label %L11
L10:
  %128 = add i32 2, 0
  %129 = load i32, i32* %26
  %130 = icmp sle i32 %128, %129
  br i1 %130, label %L13, label %L14
L13:
  %131 = load i32, i32* %26
  %132 = add i32 2, 0
  %133 = icmp eq i32 %131, %132
  br i1 %133, label %L16, label %L17
L16:
  %134 = add i32 2, 0
  %135 = load i32, i32* %26
  %136 = icmp eq i32 %134, %135
  br i1 %136, label %L19, label %L20
L19:
  %137 = load i32, i32* %26
  %138 = add i32 2, 0
  %139 = icmp sgt i32 %137, %138
  br i1 %139, label %L22, label %L23
L22:
  %140 = load i32, i32* %26
  %141 = add i32 2, 0
  %142 = icmp slt i32 %140, %141
  br i1 %142, label %L25, label %L26
L25:
  %143 = add i32 2, 0
  %144 = load i32, i32* %26
  %145 = icmp sgt i32 %143, %144
  br i1 %145, label %L28, label %L29
L28:
  %146 = add i32 2, 0
  %147 = load i32, i32* %26
  %148 = icmp slt i32 %146, %147
  br i1 %148, label %L31, label %L32
L31:
  %149 = load i32, i32* %26
  %150 = add i32 2, 0
  %151 = icmp ne i32 %149, %150
  br i1 %151, label %L34, label %L35
L34:
  %152 = add i32 2, 0
  %153 = load i32, i32* %26
  %154 = icmp ne i32 %152, %153
  br i1 %154, label %L37, label %L38
L37:
  br label %L39
L38:
  br label %L39
L39:
  br label %L36
L35:
  br label %L36
L36:
  br label %L33
L32:
  br label %L33
L33:
  br label %L30
L29:
  br label %L30
L30:
  br label %L27
L26:
  br label %L27
L27:
  br label %L24
L23:
  br label %L24
L24:
  br label %L21
L20:
  br label %L21
L21:
  br label %L18
L17:
  br label %L18
L18:
  br label %L15
L14:
  br label %L15
L15:
  br label %L12
L11:
  br label %L12
L12:
  br label %L9
L8:
  br label %L9
L9:
  br label %L6
L5:
  br label %L6
L6:
  %155 = getelementptr [11 x i8], [11 x i8]* @.str.7, i32 0, i32 0
  %156 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %157 = call i32 (i8*, ...) @printf(i8* %156, i8* %155)
  %158 = load double, double* %28
  %159 = add i32 2, 0
  %160 = sitofp i32 %159 to double
  %161 = fcmp oge double %158, %160
  br i1 %161, label %L40, label %L41
L40:
  %162 = load double, double* %28
  %163 = add i32 2, 0
  %164 = sitofp i32 %163 to double
  %165 = fcmp ole double %162, %164
  br i1 %165, label %L43, label %L44
L43:
  %166 = add i32 2, 0
  %167 = load double, double* %28
  %168 = sitofp i32 %166 to double
  %169 = fcmp oge double %168, %167
  br i1 %169, label %L46, label %L47
L46:
  %170 = add i32 2, 0
  %171 = load double, double* %28
  %172 = sitofp i32 %170 to double
  %173 = fcmp ole double %172, %171
  br i1 %173, label %L49, label %L50
L49:
  %174 = load double, double* %28
  %175 = add i32 2, 0
  %176 = sitofp i32 %175 to double
  %177 = fcmp oeq double %174, %176
  br i1 %177, label %L52, label %L53
L52:
  %178 = add i32 2, 0
  %179 = load double, double* %28
  %180 = sitofp i32 %178 to double
  %181 = fcmp oeq double %180, %179
  br i1 %181, label %L55, label %L56
L55:
  %182 = load double, double* %28
  %183 = add i32 2, 0
  %184 = sitofp i32 %183 to double
  %185 = fcmp ogt double %182, %184
  br i1 %185, label %L58, label %L59
L58:
  %186 = load double, double* %28
  %187 = add i32 2, 0
  %188 = sitofp i32 %187 to double
  %189 = fcmp olt double %186, %188
  br i1 %189, label %L61, label %L62
L61:
  %190 = add i32 2, 0
  %191 = load double, double* %28
  %192 = sitofp i32 %190 to double
  %193 = fcmp ogt double %192, %191
  br i1 %193, label %L64, label %L65
L64:
  %194 = add i32 2, 0
  %195 = load double, double* %28
  %196 = sitofp i32 %194 to double
  %197 = fcmp olt double %196, %195
  br i1 %197, label %L67, label %L68
L67:
  %198 = load double, double* %28
  %199 = add i32 2, 0
  %200 = sitofp i32 %199 to double
  %201 = fcmp one double %198, %200
  br i1 %201, label %L70, label %L71
L70:
  %202 = add i32 2, 0
  %203 = load double, double* %28
  %204 = sitofp i32 %202 to double
  %205 = fcmp one double %204, %203
  br i1 %205, label %L73, label %L74
L73:
  br label %L75
L74:
  br label %L75
L75:
  br label %L72
L71:
  br label %L72
L72:
  br label %L69
L68:
  br label %L69
L69:
  br label %L66
L65:
  br label %L66
L66:
  br label %L63
L62:
  br label %L63
L63:
  br label %L60
L59:
  br label %L60
L60:
  br label %L57
L56:
  br label %L57
L57:
  br label %L54
L53:
  br label %L54
L54:
  br label %L51
L50:
  br label %L51
L51:
  br label %L48
L47:
  br label %L48
L48:
  br label %L45
L44:
  br label %L45
L45:
  br label %L42
L41:
  br label %L42
L42:
  %206 = load double, double* %28
  %207 = fadd double 2.200000e+00, 0.0
  %208 = fcmp oge double %206, %207
  br i1 %208, label %L76, label %L77
L76:
  %209 = load double, double* %28
  %210 = fadd double 2.200000e+00, 0.0
  %211 = fcmp ole double %209, %210
  br i1 %211, label %L79, label %L80
L79:
  %212 = fadd double 2.200000e+00, 0.0
  %213 = load double, double* %28
  %214 = fcmp oge double %212, %213
  br i1 %214, label %L82, label %L83
L82:
  %215 = fadd double 2.200000e+00, 0.0
  %216 = load double, double* %28
  %217 = fcmp ole double %215, %216
  br i1 %217, label %L85, label %L86
L85:
  %218 = load double, double* %28
  %219 = fadd double 2.200000e+00, 0.0
  %220 = fcmp oeq double %218, %219
  br i1 %220, label %L88, label %L89
L88:
  %221 = fadd double 2.200000e+00, 0.0
  %222 = load double, double* %28
  %223 = fcmp oeq double %221, %222
  br i1 %223, label %L91, label %L92
L91:
  %224 = load double, double* %28
  %225 = fadd double 2.200000e+00, 0.0
  %226 = fcmp ogt double %224, %225
  br i1 %226, label %L94, label %L95
L94:
  %227 = load double, double* %28
  %228 = fadd double 2.200000e+00, 0.0
  %229 = fcmp olt double %227, %228
  br i1 %229, label %L97, label %L98
L97:
  %230 = fadd double 2.200000e+00, 0.0
  %231 = load double, double* %28
  %232 = fcmp ogt double %230, %231
  br i1 %232, label %L100, label %L101
L100:
  %233 = fadd double 2.200000e+00, 0.0
  %234 = load double, double* %28
  %235 = fcmp olt double %233, %234
  br i1 %235, label %L103, label %L104
L103:
  %236 = load double, double* %28
  %237 = fadd double 2.200000e+00, 0.0
  %238 = fcmp one double %236, %237
  br i1 %238, label %L106, label %L107
L106:
  %239 = fadd double 2.200000e+00, 0.0
  %240 = load double, double* %28
  %241 = fcmp one double %239, %240
  br i1 %241, label %L109, label %L110
L109:
  br label %L111
L110:
  br label %L111
L111:
  br label %L108
L107:
  br label %L108
L108:
  br label %L105
L104:
  br label %L105
L105:
  br label %L102
L101:
  br label %L102
L102:
  br label %L99
L98:
  br label %L99
L99:
  br label %L96
L95:
  br label %L96
L96:
  br label %L93
L92:
  br label %L93
L93:
  br label %L90
L89:
  br label %L90
L90:
  br label %L87
L86:
  br label %L87
L87:
  br label %L84
L83:
  br label %L84
L84:
  br label %L81
L80:
  br label %L81
L81:
  br label %L78
L77:
  br label %L78
L78:
  %242 = load i1, i1* %35
  %243 = add i1 1, 0
  %244 = icmp eq i1 %242, %243
  br label %L120
L120:
  br i1 %244, label %L118, label %L119
L118:
  %245 = load double, double* %28
  %246 = fadd double 2.200000e+00, 0.0
  %247 = fcmp oge double %245, %246
  br label %L121
L121:
  br label %L119
L119:
  %248 = phi i1 [ %244, %L120 ], [ %247, %L121 ]
  br label %L122
L122:
  br i1 %248, label %L116, label %L117
L116:
  %249 = add i32 2, 0
  %250 = load double, double* %28
  %251 = sitofp i32 %249 to double
  %252 = fcmp oge double %251, %250
  br label %L123
L123:
  br label %L117
L117:
  %253 = phi i1 [ %248, %L122 ], [ %252, %L123 ]
  br label %L124
L124:
  br i1 %253, label %L114, label %L115
L114:
  %254 = fadd double 2.200000e+00, 0.0
  %255 = load double, double* %28
  %256 = fcmp ole double %254, %255
  br label %L125
L125:
  br label %L115
L115:
  %257 = phi i1 [ %253, %L124 ], [ %256, %L125 ]
  br label %L126
L126:
  br i1 %257, label %L113, label %L112
L112:
  %258 = fadd double 2.200000e+00, 0.0
  %259 = load double, double* %28
  %260 = fadd double 2.300000e+00, 0.0
  %261 = add i32 2, 0
  store i32 %261, i32* %26
  %262 = sitofp i32 %261 to double
  %263 = fadd double %260, %262
  %264 = add i32 2, 0
  %265 = sitofp i32 %264 to double
  store double %265, double* %28
  %266 = fadd double %263, %265
  store double %266, double* %28
  %267 = fadd double %259, %266
  %268 = fcmp oeq double %258, %267
  br label %L127
L127:
  br label %L113
L113:
  %269 = phi i1 [ %257, %L126 ], [ %268, %L127 ]
  br i1 %269, label %L128, label %L129
L128:
  %270 = add i1 1, 0
  %271 = load i1, i1* %35
  %272 = icmp eq i1 %270, %271
  br i1 %272, label %L131, label %L132
L131:
  %273 = load i1, i1* %35
  %274 = add i1 1, 0
  %275 = icmp ne i1 %273, %274
  br i1 %275, label %L134, label %L135
L134:
  %276 = add i1 1, 0
  %277 = load i1, i1* %35
  %278 = icmp ne i1 %276, %277
  br i1 %278, label %L137, label %L138
L137:
  br label %L139
L138:
  br label %L139
L139:
  br label %L136
L135:
  br label %L136
L136:
  br label %L133
L132:
  br label %L133
L133:
  br label %L130
L129:
  br label %L130
L130:
  %279 = load i1, i1* %35
  br label %L146
L146:
  br i1 %279, label %L144, label %L145
L144:
  %280 = add i1 1, 0
  br label %L147
L147:
  br label %L145
L145:
  %281 = phi i1 [ %279, %L146 ], [ %280, %L147 ]
  br label %L148
L148:
  br i1 %281, label %L143, label %L142
L142:
  %282 = load i1, i1* %35
  br label %L149
L149:
  br label %L143
L143:
  %283 = phi i1 [ %281, %L148 ], [ %282, %L149 ]
  br label %L150
L150:
  br i1 %283, label %L141, label %L140
L140:
  %284 = add i1 1, 0
  br label %L153
L153:
  br i1 %284, label %L151, label %L152
L151:
  br label %L154
L154:
  br label %L152
L152:
  %285 = phi i1 [ %284, %L153 ], [ %-1, %L154 ]
  br label %L155
L155:
  br label %L141
L141:
  %286 = phi i1 [ %283, %L150 ], [ %285, %L155 ]
  br i1 %286, label %L156, label %L157
L156:
  %287 = add i1 1, 0
  br label %L165
L165:
  br i1 %287, label %L163, label %L164
L163:
  %288 = load i1, i1* %35
  br label %L166
L166:
  br label %L164
L164:
  %289 = phi i1 [ %287, %L165 ], [ %288, %L166 ]
  br label %L167
L167:
  br i1 %289, label %L162, label %L161
L161:
  %290 = add i1 1, 0
  br label %L168
L168:
  br label %L162
L162:
  %291 = phi i1 [ %289, %L167 ], [ %290, %L168 ]
  br label %L169
L169:
  br i1 %291, label %L160, label %L159
L159:
  %292 = load i1, i1* %35
  br label %L172
L172:
  br i1 %292, label %L170, label %L171
L170:
  br label %L173
L173:
  br label %L171
L171:
  %293 = phi i1 [ %292, %L172 ], [ %-1, %L173 ]
  br label %L174
L174:
  br label %L160
L160:
  %294 = phi i1 [ %291, %L169 ], [ %293, %L174 ]
  br i1 %294, label %L175, label %L176
L175:
  br label %L177
L176:
  br label %L177
L177:
  br label %L158
L157:
  br label %L158
L158:
  %295 = getelementptr [11 x i8], [11 x i8]* @.str.7, i32 0, i32 0
  %296 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %297 = call i32 (i8*, ...) @printf(i8* %296, i8* %295)
  %298 = load i32, i32* %26
  %299 = load double, double* %28
  %300 = fsub double 0.0, %299
  %301 = sitofp i32 %298 to double
  %302 = fmul double %301, %300
  %303 = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0
  %304 = call i32 (i8*, ...) @printf(i8* %303, double %302)
  %305 = getelementptr [2 x i8], [2 x i8]* @.str.4, i32 0, i32 0
  %306 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %307 = call i32 (i8*, ...) @printf(i8* %306, i8* %305)
  %308 = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0
  %309 = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0
  %310 = select i1 %-1, i8* %308, i8* %309
  %311 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %312 = call i32 (i8*, ...) @printf(i8* %311, i8* %310)
  %313 = getelementptr [2 x i8], [2 x i8]* @.str.4, i32 0, i32 0
  %314 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %315 = call i32 (i8*, ...) @printf(i8* %314, i8* %313)
  %316 = alloca double
  %317 = load i32, i32* @argc
  %318 = sub i32 %317, 1
  %319 = fadd double 1.100000e+00, 0.0
  %320 = sitofp i32 %318 to double
  %321 = fadd double %320, %319
  %322 = add i32 3, 0
  %323 = sitofp i32 %322 to double
  %324 = call double @_Main_overload_double(double %323)
  %325 = fadd double %321, %324
  %326 = add i32 3, 0
  %327 = add i32 3, 0
  %328 = mul i32 %326, %327
  %329 = sitofp i32 %328 to double
  %330 = fadd double %325, %329
  %331 = add i32 2, 0
  %332 = sitofp i32 %331 to double
  %333 = fadd double %330, %332
  %334 = add i32 2, 0
  %335 = sitofp i32 %334 to double
  %336 = fadd double %333, %335
  %337 = add i32 2, 0
  %338 = sub i32 0, %337
  %339 = sitofp i32 %338 to double
  %340 = fadd double %336, %339
  %341 = add i32 3, 0
  %342 = add i32 3, 0
  %343 = sdiv i32 %341, %342
  %344 = sitofp i32 %343 to double
  %345 = fadd double %340, %344
  store double %345, double* %316
  %346 = alloca i32
  %347 = add i32 0, 0
  store i32 %347, i32* %346
  %348 = alloca i32
  %349 = add i32 0, 0
  store i32 %349, i32* %348
  %350 = getelementptr [14 x i8], [14 x i8]* @.str.8, i32 0, i32 0
  %351 = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0
  %352 = call i32 (i8*, ...) @printf(i8* %351, i8* %350)
  br label %L178
L178:
  %353 = load i32, i32* %346
  %354 = add i32 10, 0
  %355 = icmp slt i32 %353, %354
  br i1 %355, label %L179, label %L180
L179:
  br label %L181
L181:
  %356 = add i32 10, 0
  %357 = load i32, i32* %346
  %358 = icmp sgt i32 %356, %357
  br i1 %358, label %L182, label %L183
L182:
  br label %L184
L184:
  %359 = add i32 20, 0
  %360 = load i32, i32* %348
  %361 = icmp sgt i32 %359, %360
  br label %L189
L189:
  br i1 %361, label %L187, label %L188
L187:
  %362 = load i32, i32* %346
  %363 = add i32 10, 0
  %364 = icmp slt i32 %362, %363
  br label %L190
L190:
  br label %L188
L188:
  %365 = phi i1 [ %361, %L189 ], [ %364, %L190 ]
  br i1 %365, label %L185, label %L186
L185:
  %366 = load i32, i32* %346
  %367 = add i32 1, 0
  %368 = add i32 %366, %367
  store i32 %368, i32* %346
  %369 = load i32, i32* %348
  %370 = add i32 1, 0
  %371 = add i32 %369, %370
  store i32 %371, i32* %348
  %372 = load i32, i32* %346
  %373 = add i32 10, 0
  %374 = icmp eq i32 %372, %373
  br i1 %374, label %L191, label %L192
L191:
  %375 = load i32, i32* %348
  %376 = add i32 9, 0
  %377 = icmp eq i32 %375, %376
  br i1 %377, label %L194, label %L195
L194:
  br label %L196
L195:
  br label %L196
L196:
  br label %L193
L192:
  br label %L193
L193:
  br label %L184
L186:
  br label %L181
L183:
  br label %L178
L180:
  ret void
}

define i32 @main(i32 %argc, i8** %argv) {
  store i32 %argc, i32* @argc
  store i8** %argv, i8*** @argv
  call void @_Main_main_StringArray()
  ret i32 0
}
