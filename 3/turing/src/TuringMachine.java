//import java.io.File;
//import java.io.FileNotFoundException;
//import java.util.*;
//
//public class TuringMachine {
//    private Map<StateSymbol, StateSymbolDirection> delta;
//    private String blank;
//    private String start;
//    private String accept;
//    private String reject;
//
//    public TuringMachine(File file) throws FileNotFoundException {
//        Scanner in = new Scanner(file);
//        delta = new HashMap<>();
//        while (in.hasNextLine()) {
//            String s = in.nextLine();
//            if (s.startsWith("start:")) {
//                StringTokenizer st = new StringTokenizer(s);
//                st.nextToken();
//                if (!st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                String start = st.nextToken();
//                if (st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                if (this.start != null) {
//                    throw new IncorrectConfiguration("Two entry points: <" + this.start + "> and <" + start + ">");
//                }
//                this.start = start;
//            } else if (s.startsWith("accept:")) {
//                StringTokenizer st = new StringTokenizer(s);
//                st.nextToken();
//                if (!st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                String accept = st.nextToken();
//                if (st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                if (this.accept != null) {
//                    throw new IncorrectConfiguration("Two accept states: <" + this.accept + "> and <" + accept + ">");
//                }
//                this.accept = accept;
//            } else if (s.startsWith("reject:")) {
//                StringTokenizer st = new StringTokenizer(s);
//                st.nextToken();
//                if (!st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                String reject = st.nextToken();
//                if (st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                if (this.reject != null) {
//                    throw new IncorrectConfiguration("Two reject states: <" + this.reject + "> and <" + reject + ">");
//                }
//                this.reject = reject;
//            } else if (s.startsWith("blank:")) {
//                StringTokenizer st = new StringTokenizer(s);
//                st.nextToken();
//                if (!st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                String blank = st.nextToken();
//                if (st.hasMoreTokens()) {
//                    throw new ParseError(s);
//                }
//                if (this.blank != null) {
//                    throw new IncorrectConfiguration("Two different blanks: <" + this.blank + "> and <" + blank + ">");
//                }
//                this.blank = blank;
//            } else if (s.contains("->")) {
//                StringTokenizer st = new StringTokenizer(s);
//                try {
//                    String stateFrom = st.nextToken();
//                    String symbolFrom = st.nextToken();
//                    String arrow = st.nextToken();
//                    String stateTo = st.nextToken();
//                    String symbolTo = st.nextToken();
//                    String direction = st.nextToken();
//                    StateSymbol from = new StateSymbol(stateFrom, symbolFrom);
//                    StateSymbol to = new StateSymbol(stateTo, symbolTo);
//                    StateSymbolDirection too = new StateSymbolDirection(to, Direction.fromString(direction));
//                    if (delta.containsKey(from)) {
//                        throw new IncorrectConfiguration("Multiple rules for: <" + from + ">");
//                    }
//                    delta.put(from, too);
//
//                } catch (NoSuchElementException e) {
//                    throw new ParseError("Cannot parse rule: <" + s + ">");
//                }
//            } else {
//                throw new ParseError("Don't know what to do with <" + s + ">");
//            }
//        }
//        if (start == null) {
//            throw new IncorrectConfiguration("No start state found.");
//        }
//        if (accept == null) {
//            throw new IncorrectConfiguration("No accepting state found.");
//        }
//        if (reject == null) {
//            throw new IncorrectConfiguration("No rejecting state found.");
//        }
//        if (blank == null) {
//            throw new IncorrectConfiguration("No blank symbol found.");
//        }
//
//    }
//
//    public Result run(String[] initialTape, int maxIterations) {
//        String currentState = start;
//        Tape tape = new Tape(initialTape);
//        int iterations = 0;
//        while (iterations < maxIterations && !currentState.equals(accept) && !currentState.equals(reject)) {
//            StateSymbol ss = new StateSymbol(currentState, tape.get());
//            if (!delta.containsKey(ss)) {
//                return new Result(false, tape, iterations);
//            }
//            StateSymbolDirection ssd = delta.get(ss);
//            tape.set(ssd.ss.symbol);
//            tape.move(ssd.dir);
//            currentState = ssd.ss.state;
//            iterations++;
//        }
//        if (currentState.equals(accept)) {
//            return new Result(true, tape, iterations);
//        } else if (currentState.equals(reject)) {
//            return new Result(false, tape, iterations);
//        } else {
//            return null;
//        }
//    }
//
//    public static class ParseError extends RuntimeException {
//        ParseError(String comment) {
//            super(comment);
//        }
//    }
//
//    public static class IncorrectConfiguration extends RuntimeException {
//        IncorrectConfiguration(String comment) {
//            super(comment);
//        }
//    }
//
//    private static enum Direction {
//        LEFT, RIGHT, STAY;
//
//        @Override
//        public String toString() {
//            if (this.equals(LEFT)) {
//                return "←";
//            } else if (this.equals(RIGHT)) {
//                return "→";
//            } else if (this.equals(STAY)) {
//                return "↑";
//            }
//            return null;
//        }
//
//        static Direction fromString(String s) {
//            if (s.equals("<") || s.equals("←"))
//                return LEFT;
//            else if (s.equals(">") || s.equals("→"))
//                return RIGHT;
//            else if (s.equals("^") || s.equals("↑"))
//                return STAY;
//            else
//                throw new ParseError("Strange direction: <" + s + ">");
//        }
//    }
//
//    private static class StateSymbolDirection {
//        final StateSymbol ss;
//        final Direction dir;
//
//        StateSymbolDirection(StateSymbol ss, Direction dir) {
//            this.ss = ss;
//            this.dir = dir;
//        }
//
//        @Override
//        public String toString() {
//            return "(" + ss + " " + dir + ")";
//        }
//    }
//
//    private static class StateSymbol {
//        final String state;
//        final String symbol;
//
//        StateSymbol(String state, String symbol) {
//            this.state = state;
//            this.symbol = symbol;
//        }
//
//        @Override
//        public boolean equals(Object o) {
//            if (this == o) return true;
//            if (!(o instanceof StateSymbol)) return false;
//
//            StateSymbol that = (StateSymbol) o;
//
//            if (!state.equals(that.state)) return false;
//            if (!symbol.equals(that.symbol)) return false;
//
//            return true;
//        }
//
//        @Override
//        public int hashCode() {
//            int result = state.hashCode();
//            result = 31 * result + symbol.hashCode();
//            return result;
//        }
//
//        @Override
//        public String toString() {
//            return state + " " + symbol;
//        }
//    }
//
//    public class Result {
//        boolean isAccepted;
//        int steps;
//        String[] tape;
//        int posInTape;
//
//        private Result(boolean accepted, Tape tape, int steps) {
//            this.steps = steps;
//            isAccepted = accepted;
//            Deque<String> left = new ArrayDeque<>(tape.left);
//            Deque<String> right = new ArrayDeque<>(tape.right);
//            while (!left.isEmpty() && left.peekLast().equals(blank)) {
//                left.pollLast();
//            }
//            while (right.size() > 1 && right.peekLast().equals(blank)) {
//                right.pollLast();
//            }
//            posInTape = left.size();
//            List<String> res = new ArrayList<>(left);
//            Collections.reverse(res);
//            res.addAll(right);
//            this.tape = new String[res.size()];
//            for (int i = 0; i < this.tape.length; i++) {
//                this.tape[i] = res.get(i);
//            }
//        }
//
//        @Override
//        public String toString() {
//            return "Result{" +
//                    "isAccepted=" + isAccepted +
//                    ", tape=" + (tape == null ? null : Arrays.asList(tape)) +
//                    ", iter=" + steps +
//                    ", pos=" + posInTape +
//                    '}';
//        }
//    }
//
//    private class Tape {
//        private Deque<String> left, right;
//
//        public Tape(String[] ss) {
//            left = new ArrayDeque<>();
//            right = new ArrayDeque<>();
//            for (int i = ss.length - 1; i >= 0; i--) {
//                right.push(ss[i]);
//            }
//        }
//
//        public void shiftLeft() {
//            if (left.isEmpty()) {
//                right.push(blank);
//            } else {
//                right.push(left.pop());
//            }
//        }
//
//        public void shiftRight() {
//            if (right.isEmpty()) {
//                left.push(blank);
//            } else {
//                left.push(right.pop());
//            }
//        }
//
//        public void set(String s) {
//            if (!right.isEmpty())
//                right.pop();
//            right.push(s);
//        }
//
//        public void move(Direction dir) {
//            if (dir.equals(Direction.LEFT)) {
//                shiftLeft();
//            } else if (dir.equals(Direction.RIGHT)) {
//                shiftRight();
//            }
//        }
//
//        public String get() {
//            if (right.isEmpty()) {
//                return blank;
//            } else {
//                return right.peek();
//            }
//        }
//    }
//
//    @Override
//    public String toString() {
//        return "TuringMachine{" +
//                "delta=" + delta +
//                ", blank='" + blank + '\'' +
//                ", start='" + start + '\'' +
//                ", accept='" + accept + '\'' +
//                ", reject='" + reject + '\'' +
//                '}';
//    }
//}
