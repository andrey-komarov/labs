public class AutomatonFieldInteractor {
    static Score interact(AppleField field, AntAutomaton ant, int maxTurns) {
        ant.reset();
        int n = field.n;
        int m = field.m;
        int poxX = 0;
        int posY = 0;
        AntState state = new AntState(field);
        Score score = null;
        for (int i = 0; i < maxTurns; i++) {
            Query q = state.peekNext();
            Action a = ant.go(q);
            state.go(a);
            if (state.finished()) {
                score = state.score();
                break;
            }
        }
        if (score == null) {
            score = state.score();
        }
        return score;
    }
}
