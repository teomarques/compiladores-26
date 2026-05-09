class m4_methods_return {
	public static int pick(boolean flag, int a, int b) {
		if (flag) {
			return a;
		} else {
			return b;
		}
	}
	public static void main(String[] args) {
		System.out.print(pick(true, 11, 22));
		System.out.print("\n");
		System.out.print(pick(false, 11, 22));
		System.out.print("\n");
	}
}
