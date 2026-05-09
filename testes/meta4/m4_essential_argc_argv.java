class m4_essential_argc_argv {
	public static void main(String[] args) {
		System.out.print(args.length);
		System.out.print("\n");
		if (args.length > 0) {
			System.out.print(Integer.parseInt(args[0]));
			System.out.print("\n");
		}
	}
}
