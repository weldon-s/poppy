# Context-Free Grammar for Poppy

Terminal symbols: all tokens for the language.

Nonterminal symbols: $\{\text{program, defns, defn, type, optparams, stmts, params, param, stmt, semistmt, expr, cond, andcond, orcond, uncond, optelse, addexpr, multexpr, unexpr}\}$

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
\text{stmt} &\rightarrow \text{WHILE LPAREN cond RPAREN LBRACE stmts RBRACE}  \\
\text{stmt} &\rightarrow \text{FOR LPAREN semistmt SEMICOLON cond SEMICOLON semistmt RPAREN LBRACE stmts RBRACE}  \\
\text{cond} &\rightarrow \text{andcond}  \\
\text{andcond} &\rightarrow \text{andcond AND orcond}  \\
\text{andcond} &\rightarrow \text{orcond}  \\
\text{orcond} &\rightarrow \text{orcond OR uncond}  \\
\text{orcond} &\rightarrow \text{uncond}  \\
\text{uncond} &\rightarrow \text{NOT cond}  \\
\text{uncond} &\rightarrow \text{LPAREN cond RPAREN}  \\
\text{uncond} &\rightarrow \text{expr LT expr}  \\
\text{uncond} &\rightarrow \text{expr GT expr}  \\
\text{uncond} &\rightarrow \text{expr LE expr}  \\
\text{uncond} &\rightarrow \text{expr GE expr}  \\
\text{uncond} &\rightarrow \text{expr EQ expr}  \\
\text{uncond} &\rightarrow \text{expr NE expr}  \\
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