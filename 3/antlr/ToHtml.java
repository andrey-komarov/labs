import java.util.HashMap;
import java.util.Map;

public class ToHtml extends TeXBaseListener {
    private StringBuilder out;
    private int level;
    private static int DSPACE = 2;
    private static String[][] BINOPS = {
            {"+", "+"}, {"-", "-"}, {"*", "*"}, {"/", "/"}
    };
    private static String[][] BINRELS = {
            {"<", "&lt;"}, {">", "&gt;"}, {"\\le", "&le;"}
          , {"\\ge", "&ge;"}, {"\\ne", "&ne;"}, {"=", "="}
    };
    private static String[][] PARENS = {
            {"(", "("}, {")", ")"}, {"[", "["}, {"]", "]"}
          , {"\\{", "{"}, {"\\}", "}"}, {"\\lfloor", "&LeftFloor;"},
            {"\\rfloor", "&RightFloor;"}, {"\\lceil", "&LeftCeiling;"},
            {"\\rceil", "&RightCeiling;"}, {"|", "|"}
    };
    private static String[][] SECTIONS = {
            {"\\section", "h1"}, {"\\subsection" ,"h2"}, {"\\subsubsection", "h3"}
          , {"\\subsubsection", "h4"}
    };

    private static Map<String, String> BINOPSM = mkMap(BINOPS);
    private static Map<String, String> BINRELSM = mkMap(BINRELS);
    private static Map<String, String> PARENSM = mkMap(PARENS);
    private static Map<String, String> SECTIONSM = mkMap(SECTIONS);

    private static Map<String, String> mkMap(String[][] assocArr) {
        HashMap<String, String> res = new HashMap<String, String>();
        for (String[] assoc : assocArr) {
            assert assoc.length == 2;
            assert !res.containsKey(assoc[0]);
            res.put(assoc[0], assoc[1]);
        }
        return res;
    }

    public static String times(int n, String s) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < n; i++) {
            sb.append(s);
        }
        return sb.toString();
    }

    private void tab() {
        level += DSPACE;
    }

    private void untab() {
        level -= DSPACE;
    }

    private void append(String s) {
//        out.append(times(level, " "));
        out.append(s);
        out.append(" ");
//        out.append("\n");
    }

    public ToHtml() {
        out = new StringBuilder();
        level = 0;
    }

    @Override
    public void enterMath(TeXParser.MathContext ctx) {
        super.enterMath(ctx);
        append("<math xmlns=\"http://www.w3.org/1998/Math/MathML\">");
        tab();
    }

    @Override
    public void exitMath(TeXParser.MathContext ctx) {
        untab();
        append("</math>");
        super.exitMath(ctx);
    }

    @Override
    public void enterNum(TeXParser.NumContext ctx) {
        super.enterNum(ctx);
        append("<mn>");
        tab();
        append(ctx.NUM().getText());
    }

    @Override
    public void exitNum(TeXParser.NumContext ctx) {
        untab();
        append("</mn>");
        super.exitNum(ctx);
    }

    @Override
    public void enterSup(TeXParser.SupContext ctx) {
        super.enterSup(ctx);
        append("<msup>");
        tab();
    }

    @Override
    public void exitSup(TeXParser.SupContext ctx) {
        untab();
        append("</msup>");
        super.exitSup(ctx);
    }

    @Override
    public void enterSub(TeXParser.SubContext ctx) {
        super.enterSub(ctx);
        append("<msub>");
        tab();
    }

    @Override
    public void exitSub(TeXParser.SubContext ctx) {
        untab();
        append("</msub>");
        super.exitSub(ctx);
    }

    @Override
    public void enterWord(TeXParser.WordContext ctx) {
        super.enterWord(ctx);
        append("<mi>");
        tab();
        append(ctx.WORD().getText());
    }

    @Override
    public void exitWord(TeXParser.WordContext ctx) {
        untab();
        append("</mi>");
        super.exitWord(ctx);
    }

    @Override
    public void enterFrac(TeXParser.FracContext ctx) {
        super.enterFrac(ctx);
        append("<mfrac>");
        tab();
    }

    @Override
    public void exitFrac(TeXParser.FracContext ctx) {
        untab();
        append("</mfrac>");
        super.exitFrac(ctx);
    }

    @Override
    public void enterBinop(TeXParser.BinopContext ctx) {
        super.enterBinop(ctx);
        append("<mo>" + BINOPSM.get(ctx.BINOP().toString()) + "</mo>");
    }

    @Override
    public void enterBinopExpr(TeXParser.BinopExprContext ctx) {
        super.enterBinopExpr(ctx);
        append("<mrow>");
        tab();
    }

    @Override
    public void exitBinopExpr(TeXParser.BinopExprContext ctx) {
        untab();
        append("</mrow>");
        super.exitBinopExpr(ctx);
    }

    @Override
    public void enterParen(TeXParser.ParenContext ctx) {
        super.enterParen(ctx);
        append("<mo>" + PARENSM.get(ctx.LP().toString()) + "</mo>");
        tab();
    }

    @Override
    public void exitParen(TeXParser.ParenContext ctx) {
        untab();
        append("<mo>" + PARENSM.get(ctx.RP().toString()) + "</mo>");
        super.exitParen(ctx);
    }

    @Override
    public void enterBinrelExpr(TeXParser.BinrelExprContext ctx) {
        super.enterBinrelExpr(ctx);
        append("<mrow>");
        tab();
    }

    @Override
    public void exitBinrelExpr(TeXParser.BinrelExprContext ctx) {
        untab();
        append("</mrow>");
        super.exitBinrelExpr(ctx);
    }

    @Override
    public void enterBlock(TeXParser.BlockContext ctx) {
        append("<mrow>");
        tab();
        super.enterBlock(ctx);
    }

    @Override
    public void exitBlock(TeXParser.BlockContext ctx) {
        untab();
        append("</mrow>");
        super.exitBlock(ctx);
    }

    @Override
    public void enterParagraph(TeXParser.ParagraphContext ctx) {
        append("<p>");
        tab();
        super.enterParagraph(ctx);
    }

    @Override
    public void exitParagraph(TeXParser.ParagraphContext ctx) {
        untab();
        append("</p>");
        super.exitParagraph(ctx);
    }

    @Override
    public void enterDocument(TeXParser.DocumentContext ctx) {
        super.enterDocument(ctx);
        append("<!DOCTYPE html>");
        append("<html>");
        append("<head>");
        append("<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" >");
        append("<title>Converted text</title>");
        append("<script type=\"text/javascript\"");
        append("    src=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\">");
        append("</script>");
        append("</head>");
        append("<body>");
    }

    @Override
    public void exitDocument(TeXParser.DocumentContext ctx) {
        append("</body>");
        append("</html>");
        super.exitDocument(ctx);
    }

    @Override
    public void enterSection(TeXParser.SectionContext ctx) {
        super.enterSection(ctx);
        append("<" + SECTIONSM.get(ctx.SECTION().toString()) + ">");
        tab();
    }

    @Override
    public void exitSection(TeXParser.SectionContext ctx) {
        untab();
        append("</" + SECTIONSM.get(ctx.SECTION().toString()) + ">");
        super.exitSection(ctx);
    }

    @Override
    public void enterText1(TeXParser.Text1Context ctx) {
        super.enterText1(ctx);
        append(ctx.getText());
    }

    @Override
    public void enterBinrel(TeXParser.BinrelContext ctx) {
        super.enterBinrel(ctx);
        append("<mo>" + BINRELSM.get(ctx.BINREL().toString()) + "</mo>");
    }

    @Override
    public String toString() {
        return out.toString();
    }
}
