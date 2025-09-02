# Context-Free Grammar for Poppy

Terminal symbols: all tokens for the language.

Nonterminal symbols: $\{\text{program, defns, defn, type, optparams, stmts, params, param, stmt, semistmt, expr, cond, andcond, orcond, uncond, optelse, addexpr, multexpr, unexpr, optargs, args}\}$

Start symbol: $\text{program}$

Generation rules:
$$\begin{align*}
\text{program} &\rightarrow \text{optincludes defns END}\\
\text{optincludes} &\rightarrow \text{includes}\\
\text{optincludes} &\rightarrow \varnothing\\
\text{includes}&\rightarrow \text{include includes}\\
\text{includes}&\rightarrow \text{include }\\
\text{include}&\rightarrow \text{MUNCH IDENTIFIER }\\
\text{defns} &\rightarrow \text{defn}\\
\text{defns} &\rightarrow \text{defn defns}\\
\text{defn} &\rightarrow \text{type IDENTIFIER LPAREN optparams RPAREN LBRACE stmts RBRACE}\\
\text{type} &\rightarrow \text{INT}\\
\text{type} &\rightarrow \text{VOID}\\
\text{type} &\rightarrow \text{CHAR}\\
\text{type} &\rightarrow \text{BOOL}\\
\text{optparams} &\rightarrow \varnothing \\
\text{optparams} &\rightarrow \text{params} \\
\text{params} &\rightarrow \text{param COMMA params}  \\
\text{params} &\rightarrow \text{param}  \\
\text{param} &\rightarrow \text{type IDENTIFIER}  \\
\text{stmts} &\rightarrow \text{stmt}  \\
\text{stmts} &\rightarrow \text{stmt stmts}  \\
\text{stmt} &\rightarrow \text{semistmt SEMICOLON}  \\
\text{semistmt} &\rightarrow \text{vardec}  \\
\text{vardec} &\rightarrow \text{LET type IDENTIFIER ASSIGN expr}  \\
\text{vardec} &\rightarrow \text{LET type IDENTIFIER}  \\
\text{semistmt} &\rightarrow \text{varasst}  \\
\text{varasst} &\rightarrow \text{IDENTIFIER ASSIGN expr}  \\
\text{semistmt} &\rightarrow \text{ret}  \\
\text{ret} &\rightarrow \text{HOP expr}  \\
\text{ret} &\rightarrow \text{HOP}  \\
\text{semistmt} &\rightarrow \text{expr} \\
\text{stmt} &\rightarrow \text{IF LPAREN expr RPAREN LBRACE stmts RBRACE optelse}  \\
\text{optelse} &\rightarrow \varnothing  \\
\text{optelse} &\rightarrow \text{ELSE LBRACE stmts RBRACE}  \\
\text{stmt} &\rightarrow \text{WHILE LPAREN expr RPAREN LBRACE stmts RBRACE}  \\
\text{stmt} &\rightarrow \text{FOR LPAREN semistmt SEMICOLON expr SEMICOLON semistmt RPAREN LBRACE stmts RBRACE}  \\
\text{expr} &\rightarrow \text{andcond}  \\
\text{andcond} &\rightarrow \text{andcond AND orcond}  \\
\text{andcond} &\rightarrow \text{orcond}  \\
\text{orcond} &\rightarrow \text{orcond OR uncond}  \\
\text{orcond} &\rightarrow \text{uncond}  \\
\text{uncond} &\rightarrow \text{NOT expr}  \\
\text{uncond} &\rightarrow \text{LPAREN expr RPAREN}  \\
\text{uncond} &\rightarrow \text{expr LT expr}  \\
\text{uncond} &\rightarrow \text{expr GT expr}  \\
\text{uncond} &\rightarrow \text{expr LE expr}  \\
\text{uncond} &\rightarrow \text{expr GE expr}  \\
\text{uncond} &\rightarrow \text{expr EQ expr}  \\
\text{uncond} &\rightarrow \text{expr NE expr}  \\
\text{uncond} &\rightarrow \text{TRUE}  \\
\text{uncond} &\rightarrow \text{FALSE}  \\
\text{expr} &\rightarrow \text{addexpr}  \\
\text{addexpr} &\rightarrow \text{addexpr PLUS multexpr}  \\
\text{addexpr} &\rightarrow \text{addexpr MINUS multexpr}  \\
\text{addexpr} &\rightarrow \text{multexpr}  \\
\text{multexpr} &\rightarrow \text{multexpr TIMES unexpr}  \\
\text{multexpr} &\rightarrow \text{multexpr DIVIDE unexpr}  \\
\text{multexpr} &\rightarrow \text{multexpr MOD unexpr}  \\
\text{multexpr} &\rightarrow \text{unexpr}  \\
\text{unexpr} &\rightarrow \text{MINUS UNEXPR} \\
\text{unexpr} &\rightarrow \text{LPAREN expr RPAREN}  \\
\text{unexpr} &\rightarrow \text{IDENTIFIER LPAREN optargs RPAREN}  \\
\text{optargs} &\rightarrow \varnothing  \\
\text{optargs} &\rightarrow \text{args} \\
\text{args} &\rightarrow \text{expr} \\
\text{args} &\rightarrow \text{expr COMMA args} \\
\text{unexpr} &\rightarrow \text{INC IDENTIFIER} \\
\text{unexpr} &\rightarrow \text{DEC IDENTIFIER} \\
\text{unexpr} &\rightarrow \text{IDENTIFIER}  \\
\text{unexpr} &\rightarrow \text{CONSTANT}  \\
\text{unexpr} &\rightarrow \text{SQUOTE CHARLIT SQUOTE}
\end{align*}
$$