import java.util.Random;

public class AntAutomatonFactory {
    static Random rnd = new Random();

    static int[][] go;
    static Action[][] action;

    static AntAutomaton randomAutomaton(int n) {
//        int[][] go = new int[n][2];
//        Action[][] action = new Action[n][2];
        if (go == null) {
            go = new int[n][2];
            action = new Action[n][2];
        }
        int firstFree = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 2; j++) {
                go[i][j] = rnd.nextInt(n);
                if (go[i][j] >= firstFree) {
                    go[i][j] = firstFree;
                    firstFree++;
                }
                action[i][j] = Action.choice(rnd);
                action[i][1] = Action.GO;
            }
        }
        return new AntAutomaton(go, action);
    }
}
