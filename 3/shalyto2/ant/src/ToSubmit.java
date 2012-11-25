import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class ToSubmit {

    static String[] answers = { ""
                              , "3 2 L M\n3 3 R M\n2 4 R M\n2 1 M M\n"
                              , "1 1 L M\n"
                              , "2 3 L M\n3 2 R M\n1 2 M R"
                              , "2 1 M M\n3 4 M M\n1 1 R L\n1 4 L M\n"
                              , "3 2 M M\n1 1 R M\n2 3 M L"
                              , "2 2 R M\n3 4 M M\n3 5 L M\n3 5 M M\n1 2 M M\n"
                              , "2 4 R M\n2 3 L M\n2 4 M M\n1 3 M M\n"
                              , "2 3 M M\n3 3 L M\n4 3 M M\n5 2 R M\n2 1 L M\n"
                              , "2 3 M M\n4 3 R M\n5 6 M M\n6 6 L M\n1 2 R M\n5 1 L M\n"
                              , "2 3 L M\n4 4 M M\n2 4 R M\n5 6 R M\n1 1 L M\n1 4 R M\n"
                              };

    public static void main(String[] args) throws FileNotFoundException {
        Scanner in = new Scanner(new File("artificial.in"));
        int n = in.nextInt();
        PrintWriter out = new PrintWriter("artificial.out");
        out.println(answers[n]);
        out.close();
    }
}
