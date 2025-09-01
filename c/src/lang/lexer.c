#include "lang/lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NO_CHARLIT 0
#define OPENED_CHARLIT 1
#define UNCLOSED_CHARLIT 2

#define MAX_TOKEN_LENGTH 128

bool is_numeric(char c){
        return (c >= '0') && (c <= '9');
}

bool is_alphabetic(char c){
        return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) || (c == '_');
}

struct lex_data {
        enum symbol type;
        char excess;
        int val_len;
};

struct lex_data find_prefixed_type(FILE* file, char expected_second, enum symbol not_second, enum symbol if_second, char *val){
        struct lex_data ret;
        ret.excess = 0;
        ret.val_len = 1;
        ret.type = not_second;

        if (feof(file)){
                return ret;
        }
        
        char second = fgetc(file);
        if (second == expected_second){
                val[1] = second;
                ret.val_len = 2;
                ret.type = if_second;
        } else {
                ret.excess = second;        
        }

        return ret;

}

struct lex_data find_numeric_value (FILE* file, char *val){
        struct lex_data ret;
        ret.type = SYMBOL_CONSTANT;
        ret.excess = 0;
        ret.val_len = 1;
        
        while (!feof(file) && (ret.val_len <= MAX_TOKEN_LENGTH) && is_numeric(val[ret.val_len - 1])){
                val[ret.val_len] = fgetc(file);
                ++ret.val_len;
        }

        if (!feof(file)){
                --ret.val_len;
                ret.excess = val[ret.val_len];                        
        }

        return ret;
}

struct lex_data find_alphanumeric_value (FILE* file, char *val){
        struct lex_data ret;
        ret.excess = 0;
        ret.val_len = 1;
        
        while (!feof(file) && (ret.val_len <= MAX_TOKEN_LENGTH) && (is_numeric(val[ret.val_len - 1]) || is_alphabetic(val[ret.val_len - 1]))){
                val[ret.val_len] = fgetc(file);
                ++ret.val_len;
        }

        if (!feof(file)){
                --ret.val_len;
                ret.excess = val[ret.val_len];                        
        }
        if ((ret.val_len == 4) && (strncmp(val, "char", 4) == 0)){
                ret.type = SYMBOL_CHAR;
        } else if ((ret.val_len == 4) && (strncmp(val, "else", 4) == 0)){
                ret.type = SYMBOL_ELSE;
        } else if ((ret.val_len == 3) && (strncmp(val, "for", 3) == 0)){
                ret.type = SYMBOL_FOR;
        } else if ((ret.val_len == 3) && (strncmp(val, "hop", 3) == 0)){
                ret.type = SYMBOL_HOP;
        } else if ((ret.val_len == 2) && (strncmp(val, "if", 2) == 0)){
                ret.type = SYMBOL_IF;
        } else if ((ret.val_len == 3) && (strncmp(val, "int", 3) == 0)){
                ret.type = SYMBOL_INT;
        } else if ((ret.val_len == 3) && (strncmp(val, "let", 3) == 0)){
                ret.type = SYMBOL_LET;
        } else if ((ret.val_len == 5) && (strncmp(val, "munch", 5) == 0)){
                ret.type = SYMBOL_MUNCH;
        } else if ((ret.val_len == 4) && (strncmp(val, "void", 4) == 0)){
                ret.type = SYMBOL_VOID;
        } else if ((ret.val_len == 5) && (strncmp(val, "while", 3) == 0)){
                ret.type = SYMBOL_WHILE;
        } else {
                ret.type = SYMBOL_IDENTIFIER;
        }

        return ret;
}
/*
match escape sequences to the chars they escape by editing val[0]
return true if the lex is still valid
*/
bool lex_char_literal(FILE *file, char val[MAX_TOKEN_LENGTH + 1]){
        if (val[0] != '\\'){
                return true;
        }

        if (feof(file)){
                return false;
        }

        switch(fgetc(file)){
                case 'a':
                        val[0] = '\a';
                        break;
                case 'b':
                        val[0] = '\b';
                        break;
                case 'f':
                        val[0] = '\f';
                        break;
                case 'n':
                        val[0] = '\n';
                        break;
                case 'r':
                        val[0] = '\r';
                        break;
                case 't':
                        val[0] = '\t';
                        break;
                case 'v':
                        val[0] = '\v';
                        break;
                case '\\':
                        val[0] = '\\';
                        break;
                case '\'':
                        val[0] = '\'';
                        break;
                case '\"':
                        val[0] = '\"';
                        break;
                default:
                        return false;
        }

        return true;
}

