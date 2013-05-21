grammar TeX;

document : paragraph (NEWPAR paragraph)* ;

paragraph : unit*;

unit : math | text | section;

section : SECTION '{' text '}';

text1 : (NUM | WORD) PREP* | PREP;

text : text1+;

mathexpr : '{' mathexpr* '}'                # Block
         | LP mathexpr RP                   # Paren
         | WORD                             # Word
         | NUM                              # Num
         | FRAC mathexpr mathexpr           # Frac
         | mathexpr '_' mathexpr            # Sub
         | mathexpr '^' mathexpr            # Sup         
         | SQRT mathexpr                    # Sqrt
         | mathexpr binop mathexpr          # BinopExpr
         | mathexpr binrel mathexpr         # BinrelExpr
         ;

binop : BINOP ;
binrel : BINREL ;

math : '$' mathexpr* '$'     
     ;

FRAC : '\\frac' ;
SQRT : '\\sqrt' ;
SECTION : '\\section' | '\\subsection' | '\\subsubsection' | '\\subsubsubsection' ;
BINOP : '+' | '-' | '*' | '/' ;
BINREL : '<' | '>' | '=' | '\\ne' | '\\le' | '\\ge' ;
WORD : LETTER (LETTER | DIGIT)*;
NUM : DIGIT+;
DIGIT : [0-9];
LETTER : [a-zA-Zа-яА-ЯёЁ] ;
MATHMODE : '$';
SUP : '^' ;
SUB : '_' ;
LP : '(' | '[' | '\\{' | '\\lfloor' | '\\lceil' | '|' ;
RP : ')' | ']' | '\\}' | '\\rfloor' | '\\rceil' | '|' ;
BSLASH : '\\' ;
LCB : '{' ;
RCP : '}' ;
PREP : [,.;:!?%] | BINOP ;
NEWPAR : '\n' '\n'+;
WS : [ \t\n\r]+ -> skip ; // toss out all whitespace
