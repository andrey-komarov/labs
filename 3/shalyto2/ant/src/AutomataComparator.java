import java.util.Comparator;

public class AutomataComparator implements Comparator<AntAutomaton> {
    Test t;

    public AutomataComparator(Test t) {
        this.t = t;
    }

    @Override
    public int compare(AntAutomaton a1, AntAutomaton a2) {
        a1.reset();
        a2.reset();
        Score s1 = AutomatonFieldInteractor.interact(t.field, a1, t.steps);
        Score s2 = AutomatonFieldInteractor.interact(t.field, a2, t.steps);
        int f1 = s1.fitnessFunction();
        int f2 = s2.fitnessFunction();
        if (f1 == f2) {
            return a1.hashCode() - a2.hashCode();
        }
        return s1.compareTo(s2);
    }
}
