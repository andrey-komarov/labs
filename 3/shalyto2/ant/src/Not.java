import java.util.Comparator;

public class Not<T> implements Comparator<T> {
    Comparator<T> comp;

    public Not(Comparator<T> comp) {
        this.comp = comp;
    }

    @Override
    public int compare(T t1, T t2) {
        return -comp.compare(t1, t2);
    }
}
