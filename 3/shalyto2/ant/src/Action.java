import java.util.Random;

public enum Action {
    ROTATE_RIGHT, ROTATE_LEFT, GO;

    static public Action choice(Random rnd) {
        int a = rnd.nextInt(3);
        if (a == 0) {
            return ROTATE_LEFT;
        } else if (a == 1) {
            return ROTATE_RIGHT;
        } else {
            return GO;
        }
    }

    static public String print(Action a) {
        if (a.equals(ROTATE_RIGHT)) {
            return "R";
        } else if (a.equals(ROTATE_LEFT)) {
            return "L";
        } else if (a.equals(GO)) {
            return "M";
        } else {
            throw new AssertionError();
        }
    }
}
