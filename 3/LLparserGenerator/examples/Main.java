import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNext()) {
            String s = sc.nextLine();
            /*
            SadlyLexer lexer = new SadlyLexer(s);
            SadlyLexem lexem;
            while ((lexem = lexer.nextLexem()) != SadlyLexem.EOF) {
                System.err.println(lexem);
            }
            */
            SadlyParser.NodeE e = SadlyParser.parse(s);
            System.out.println(e.val);
        }
    }
}
