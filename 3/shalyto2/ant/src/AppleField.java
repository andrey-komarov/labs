public class AppleField implements Cloneable {
    final int n, m;
    private Query[][] field;

    public AppleField(String[] f) {
        this.n = f.length;
        this.m = f[0].length();
        field = new Query[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                char ch = f[i].charAt(j);
                if (ch == '.') {
                    field[i][j] = Query.NO_APPLE;
                } else if (ch == '*') {
                    field[i][j] = Query.APPLE;
                } else {
                    throw new AssertionError();
                }
            }
        }
    }

    private AppleField(Query[][] f) {
        field = new Query[f.length][];
        for (int i = 0; i < field.length; i++) {
            field[i] = f[i].clone();
        }
        n = field.length;
        m = field[0].length;
    }

    public Query get(int x, int y) {
        return field[x][y];
    }

    public void set(int x, int y, Query q) {
        field[x][y] = q;
    }

    public int applesCnt() {
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cnt += get(i, j).equals(Query.APPLE) ? 1 : 0;
            }
        }
        return cnt;
    }

    @Override
    public AppleField clone() {
        return new AppleField(field);
    }
}
