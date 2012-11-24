public class Score implements Comparable<Score> {
    private int turnsDone;
    private int applesEaten;

    public Score(int applesEaten, int turnsDone) {
        this.applesEaten = applesEaten;
        this.turnsDone = turnsDone;
    }

    int fitnessFunction() {
        return applesEaten * 10000 - turnsDone;
    }

    @Override
    public int compareTo(Score s) {
        return Integer.compare(fitnessFunction(), s.fitnessFunction());
    }

    @Override
    public String toString() {
        return "Score{" +
                "turns=" + turnsDone +
                ", apples=" + applesEaten +
                '}';
    }
}
