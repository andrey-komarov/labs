import java.io.*;
import java.util.*;

public class chaincode {
	public static void main(String[] args) throws IOException {
		new chaincode().run();
	}

	private String bin(int n, int len) {
		StringBuilder ans = new StringBuilder();
		for (int i = 0; i < len; i++) {
			ans.append((char) ((n & 1) + '0'));
			n >>= 1;
		}
		ans.reverse();
		return ans.toString();
	}

	public void run() throws IOException {
		// Python skills challenge failed :-(
		Scanner in = new Scanner(new File("chaincode.in"));
		PrintWriter out = new PrintWriter("chaincode.out");
		int n = in.nextInt();
		int m = (1 << n) - 1;
		HashSet<Integer> used = new HashSet<Integer>();
		int cur = 0;
		for (int i = 0; i < m + 1; i++) {
			used.add(cur);
			out.println(bin(cur, n));
			cur = (cur << 1 & m) + 1;
			if (used.contains(cur))
				cur--;
		}
		out.close();
	}
}
