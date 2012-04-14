import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.StringTokenizer;

public class stack1 {
	static class Stack {
		final int INIT_LENGTH = 8;
		String[] a;
		int len;
		int cnt;
		
		public Stack() {
			a = new String[INIT_LENGTH];
			len = INIT_LENGTH;
		}
		
		void push(String s) {
			if (len == cnt) {
				String[] b = new String[len * 2];
				for (int i = 0; i < cnt; i++) {
					b[i] = a[i];
				}
				len *= 2;
				a = b;
			}
			a[cnt++] = s;
		}
		
		String pop() {
			if (cnt < len / 4 && cnt > INIT_LENGTH) {
				String[] b = new String[len / 2];
				for (int i = 0; i < cnt; i++) {
					b[i] = a[i];
				}
				len /= 2;
				a = b;
			}
			return a[--cnt];
		}
	}
	
	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader("stack1.in"));
		int n = Integer.parseInt(br.readLine());
		Stack s = new Stack();
		PrintWriter out = new PrintWriter("stack1.out");
		for (int i = 0; i < n; i++) {
			StringTokenizer st = new StringTokenizer(br.readLine());
			if (st.nextToken().equals("+")) {
				s.push(st.nextToken());
			} else {
				out.println(s.pop());
			}
		}
		out.close();
	}
}
