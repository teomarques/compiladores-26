class m4_full_argv_sum {
	public static void main(String[] args) {
		int i;
		int s;
		i = 0;
		s = 0;
		while (i < args.length) {
			s = s + Integer.parseInt(args[i]);
			i = i + 1;
		}
		System.out.print(s);
		System.out.print("\n");
	}
}
