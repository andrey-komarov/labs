import java.io.*;
import java.util.*;

public class path {

	final long INF = (Long.MAX_VALUE / 6) * 5;

	void solve() throws IOException {
		int n = nextInt();
		int m = nextInt();
		int s = nextInt() - 1;
		int[] v1 = new int[m];
		int[] v2 = new int[m];
		long[] w = new long[m];
		for (int i = 0; i < m; i++) {
			v1[i] = nextInt() - 1;
			v2[i] = nextInt() - 1;
			w[i] = nextLong();
		}
		long[] d = new long[n];
		Arrays.fill(d, INF);
		d[s] = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (d[v2[j]] != -INF && d[v1[j]] != -INF && d[v1[j]] != INF && d[v1[j]] + w[j] < d[v2[j]]) {
					d[v2[j]] = d[v1[j]] + w[j];
					if (d[v2[j]] < -INF)
						d[v2[j]] = -INF;
				}
				if (d[v1[j]] == -INF) 
					d[v2[j]] = -INF;
			}
		}
		long[] dOld = d.clone();
		for (int i = 0; i < 2 * n; i++) {
			for (int j = 0; j < m; j++) {
				if (d[v1[j]] != INF && d[v1[j]] + w[j] < d[v2[j]]) {
					d[v2[j]] = -INF;
				}
			}
		}
		for (int i = 0; i < n; i++) {
			if (d[i] == INF) {
				out.println("*");
			} else if (d[i] != dOld[i] || d[i] == -INF) {
				out.println("-");
			} else {
				out.println(d[i]);
			}
		}
	}

	final String TASKNAME = "path";

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

	long nextLong() throws IOException {
		return Long.parseLong(next());
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
		new path().run();
	}
}
