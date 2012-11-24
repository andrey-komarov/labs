import java.util.Comparator;
import java.util.Random;
import java.util.SortedSet;
import java.util.TreeSet;

public class AutomataPool {
    private final int n;
    private SortedSet<AntAutomaton> automata;
    private Test t;
    private static Random rnd = new Random(3);
    private static Comparator<AntAutomaton> comp;

    private SortedSet<AntAutomaton> shrink(SortedSet<AntAutomaton> s) {
        SortedSet<AntAutomaton> res = new TreeSet<AntAutomaton>(comp);
        int i = 0;
        for (AntAutomaton a : s) {
            i++;
            if (rnd.nextDouble() < 1 - 1 / i) {
                continue;
            }
            res.add(a);
            if (res.size() == 5 * n / 6) {
                break;
            }
        }
        for (; i < n; i++) {
            res.add(AntAutomatonFactory.randomAutomaton(t.states));
        }
        return res;
    }

    public AutomataPool(Test t, int n) {
        this.n = n;
        this.t = t;
        comp = new Not<AntAutomaton>(new AutomataComparator(t));
        automata = new TreeSet<AntAutomaton>(comp);
        for (int i = 0; i < n; i++) {
            automata.add(AntAutomatonFactory.randomAutomaton(t.states));
        }
    }

    public AutomataPool(Test t, int n, AntAutomaton initial) {
        this(t, n);
        automata.add(initial);
    }

    public void step() {
        SortedSet<AntAutomaton> auto = new TreeSet<AntAutomaton>(comp);
        for (int i = 0; i < 3 * n; i++) {
            auto.add(AntAutomatonFactory.randomAutomaton(t.states));
        }
        for (AntAutomaton aut : automata) {
            for (int i = 0; i < 3; i++) {
                AntAutomaton newAut = aut.clone();
                newAut.mutate();
                auto.add(newAut);
            }
        }
        automata.addAll(auto);
        automata = shrink(automata);

//        System.err.println("================================================");
//        for (AntAutomaton aut : automata) {
//            Score score = AutomatonFieldInteractor.interact(t.field, aut, t.steps);
//            System.err.println(score);
//            System.err.println(aut);
//        }
//        System.err.println("================================================");


    }

    public AntAutomaton best() {
        return automata.first();
    }
}