struct LIST(token)* lex(FILE *file){
        char val[MAX_TOKEN_LENGTH + 1];
        val[0] = 0;
        struct LIST(token) *list = (struct LIST(token)*) malloc(sizeof(struct LIST(token)));
        init_list(list);
        int charlit_status = NO_CHARLIT;

        while (!feof(file)){
                if (val[0] == 0){
                        val[0] = fgetc(file);
                }

                struct lex_data data;
                data.type = SYMBOL_NULL;
                data.excess = 0;

                if (charlit_status == OPENED_CHARLIT){
                        data.type = SYMBOL_CHARLIT;
                        data.val_len = 1;
                        if (!lex_char_literal(file, val)){
                                free_list(list, free_token, token);
                                free(list);
                                return NULL;
                        }
                } else {
                        if (isspace(val[0])){
                                val[0] = 0;
                                continue;
                        }

                        switch (val[0]){
                                case -1:
                                        data.type = SYMBOL_END;
                                        data.val_len = 0;
                                        break;
                                case '(':
                                        data.type = SYMBOL_LPAREN;
                                        data.val_len = 1;
                                        break;
                                case ')':
                                        data.type = SYMBOL_RPAREN;
                                        data.val_len = 1;
                                        break;
                                case '{':
                                        data.type = SYMBOL_LBRACE;
                                        data.val_len = 1;
                                        break;
                                case '}':
                                        data.type = SYMBOL_RBRACE;
                                        data.val_len = 1;
                                        break;
                                case '*':
                                        data.type = SYMBOL_TIMES;
                                        data.val_len = 1;
                                        break;
                                case '/':
                                        data.type = SYMBOL_DIVIDE;
                                        data.val_len = 1;
                                        break;
                                case '%':
                                        data.type = SYMBOL_MOD;
                                        data.val_len = 1;
                                        break;
                                case ',':
                                        data.type = SYMBOL_COMMA;
                                        data.val_len = 1;
                                        break;
                                case ';':
                                        data.type = SYMBOL_SEMICOLON;
                                        data.val_len = 1;
                                        break;
                                case '\'':
                                        data.type = SYMBOL_SQUOTE;
                                        data.val_len = 1;
                                        if (charlit_status == NO_CHARLIT){
                                                charlit_status = OPENED_CHARLIT;
                                        } else if (charlit_status == UNCLOSED_CHARLIT){
                                                charlit_status = NO_CHARLIT;
                                        }
                                        break;
                                case '+':
                                        data = find_prefixed_type(file, '+', SYMBOL_PLUS, SYMBOL_INC, val);
                                        break;
                                case '-':
                                        data = find_prefixed_type(file, '-', SYMBOL_MINUS, SYMBOL_DEC, val);
                                        break;
                                case '&':
                                        data = find_prefixed_type(file, '&', SYMBOL_NULL, SYMBOL_AND, val);
                                        break;                        
                                case '|':
                                        data = find_prefixed_type(file, '|', SYMBOL_NULL, SYMBOL_OR, val);
                                        break;   
                                case '!':
                                        data = find_prefixed_type(file, '=', SYMBOL_NOT, SYMBOL_NE, val);
                                        break;      
                                case '>':
                                        data = find_prefixed_type(file, '=', SYMBOL_GT, SYMBOL_GE, val);
                                        break;      
                                case '<':
                                        data = find_prefixed_type(file, '=', SYMBOL_LT, SYMBOL_LE, val);
                                        break;
                                case '=':
                                        data = find_prefixed_type(file, '=', SYMBOL_ASSIGN, SYMBOL_EQ, val);
                                        break; 
                                default:
                                        if (is_alphabetic(val[0])){
                                                data = find_alphanumeric_value(file, val);
                                        } else if (is_numeric(val[0])){
                                                data = find_numeric_value(file, val);
                                        }
                        }

                }

                if ((data.type == SYMBOL_NULL) || (charlit_status == UNCLOSED_CHARLIT)){
                        free_list(list, free_token, token);
                        free(list);
                        return NULL;
                }

                if ((charlit_status == OPENED_CHARLIT) && (data.type != SYMBOL_SQUOTE)){
                        charlit_status = UNCLOSED_CHARLIT;
                }

                struct token *new_token = (struct token*) malloc(sizeof(struct token));

                new_token->value = (char*) malloc((data.val_len + 1) * sizeof(char));
                val[data.val_len] = 0;
                strcpy(new_token->value, val);
                new_token->type = data.type;
                append_list(list, new_token, token);

                val[0] = data.excess;
                data.excess = 0;
        }

        if (charlit_status != NO_CHARLIT){
                free_list(list, free_token, token);
                free(list);
                return NULL;
        }

        return list;
}


void free_token(struct token *t){
        free(t->value);
        free(t);
}
