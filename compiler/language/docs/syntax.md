# Context-Free Grammar for Poppy

Terminal symbols: all tokens for the language.

Nonterminal symbols: $\{\}$

Start symbol: $\text{program}$

Generation rules:
$$\begin{align*}
\text{program} &\rightarrow \text{defns END}\\
\text{defns} &\rightarrow \text{defn}\\
\text{defns} &\rightarrow \text{defn defns}\\
\text{defn} &\rightarrow \text{type IDENTIFIER LPAREN optparams RPAREN LBRACE stmts RBRACE}\\
\text{type} &\rightarrow \text{INT}\\
\text{type} &\rightarrow \text{CHAR}\\
\text{type} &\rightarrow \text{STRING} \\
\text{optparams} &\rightarrow \varnothing \\
\text{optparams} &\rightarrow \text{params} \\
\text{params} &\rightarrow \text{param COMMA params}  \\
\text{params} &\rightarrow \text{param}  \\
\text{param} &\rightarrow \text{type IDENTIFIER}  \\
\text{stmts} &\rightarrow \text{stmt}  \\
\text{stmts} &\rightarrow \text{stmt stmts}  \\
\text{stmt} &\rightarrow \text{semistmt SEMICOLON}  \\
\text{semistmt} &\rightarrow \text{type IDENTIFIER ASSIGN expr}  \\
\text{semistmt} &\rightarrow \text{IDENTIFIER ASSIGN expr}  \\
\text{semistmt} &\rightarrow \text{HOP expr}  \\
\text{semistmt} &\rightarrow \text{expr} \\
\text{stmt} &\rightarrow \text{IF LPAREN cond RPAREN LRBACE stmts RBRACE optelse}  \\
\text{optelse} &\rightarrow \varnothing  \\
\text{optelse} &\rightarrow \text{ELSE LBRACE stmts RBRACE}  \\


\text{stmt} &\rightarrow \text{IF LPAREN cond RPAREN LRBACE stmts RBRACE ELSE LBRACE stmts RBRACE}  \\
\text{stmt} &\rightarrow \text{WHILE LPAREN cond RPAREN LBRACE stmts RBRACE}  \\
\text{stmt} &\rightarrow \text{FOR LPAREN semistmt SEMICOLON semistmt SEMICOLON semistmt RPAREN LBRACE stmts RBRACE}  \\
\text{stmt} &\rightarrow \text{FOR LPAREN semistmt SEMICOLON semistmt SEMICOLON semistmt RPAREN LBRACE stmts RBRACE}  \\
\text{cond} &\rightarrow \text{cond AND cond}  \\
\text{cond} &\rightarrow \text{cond OR cond}  \\
\text{cond} &\rightarrow \text{NOT cond}  \\
\text{cond} &\rightarrow \text{expr LT expr}  \\
\text{cond} &\rightarrow \text{expr GT expr}  \\
\text{cond} &\rightarrow \text{expr LE expr}  \\
\text{cond} &\rightarrow \text{expr GE expr}  \\
\text{cond} &\rightarrow \text{expr EQ expr}  \\
\text{cond} &\rightarrow \text{expr NE expr}  \\
\text{expr} &\rightarrow \text{addexpr}  \\
\text{addexpr} &\rightarrow \text{addexpr PLUS multexpr}  \\
\text{addexpr} &\rightarrow \text{addexpr MINUS multexpr}  \\
\text{addexpr} &\rightarrow \text{multexpr}  \\
\text{multexpr} &\rightarrow \text{multexpr TIMES unexpr}  \\
\text{multexpr} &\rightarrow \text{multexpr DIVIDE unexpr}  \\
\text{multexpr} &\rightarrow \text{multexpr MOD unexpr}  \\
\text{multexpr} &\rightarrow \text{unexpr}  \\
\text{unexpr} &\rightarrow \text{LPAREN expr RPAREN}  \\
\text{unexpr} &\rightarrow \text{IDENTIFIER LPAREN optparams RPAREN}  \\
\text{unexpr} &\rightarrow \text{INC IDENTIFIER} \\
\text{unexpr} &\rightarrow \text{DEC IDENTIFIER} \\
\text{unexpr} &\rightarrow \text{IDENTIFIER}  \\
\text{unexpr} &\rightarrow \text{CONSTANT}  \\
\text{unexpr} &\rightarrow \text{STRINGLIT}  \\
\text{unexpr} &\rightarrow \text{CHARLIT}  \\
\end{align*}
$$