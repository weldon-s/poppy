# Poppy's Type System

Poppy's types are defined according to the context-free grammar below. The starting symbol is $\text{type}$.
$$
\begin{align*}
\text{type}&\rightarrow \text{assignable}\\
\text{type}&\rightarrow \text{void}\\
\text{type}&\rightarrow \text{bool}\\
\text{assignable}&\rightarrow \text{int}\\
\text{assignable}&\rightarrow \text{char}\\
\text{assignable}&\rightarrow \text{string}\\
\text{type}&\rightarrow \text{(optparams) $\Rightarrow$ type}\\
\text{optparams}&\rightarrow \varnothing\\
\text{optparams}&\rightarrow \text{params}\\
\text{params}&\rightarrow \text{param, params}\\
\text{param}&\rightarrow \text{assignable}\\
\text{param}&\rightarrow \text{void}\\
\text{param}&\rightarrow \text{bool}\\
\end{align*}
$$

Poppy has the following typing inference rules:

## Program
$$\frac{\forall i \text{ } \overline{\text{defns}} \vdash \text{defns}_i}{\varnothing \vdash \overline{\text{defns}}}$$

## Functions

$$\frac{\Gamma(\text{IDENTIFIER}) = \tau \text{ IDENTIFIER}(\overline{\tau \text{ IDENTIFIER}}) \quad \tau \in \{\text{int, char, string}\}}{\Gamma \vdash \text{IDENTIFIER}:(\overline{\tau}) \rightarrow \tau}$$

$$\frac{\Gamma \vdash E:(\tau_1,\dots,\tau_n)\to \tau' \quad \forall i \in [n], \Gamma \vdash E_i : \tau_i} {
    \Gamma \vdash E(E_1,\dots, E_n): \tau'
}$$

$$\frac{\text{all symbol names in params, $E$ distinct} \quad \Gamma \vdash E: \tau \quad \tau \in \{\text{int, char, string}\}}{\Gamma \vdash \tau \text{ IDENTIFIER }(\overline{\text{params}})\{E\}}$$

## Variables

$$\frac{\Gamma(\text{IDENTIFIER}) = \tau \text{ IDENTIFIER}; \quad \tau \in \{\text{int, char, string}\}}{\Gamma \vdash \text{IDENTIFIER} : \tau}$$

$$\frac{\Gamma(\text{IDENTIFIER}) = \tau \text{ IDENTIFIER} = E; \quad \Gamma \vdash E : \tau \quad \tau \in \{\text{int, char, string}\}}{\Gamma \vdash \text{IDENTIFIER} : \tau \quad \Gamma \vdash \text{IDENTIFIER} = E : \tau}$$

## Statements

$$\frac{\Gamma \vdash E_1: \tau_1 \quad \Gamma \vdash E_2: \tau_2 \quad E_1, E_2 \in \text{stmt}}{
    \Gamma \vdash E_1 E_2 : \tau_2
}$$

$$\frac{\Gamma \vdash E : \tau \quad \tau \in \{\text{int, char, string}\}}{
    \Gamma \vdash \text{hop } E; : \tau
}$$

## Predicates
$$\frac{\Gamma \vdash E_1:\tau \quad \Gamma \vdash E_2:\tau}{
    \Gamma \vdash E_1 == E_2 : \text{bool} \quad \Gamma \vdash E_1 \text{ != } E_2 : \text{bool}
}$$

$$\frac{\Gamma \vdash E_1:\text{int} \quad \Gamma \vdash E_2:\text{int}}{
    \Gamma \vdash E_1 < E_2 : \text{bool} \quad \Gamma \vdash E_1 > E_2 : \text{bool} \quad
    \Gamma \vdash E_1 <= E_2 : \text{bool} \quad \Gamma \vdash E_1 >= E_2 : \text{bool}
}$$

$$\frac{\Gamma \vdash E_1:\text{char} \quad \Gamma \vdash E_2:\text{char}}{
    \Gamma \vdash E_1 < E_2 : \text{bool} \quad \Gamma \vdash E_1 > E_2 : \text{bool} \quad
    \Gamma \vdash E_1 <= E_2 : \text{bool} \quad \Gamma \vdash E_1 >= E_2 : \text{bool}
}$$

$$\frac{\Gamma \vdash E : \text{bool}}{
    \Gamma \vdash \text{ !}E : \text{bool}
}$$

$$\frac{\Gamma \vdash E_1 : \text{bool} \quad \Gamma \vdash E_2 : \text{bool}}{
    \Gamma \vdash E_1 \text{ \&\& } E_2 : \text{bool} \quad     \Gamma \vdash E_1 \text{ || } E_2 : \text{bool}
}$$

## Arithmetic
$$\frac{\Gamma \vdash E_1: \text{int}\quad \Gamma \vdash E_2: \text{int}}{
    \Gamma \vdash E_1 + E_2 : \text{int} \quad \Gamma \vdash E_1 - E_2 : \text{int} \quad \Gamma \vdash E_1 * E_2 : \text{int} \quad \Gamma \vdash E_1 \text{ / }E_2 : \text{int} \quad \Gamma \vdash E_1 
    \text{ \% } E_2 : \text{int}
}$$

$$\frac{\Gamma \vdash E: \tau}{\Gamma \vdash (E):\tau}$$

## String manipulation
$$\frac{\Gamma \vdash E_1: \text{string}\quad \Gamma \vdash E_2:\text{string}}{
    \Gamma \vdash E_1 + E_2 : \text{string}
}$$

$$\frac{\Gamma \vdash E_1: \text{string}\quad \Gamma \vdash E_2:\text{int}}{
    \Gamma \vdash E_1 + E_2 : \text{char}
}$$

## Literals

$$\Gamma \vdash \text{CONSTANT}: \text{int}$$
$$\Gamma \vdash \text{STRLIT}: \text{string}$$
$$\Gamma \vdash \text{CHARLIT}: \text{char}$$
