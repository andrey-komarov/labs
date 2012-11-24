public class AntState {
    static enum Direction {
        LEFT, RIGHT, DOWN, UP
    }

    private AppleField f;
    private int x, y;
    private Direction dir;
    private int eaten, steps;
    private int totalApples;

    public AntState(AppleField f) {
        this.f = f.clone();
        x = 0;
        y = 0;
        dir = Direction.RIGHT;
        for (int i = 0; i < f.n; i++) {
            for (int j = 0; j < f.m; j++) {
                totalApples += f.get(i, j).equals(Query.APPLE) ? 1 : 0;
            }
        }
    }

    private void advance() {
        if (dir.equals(Direction.LEFT)) {
            y = (y - 1 + f.m) % f.m;
        } else if (dir.equals(Direction.RIGHT)) {
            y = (y + 1) % f.m;
        } else if (dir.equals(Direction.DOWN)) {
            x = (x + 1) % f.n;
        } else if (dir.equals(Direction.UP)) {
            x = (x - 1 + f.n) % f.n;
        } else {
            throw new AssertionError();
        }
    }

    private void rotateLeft() {
        if (dir.equals(Direction.LEFT)) {
            dir = Direction.DOWN;
        } else if (dir.equals(Direction.DOWN)) {
            dir = Direction.RIGHT;
        } else if (dir.equals(Direction.RIGHT)) {
            dir = Direction.UP;
        } else if (dir.equals(Direction.UP)) {
            dir = Direction.LEFT;
        } else {
            throw new AssertionError();
        }
    }

    private void rotateRight() {
        rotateLeft();
        rotateLeft();
        rotateLeft();
    }

    public Query peekNext() {
        int oldX = x;
        int oldY = y;
        advance();
        Query res = f.get(x, y);
        x = oldX;
        y = oldY;
        return res;
    }

    public void go(Action a) {
        if (a.equals(Action.GO)) {
            advance();
            if (f.get(x, y).equals(Query.APPLE)) {
                f.set(x, y, Query.NO_APPLE);
                eaten++;
            }
        } else if (a.equals(Action.ROTATE_LEFT)) {
            rotateLeft();
        } else if (a.equals(Action.ROTATE_RIGHT)) {
            rotateRight();
        } else {
            throw new AssertionError();
        }
        steps++;
    }

    public Score score() {
        return new Score(eaten, steps);
    }

    public boolean finished() {
        return eaten == totalApples;
    }
}
