import java.util.Arrays;
import java.util.Random;

public class AntAutomaton implements Automaton<Action, Query>, Cloneable {
    private int n;
    private int[][] go;
    private Action[][] action;
    private int startState;
    private int currentState;
    private Random rnd = new Random(3);

    public AntAutomaton(int[][] go, Action[][] action) {
        this.n = go.length;
        this.go = go;
        this.action = action;
        this.startState = 0;
        currentState = startState;
    }

    @Override
    public Action go(Query q) {
        int index;
        if (q.equals(Query.APPLE)) {
            index = 1;
        } else {
            index = 0;
        }
        int newState = go[currentState][index];
        Action result = action[currentState][index];
        currentState = newState;
        return result;
    }

    @Override
    public void reset() {
        currentState = startState;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < n; i++) {
            String s = "" + (go[i][0] + 1) + " " + (go[i][1] + 1) + " " + Action.print(action[i][0]) + " " + Action.print(action[i][1]);
            sb.append(s);
            sb.append("\n");
        }
        return sb.toString();
    }

    @Override
    public AntAutomaton clone() {
        int[][] newGo = new int[go.length][];
        for (int i = 0; i < newGo.length; i++) {
            newGo[i] = go[i].clone();
        }
        Action[][] newAction = new Action[action.length][];
        for (int i = 0; i < newAction.length; i++) {
            newAction[i] = action[i].clone();
        }
        return new AntAutomaton(newGo, newAction);
    }

    public void mutate() {
        double p = 1.5 / (go.length);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 2; j++) {
                if (rnd.nextDouble() < p) {
                    go[i][j] = rnd.nextInt(n);
                }
            }
            if (rnd.nextDouble() < p) {
                action[i][0] = Action.choice(rnd);
            }
        }
    }

    @Override
    public int hashCode() {
        return Arrays.deepHashCode(go) + Arrays.deepHashCode(action);
    }
}
