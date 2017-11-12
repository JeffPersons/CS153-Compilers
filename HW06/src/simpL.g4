/*
 * Notes:
 * - Earlier the definition, greater the precedence
 * - Identifier is defined to avoid name clashes with other keywords
 * 
 * TODO: add string concatenation
 * TODO: change how we deal with expressions and statements, breakup by type
 * TODO: add return value to function definition
 */
grammar simpL;

/*
 * types of statements:
 * - simple statements: assignment, return
 * - compound statements:
 *   - func def, loop, etc (anything with a block enclosed with {})
 * 
 * types of expressions:
 * - function calls/simple expressions
 */
// starting rule
program : block;
block   : statement* return_statement?;
return_statement : RETURN expression EOS;

// declarations and assignments
declaration : basic_type_name (NAME EOS | assignment EOS);
assignment  : NAME ASSIGN expression;

// statements and expressions
statement     : simple_stmt | compound_stmt;
simple_stmt   : bool_expr   | arith_expr;
compound_stmt : func_def    | if_stmt;
if_stmt       : IF LPAREN simple_stmt RPAREN LCURL block RCURL
                    (ELSE_IF LPAREN simple_stmt RPAREN LCURL block RCURL)* (ELSE LCURL block RCURL)?;
func_def      : DEF NAME LPAREN param_list RPAREN LCURL block RCURL;
param_list    : (basic_type_name NAME (SEPARATOR basic_type_name NAME)*)?;

expression    : arith_expr | bool_expr;
arith_expr    : arith_expr (ADD | SUB) term | term;
term          : term (MUL | DIV) power | power;
power         : factor POW power | factor;
factor        : LPAREN arith_expr RPAREN | NAME | NUMBER_VALUE;
bool_expr     : BOOLEAN_VALUE bool_operator BOOLEAN_VALUE | NAME bool_operator NAME;

// groupings of reserved symbols
bool_operator  : AND   | OR     | NOT;
arith_operator : ADD   | SUB    | MUL | DIV | POW;
comp_operator  : IS_EQ | NOT_EQ | GT  | LT  | LTE | GTE;

// basic labels
basic_type_name  : TEXT       | NUMBER       | BOOLEAN;
basic_type_value : TEXT_VALUE | NUMBER_VALUE | BOOLEAN_VALUE;

// values
TEXT_VALUE    : QUOTE ~[QUOTE]* QUOTE;
NUMBER_VALUE  : ([0-9]+ | [0-9]+.[0-9]+);
BOOLEAN_VALUE : 'true' | 'false';

// reserved words
TEXT    : 'Text';
NUMBER  : 'Number';
BOOLEAN : 'Boolean';
IF      : 'if';
ELSE    : 'else';
ELSE_IF : 'else if';
DEF     : 'def';
RETURN  : 'return';

// reserved symbols
ASSIGN    : '=';
QUOTE     : '\'';
SEPARATOR : ',';
LPAREN    : '(';
RPAREN    : ')';
LCURL     : '{';
RCURL     : '}';
LSQUARE   : '[';
RSQUARE   : ']';

// comparison and boolean operators
GT     : '>';
LT     : '<';
LTE    : '<=';
GTE    : '>=';
NOT    : 'not';
IS_EQ  : '!=';
NOT_EQ : '==';
AND    : 'and';
OR     : 'or';

// arithmetic operators
ADD : '+';
SUB : '-';
MUL : '*';
DIV : '/';
POW : '^';

// misc
EOS        : NEWLINE+;
SKIP       : (WHITESPACE | COMMENT) -> skip;
NAME       : [_a-zA-Z]+[_0-9a-zA-Z]*;
COMMENT    : '#' ~[\r\n]*;
WHITESPACE : [ \t]+;

// fragments (helper definitions)
fragment DIGIT   : [0-9];
fragment NEWLINE : '\n' | '\r\n';
