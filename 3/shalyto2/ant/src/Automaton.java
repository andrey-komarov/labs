public interface Automaton<A,Q> {
    A go(Q q);
    void reset();
}