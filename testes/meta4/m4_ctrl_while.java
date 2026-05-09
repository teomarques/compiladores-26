class m4_ctrl_while {
	public static void main(String[] args) {
		int i;
		int s;
		i = 0;
		s = 0;
		while (i < 5) {
			s = s + i;
			i = i + 1;
		}
		System.out.print(s);
		System.out.print("\n");
	}
}
