#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

#define TOKEN_LEN 20
#define MX_TOKEN 100
#define NUM_TOKEN 6
#define NUM_OPRT 24
#define FUNC_OP 25

enum TOKEN{OPEN_BR, CLOSED_BR, CONST, OPRT, FUNC, VAR};

const char token_names[NUM_TOKEN][TOKEN_LEN] = {"OPEN_BR", "CLOSED_BR", "CONST", "OPRT", "FUNC", "VAR"};
const char oprt_sym[14] = "+-*/^><=&|%!~";

char oprt_syg[NUM_OPRT][3] = {"++", "--", "!", "~", "**", "*", "/", "%", "+", "-",
                              ">>", "<<", "<", "<=", ">", ">=", "==", "!=", "&",
                              "^", "|", "&&", "||", "//"};

const int precedence[NUM_OPRT + 2] = {0, 1, 1, 2, 2, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 8, 8, 9, 10, 11, 12, 13, 4, 15};

//0 -> functions
//15 -> paranthesis

int get_oprt_idx(char* input_oprt) {
    for (int i = 0; i < NUM_OPRT; i++)
        if (strcmp(input_oprt, oprt_syg[i]) == 0)
            return i; 
    return -1;
}

struct Token {
    int id, len, op_id;

    Token() {
        op_id = 0;
    }

    char str[TOKEN_LEN];
};

struct TokenList {
    int num_tokens;

    TokenList() {
        num_tokens = 0;
    }

    Token token[MX_TOKEN];
};

void strncpy2(char* dest, char* src, int len) {
    for (int i = 0; i < len; i++)
        *(dest + i) = *(src + i);
    *(dest + len) = '\0';
}

void tokenize(char* expr, TokenList& tl) {
    Token* token = tl.token; 
    int& num_tokens = tl.num_tokens;
    int len = strlen(expr);

    for (int i = 0; i < len; i++) {
        if (expr[i] == '(' || expr[i] == ')') {
            token[num_tokens].id = (expr[i] == '(') ? OPEN_BR : CLOSED_BR;
            token[num_tokens].len = 1;
            token[num_tokens].op_id = NUM_OPRT + 2;
            strncpy2(token[num_tokens].str, expr + i, 1);
            num_tokens++;
        }
        else if (isdigit(expr[i]) || expr[i] == '.') {
            int j = i, len = 0;
            while (isdigit(expr[i]) || expr[i] == '.')
                i++, len++;
            
            token[num_tokens].id = CONST;
            token[num_tokens].len = len;
            strncpy2(token[num_tokens].str, expr + j, len);
            num_tokens++;
            --i;
        }
        else if (isalpha(expr[i])) {
            int j = i, len = 0;
            while (isalpha(expr[i]) || isdigit(expr[i]))
                i++, len++;

            token[num_tokens].id = (expr[i] == '(') ? FUNC : VAR;
            token[num_tokens].len = len;
            strncpy2(token[num_tokens].str, expr + j, len);
            num_tokens++;
            --i;
        }
        else if(strchr(oprt_sym, expr[i])) {
            char input_oprt[3];
            input_oprt[0] = expr[i];
            if (!strchr(oprt_sym, expr[i + 1]))
                input_oprt[1] = '\0';
            else {
                input_oprt[1] = expr[i + 1];
                i++; 
            }
            
            int oprt_idx = get_oprt_idx(input_oprt);
            if (oprt_idx >= 0) {
                token[num_tokens].id = OPRT;
                token[num_tokens].len = strlen(input_oprt);
                token[num_tokens].op_id = oprt_idx + 1;
                strncpy2(token[num_tokens].str, input_oprt, strlen(input_oprt));
            }
            num_tokens++;
        }
    }
}

void print_TokenList(TokenList& tl) {
    for (int i = 0; i < tl.num_tokens; i++)
        cout << "['" << token_names[tl.token[i].id] << "', '" << tl.token[i].str << "']\n";
    cout << "\n";
}

bool is_const_token_valid(Token token) {
    int dot = 0;
    for (int i = 0; i < token.len; i++)
        if (token.str[i] == '.')
            dot++;
    return dot <= 1;
}

bool is_incr_op(Token& t) {
    return t.op_id == 1 || t.op_id == 2;
}

int _is_expr_valid(TokenList& tl) {
    Token* token = tl.token; 
    int& num_tokens = tl.num_tokens;
    
    if (!num_tokens)
        return 0;
    
    if (token[num_tokens - 1].id == OPRT && 
        token[num_tokens - 1].op_id != 1 && token[num_tokens - 1].op_id != 2)
        return 3;

    int openbr = 0, closedbr = 0;
    for (int i = 0; i < num_tokens; i++) {
        if (token[i].id == CONST && !is_const_token_valid(token[i]))
            return 9;

        if (token[i].id == OPEN_BR)
            openbr++;
        else if (token[i].id == CLOSED_BR)
            closedbr++;
        if (openbr < closedbr)
            return 2;
    }
    if (openbr != closedbr)
        return 1;

    for (int i = 0; i < num_tokens; i++) {
        if (i > 0 && is_incr_op(token[i]) && token[i - 1].id != VAR)
            return 3;
        
        if (i >= num_tokens - 1)
            break;

        if (is_incr_op(token[i]) && (token[i + 1].id != VAR && token[i + 1].id != OPRT))
            return 3;

        if (token[i].id == OPRT && !is_incr_op(token[i]) && (token[i + 1].id == OPRT || token[i + 1].id == CLOSED_BR))
            return 3;

        if (token[i].id == VAR && token[i + 1].id != OPRT && token[i + 1].id != CLOSED_BR)
            return 4;

        if (token[i].id == CONST && token[i + 1].id != OPRT && token[i + 1].id != CLOSED_BR)
            return 5;
        
        if (token[i].id == FUNC && token[i + 1].id != OPEN_BR)
            return 6;

        if (token[i].id == OPEN_BR && (token[i].str[0] == '*' || token[i].str[0] == '/'))
            return 7;
        
        if (token[i].id == CLOSED_BR && token[i + 1].id != CLOSED_BR && token[i + 1].id != OPRT)
            return 8;
    }

    return 0;
}

const char syntax_rule[10][200] = {
    "",
    "Numarul de paranteze inchise este egal cu numarul de paranteze deschise.",
    "La o parcurgere a sirului, de la stanga la dreapta, pana la orice pozitie trebuie ca numarul de paranteze deschise sa fie mai mare sau egal cu cel al parantezelor inchise.",
    "Dupa un operator poate urma o variabila (deci o litera), o functie (deci sin/cos/ln etc.), o constanta (deci o cifra) sau o paranteza deschisa (().",
    "Dupa o variabila poate urma un operator sau o paranteza inchisa ()).",
    "Dupa o constanta poate urma un operator sau o paranteza inchisa ()).",
    "Dupa un nume de functie poate urma doar o paranteză deschisa (().",
    "Dupa paranteză deschisă (()) poate urma o alta paranteza deschisa, un nume de functie, o variabila, o constanta sau chiar operatorul unar + sau -.",
    "Dupa paranteza inchisa ())) poate urma o alta paranteza închisa sau un operator.",
    "Numar real scris gresit. Un numar real scris corect contine un singur punct; poate incepe direct cu punctul."
};

bool is_expr_valid(TokenList& tl) {
    if (int e = _is_expr_valid(tl)) {
        cerr << "<Invalid expression>\nExpresia nu respecta regula:\n" << syntax_rule[e] << "\n";
        return 0;
    }
    return 1;
}
