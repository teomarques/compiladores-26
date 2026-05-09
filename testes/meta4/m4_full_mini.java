class m4_full_mini {
	public static int acc;
	public static int bump(int d) {
		acc = acc + d;
		return acc;
	}
	public static void main(String[] args) {
		int i;
		acc = 0;
		i = 0;
		while (i < 4) {
			if ((i % 2) == 0) {
				bump(i * 2);
			} else {
				bump(-1);
			}
			i = i + 1;
		}
		System.out.print(acc);
		System.out.print("\n");
		System.out.print((true || false) && !(false ^ false));
		System.out.print("\n");
	}
}
