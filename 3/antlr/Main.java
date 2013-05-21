import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        ANTLRInputStream input = new ANTLRInputStream(System.in);
        TeXLexer lexer = new TeXLexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        TeXParser parser = new TeXParser(tokens);
        ParseTree tree = parser.document();
        ParseTreeWalker walker = new ParseTreeWalker();
        ToHtml tohtml = new ToHtml();
        walker.walk(tohtml, tree);
        System.out.println(tohtml);
    }
}
