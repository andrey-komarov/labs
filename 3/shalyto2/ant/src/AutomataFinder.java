public class AutomataFinder {
    Test t;

    public AutomataFinder(Test t) {
        this.t = t;
    }

    AntAutomaton findUsingRandomAutomata(int iterations) {
        Score bestScore = null;
        AntAutomaton bestAutomaton = null;
        int mutationsRemain = 0;
        AntAutomaton aut;
        for (int i = 0; i < iterations; i++) {
            if (mutationsRemain == 0) {
                aut = AntAutomatonFactory.randomAutomaton(t.states);
            } else {
                aut = bestAutomaton.clone();
                aut.mutate();
                mutationsRemain--;
                if (mutationsRemain == 0) {
                    System.err.println("mutations finished... Returning to random");
                }
            }
            Score score = AutomatonFieldInteractor.interact(t.field, aut, t.steps);
            if (bestScore == null || score.compareTo(bestScore) > 0) {
                bestAutomaton = aut.clone();
                bestScore = score;
                System.err.println("New highscore at iteration " + i + " total apples : " + t.field.applesCnt());
                System.err.println(score);
                System.err.println(aut);
                mutationsRemain = i;
            }
        }
        bestAutomaton.reset();
        return bestAutomaton;
    }
}
