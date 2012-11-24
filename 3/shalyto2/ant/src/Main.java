import java.io.FileNotFoundException;

public class Main {



    public static void main(String[] args) {
//        int[][] go = {
//                {1, 0},
//                {2, 2},
//                {2, 3},
//                {1, 1}
//        };
//        Action[][] actions = {
//                {Action.ROTATE_RIGHT, Action.GO},
//                {Action.GO, Action.GO},
//                {Action.ROTATE_RIGHT, Action.GO},
//                {Action.ROTATE_LEFT, Action.GO}
//        };
//        AntAutomaton aut = new AntAutomaton(go, actions);
//        String[] field = { ".*........"
//                         , ".*........"
//                         , ".*****...."
//                         , ".....*...."
//                         , ".....*...."
//                         , ".........."
//                         , ".....*...."
//                         , ".....*...."
//                         , ".****....."
//                         , ".........."
//                         };
        Test test = null;
        try {
            test = new Test("tests/09");
        } catch (FileNotFoundException e) {
            System.err.println(":(");
            return;
        }
        AutomataFinder finder = new AutomataFinder(test);
        AntAutomaton rand = finder.findUsingRandomAutomata(1000000000);
        AutomataPool pool = new AutomataPool(test, 50, rand);
        for (int i = 0; i < 100000; i++) {
            pool.step();
            if (i % 100 != 0)
                continue;
            AntAutomaton aut = pool.best();
            System.err.println("Step " + i);
            Score score = AutomatonFieldInteractor.interact(test.field, aut, test.steps);
            System.err.println(score);
            System.err.println(aut);
        }

    }
}
