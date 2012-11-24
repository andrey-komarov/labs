import java.io.*;
import java.util.*;

public class cf {

	private static final long MOD = (long) (1e9 + 7);
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

	class Rule {
		int where, first, second;

		public Rule(int where, int first, int second) {
			super();
			this.where = where;
			this.first = first;
			this.second = second;
		}

	}

	class PoorRule {
		int where, first;
	}

	public void solve() throws IOException {
		int n = nextInt();
		int s = nextToken().charAt(0) - 'A';
		ArrayList<Integer>[] start = new ArrayList[100];
		for (int i = 0; i < 100; i++) {
			start[i] = new ArrayList<Integer>();
		}

		ArrayList<Rule> rules = new ArrayList<Rule>();
		ArrayList<PoorRule> poorRules = new ArrayList<PoorRule>();
		HashSet<Integer> empty = new HashSet<Integer>();

		int cnt = 100;

		for (int i = 0; i < n; i++) {
			int w = nextToken().charAt(0) - 'A';
			nextToken();
			if (!st.hasMoreTokens()) {
				empty.add(w);
				continue;
			}
			String t = nextToken();
			if (t.length() == 1) {
				start[t.charAt(0) - 'A'].add(w);
				continue;
			}
			while (t.length() > 2) {

				rules.add(new Rule(w, cnt, t.charAt(t.length() - 1) - 'A'));
				t = t.substring(0, t.length() - 1);
				w = cnt;
				cnt++;
			}
			rules.add(new Rule(w, t.charAt(0) - 'A', t.charAt(1) - 'A'));
		}

		String t = br.readLine();
		for (int i = 0; i < cnt * 2; i++) {
			HashSet<Integer> add = new HashSet<Integer>();
			for (int x : empty) {
				if (x < start.length)
					add.addAll(start[x]);
			}
			empty.addAll(add);
			for (Rule z : rules) {
				boolean can = true;
				if (empty.contains(z.first) && empty.contains(z.second))
					empty.add(z.where);
			}
		}

		if (t.equals("")) {
			if (empty.contains(s)) {
				out.println("yes");
			} else {
				out.println("no");
			}
			return;
		}

		boolean[][][] dp = new boolean[t.length()][t.length()][cnt];
		for (int i = 0; i < t.length(); i++) {
			for (int x : start[t.charAt(i) - 'A']) {
				dp[i][i][x] = true;
			}
			dp[i][i][t.charAt(i) - 'A'] = true;
		}

		for (int l = 1; l < t.length() + 1; l++)
			for (int i = 0; i < t.length() - l + 1; i++) {
				for (int j = i; j < i + l - 1; j++) {
					for (Rule z : rules) {
						// System.err.println(i + " " + l);
						if (dp[i][j][z.first] && dp[j + 1][i + l - 1][z.second]) {
							dp[i][i + l - 1][z.where] = true;
						}
					}

				}
				for (int zz = 0; zz < 10; zz++) {
					for (Rule z : rules) {
						// System.err.println(i + " " + l);
						if (dp[i][i + l - 1][z.first]
								&& (empty.contains(z.second))
								|| dp[i][i + l - 1][z.second]
								&& empty.contains(z.first)) {
							dp[i][i + l - 1][z.where] = true;
						}
					}
					for (int k = 0; k < 100; k++) {
						if (dp[i][i + l - 1][k] == true) {
							for (int x : start[k]) {
								dp[i][i + l - 1][x] = true;
							}
						}
					}
				}
			}

		if (dp[0][t.length() - 1][s])
			out.println("yes");
		else
			out.println("no");

	}

	public void run() {
		try {
			br = new BufferedReader(new InputStreamReader(System.in));
			out = new PrintWriter(System.out);

			br = new BufferedReader(new FileReader("cf.in"));
			out = new PrintWriter("cf.out");

			solve();

			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		new cf().run();
	}
}
