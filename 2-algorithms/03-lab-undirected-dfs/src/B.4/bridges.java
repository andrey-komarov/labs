import java.io.*;
import java.util.*;

public class bridges {

	class Edge {
		int to, n;

		public Edge(int to, int n) {
			this.to = to;
			this.n = n;
		}

	}

	ArrayList<Edge>[] g;
	boolean[] used;
	int[] up, t;
	int time = 0;

	ArrayList<Edge> ans;

	void dfs(int v, int p) {
		used[v] = true;
		time++;
		t[v] = up[v] = time;
		for (Edge e : g[v]) {
			if (e.n == p)
				continue;
			if (used[e.to]) {
				up[v] = Math.min(up[v], t[e.to]);
			} else {
				dfs(e.to, e.n);
				up[v] = Math.min(up[v], up[e.to]);
				if (up[e.to] > t[v])
					ans.add(e);
			}

		}
	}

	void solve() throws IOException {
		int n = nextInt();
		int m = nextInt();
		g = new ArrayList[n];
		used = new boolean[n];
		up = new int[n];
		t = new int[n];
		ans = new ArrayList<Edge>();
		for (int i = 0; i < n; i++) {
			g[i] = new ArrayList<Edge>();
		}
		for (int i = 0; i < m; i++) {
			int v1 = nextInt() - 1;
			int v2 = nextInt() - 1;
			g[v1].add(new Edge(v2, i + 1));
			g[v2].add(new Edge(v1, i + 1));
		}
		for (int i = 0; i < n; i++) {
			if (!used[i])
				dfs(i, -1);
		}
		out.println(ans.size());
		ArrayList<Integer> ans2 = new ArrayList<Integer>();
		for (Edge e : ans) {
			ans2.add(e.n);			
		}
		Collections.sort(ans2);
		for (int i : ans2) {
			out.print(i + " ");
		}
	}

	final String TASKNAME = "bridges";

	PrintWriter out;
	BufferedReader br;
	StringTokenizer st;

	void run() throws IOException {
		br = new BufferedReader(new FileReader(new File(TASKNAME + ".in")));
		out = new PrintWriter(TASKNAME + ".out");
		solve();
		out.close();
	}

	int nextInt() throws IOException {
		return Integer.parseInt(next());
	}

	double nextDouble() throws IOException {
		return Double.parseDouble(next());
	}

	String next() throws IOException {
		while (st == null || !st.hasMoreTokens()) {
			st = new StringTokenizer(br.readLine());
		}
		return st.nextToken();
	}

	public static void main(String[] args) throws IOException {
		new bridges().run();
	}
}
