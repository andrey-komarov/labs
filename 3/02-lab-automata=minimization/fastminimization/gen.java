import java.io.*;
import java.util.*;

public class gen {

	PrintWriter out;
	BufferedReader br;
	StringTokenizer st;

	String nextToken() throws IOException {
		while ((st == null) || (!st.hasMoreTokens()))
			st = new StringTokenizer(br.readLine());
		return st.nextToken();
	}

	public int nextInt() throws IOException {
		return Integer.parseInt(nextToken());
	}

	public long nextLong() throws IOException {
		return Long.parseLong(nextToken());
	}

	public double nextDouble() throws IOException {
		return Double.parseDouble(nextToken());
	}

	public void solve() throws IOException {
		Random rnd = new Random();
		int n = 50000;
		out.println(n + " " + n + " " + n / 2);
		boolean[] was = new boolean[n];
		for (int i = 0; i < n / 2; i++) {
			int x = rnd.nextInt(n);
			while (was[x]) {
				x = rnd.nextInt(n);
			}
			was[x] = true;
			out.print(x + 1 + " ");
			
		}
		out.println();
		for (int i = 0; i < n; i++) {
			out.println(i + 1 + " " + ((i + 1) % n + 1) + " " + (char)('a' + rnd.nextInt(2)));
		}
	}

	public void run() {
		try {
			br = new BufferedReader(new InputStreamReader(System.in));
			out = new PrintWriter(System.out);

			//br = new BufferedReader(new FileReader("gen.in"));
			out = new PrintWriter("fastminimization.in");

			solve();

			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		new gen().run();
	}
}
