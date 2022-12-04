#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#define TOKEN_LEN 20
#define MX_TOKEN 100
#define NUM_TOKEN 6
#define NUM_OPRT 24
#define FUNC_OP 25

enum TOKEN{OPEN_BR, CLOSED_BR, _CONST, OPRT, FUNC, VAR};

string token_names[NUM_TOKEN] = {"OPEN_BR", "CLOSED_BR", "CONST", "OPRT", "FUNC", "VAR"};
string oprt_sym = "+-*/^><=&|%!~";
string oprt_syg[NUM_OPRT + 1] = {"++", "--", "!", "~", "**", "*", "/", "%", "+", "-",
                              ">>", "<<", "<", "<=", ">", ">=", "==", "!=", "&",
                              "^", "|", "&&", "||", "//"};

const int precedence[NUM_OPRT + 2] = {0, 1, 1, 2, 2, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 8, 8, 9, 10, 11, 12, 13, 4, 15};

//0 -> functions
//15 -> paranthesis

int get_oprt_idx(string input_oprt) {
    for (int i = 0; i < NUM_OPRT; i++)
        if (input_oprt == oprt_syg[i]) {
            return i; 
        }
    return -1;
}

struct Token {
    int id, len, op_id;

    Token() {
        op_id = 0;
    }

    string str;
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

void print_token(Token& t) {
    cout << "['" << token_names[t.id] << "', '" << t.str << "']\n";
}

void print_TokenList(TokenList& tl) {
    for (int i = 0; i < tl.num_tokens; i++)
        print_token(tl.token[i]);
    cout << "\n";
}

void tokenize(string expr, TokenList& tl) {
    int len = expr.size();

    for (int i = 0; i < len; i++) {
        if (expr[i] == '(' || expr[i] == ')') {
            tl.token[tl.num_tokens].id = (expr[i] == '(') ? OPEN_BR : CLOSED_BR;
            tl.token[tl.num_tokens].len = 1;
            tl.token[tl.num_tokens].op_id = NUM_OPRT + 2;
            tl.token[tl.num_tokens].str = expr[i];
            tl.num_tokens++;
        }
        else if (isdigit(expr[i]) || expr[i] == '.') {
            int j = i, len = 0;
            while (isdigit(expr[i]) || expr[i] == '.')
                i++, len++;
            
            tl.token[tl.num_tokens].id = _CONST;
            tl.token[tl.num_tokens].len = len;
            tl.token[tl.num_tokens].str = expr.substr(j, len);
            tl.num_tokens++;
            --i;
        }
        else if (isalpha(expr[i])) {
            int j = i, len = 0;
            while (isalpha(expr[i]) || isdigit(expr[i]))
                i++, len++;

            tl.token[tl.num_tokens].id = (expr[i] == '(') ? FUNC : VAR;
            tl.token[tl.num_tokens].len = len;
            tl.token[tl.num_tokens].str = expr.substr(j, len);
            tl.num_tokens++;
            --i;
        }
        else if(oprt_sym.find(expr[i]) != -1) {
            string input_oprt = ""; input_oprt += expr[i];
            if (oprt_sym.find(expr[i + 1]) != -1) {
                int oprt_idx = get_oprt_idx(input_oprt + expr[i + 1]);
                if (oprt_idx != -1)
                    input_oprt += expr[i + 1], i++;
            }
            
            int oprt_idx = get_oprt_idx(input_oprt);
            if (oprt_idx >= 0) {
                tl.token[tl.num_tokens].id = OPRT;
                tl.token[tl.num_tokens].len = input_oprt.size();
                tl.token[tl.num_tokens].op_id = oprt_idx + 1;
                tl.token[tl.num_tokens].str = input_oprt;
            }
            tl.num_tokens++;
        }
    }
}

bool is_const_token_valid(Token token) {
    int dot = 0;
    for (int i = 0; i < token.len; i++)
        if (token.str[i] == '.')
            dot++;
    return dot <= 1;
}

bool is_incr_op(Token t) {
    return t.op_id == 1 || t.op_id == 2;
}

bool is_unar_op(char op) {
    return op == '-'  || op == '!' || op == '~';
}

int _is_expr_valid(TokenList tl) {
    int err = 0, errIdx = -1;

    if (!tl.num_tokens)
        return 0;
    
    if (tl.token[tl.num_tokens - 1].id == OPRT && 
        tl.token[tl.num_tokens - 1].op_id != 1 && tl.token[tl.num_tokens - 1].op_id != 2)
        err = 3, errIdx = tl.num_tokens - 1;

    int openbr = 0, closedbr = 0;
    for (int i = 0; i < tl.num_tokens && !err; i++) {
        if (tl.token[i].id == _CONST && !is_const_token_valid(tl.token[i]))
            err = 9, errIdx = i;;

        if (tl.token[i].id == OPEN_BR)
            openbr++;
        else if (tl.token[i].id == CLOSED_BR)
            closedbr++;
        if (openbr < closedbr)
            err = 2;
    }
    if (openbr != closedbr)
        err = 1;

    for (int i = 0; i < tl.num_tokens && !err; i++) {
        //if (i > 0 && is_incr_op(token[i]) && tl.token[i - 1].id != VAR)
        //    err = 3, errIdx = i;
        
        if (i >= tl.num_tokens - 1)
            break;

        if (is_incr_op(tl.token[i]) && tl.token[i + 1].id == CLOSED_BR)
            continue;
        
        //if (is_incr_op(tl.token[i]) && (tl.token[i + 1].id != VAR && tl.token[i + 1].id != OPRT))
        //    err = 3, errIdx = i;

        //if (tl.token[i].id == OPRT && !is_incr_op(tl.token[i]) && (tl.token[i + 1].id == OPRT && 
        //    !is_unar_op(tl.token[i + 1].str[0])) || tl.token[i + 1].id == CLOSED_BR)
        //    err = 3, errIdx = i;

        if (tl.token[i].id == VAR && tl.token[i + 1].id != OPRT && tl.token[i + 1].id != CLOSED_BR)
            err = 4, errIdx = i;

        if (tl.token[i].id == _CONST && tl.token[i + 1].id != OPRT && tl.token[i + 1].id != CLOSED_BR)
            err = 5, errIdx = i;

        if (tl.token[i].id == FUNC && tl.token[i + 1].id != OPEN_BR)
            err = 6, errIdx = i;

        if (tl.token[i].id == OPEN_BR && (tl.token[i].str[0] == '*' || tl.token[i].str[0] == '/'))
            err = 7, errIdx = i;

        if (tl.token[i].id == CLOSED_BR && tl.token[i + 1].id != CLOSED_BR && tl.token[i + 1].id != OPRT)
            err = 8, errIdx = i;;
    }
    
    if (err) {
        if (errIdx != -1)
            print_token(tl.token[errIdx]);
        return err;
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
    "Dupa un nume de functie poate urma doar o paranteza deschisa (().",
    "Dupa paranteză deschisa (()) poate urma o alta paranteza deschisa, un nume de functie, o variabila, o constanta sau chiar operatorul unar + sau -.",
    "Dupa paranteza inchisa ())) poate urma o alta paranteza inchisa sau un operator.",
    "Numar real scris gresit. Un numar real scris corect contine un singur punct; poate incepe direct cu punctul."
};

bool is_expr_valid(TokenList tl) {
    if (int e = _is_expr_valid(tl)) {
        cout << "<Invalid expression>\n" << syntax_rule[e] << "\n";
        return 0;
    }
    return 1;
}

#endif
