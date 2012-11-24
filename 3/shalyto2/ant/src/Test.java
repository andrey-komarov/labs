import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Test {
    public final int n;
    public final int states;
    public final int steps;
    public final AppleField field;

    public Test(String filename) throws FileNotFoundException {
        Scanner in = new Scanner(new File(filename));
        n = in.nextInt();
        states = in.nextInt();
        steps = in.nextInt();
        in.nextLine();
        String[] s = new String[n];
        for (int i = 0; i < n; i++) {
            s[i] = in.nextLine();
        }
        field = new AppleField(s);
    }
}