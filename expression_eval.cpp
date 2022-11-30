#include <iostream>
#include <math.h>
#include <string.h>
#include <stack>
#include <stdio.h>
#include "tokenizer.cpp"
using namespace std;

#define NUM_FUNC 30
#define NAME_LEN 15
#define EPS 1e-5
#define ERROR 17

double flipSign(double x) { return -x; }
double keepSign(double x) { return x; }
double logicNot(double x) { return !((int)x != 0); }
double bitwiseNot(double x) { return ~(int)x; }
double incr(double x) { return ++x; };
double decr(double x) { return --x; };

char func_names[NUM_FUNC][NAME_LEN] = {"fabs", "exp", "log", "log2", "log10",
                                       "sqrt", "cbrt", "sin", "cos", "tan", 
                                       "asin", "acos", "atan", "sinh", 
                                       "cosh", "tanh", "asinh", "acosh", "atanh", 
                                       "ceil", "floor", "round", "trunc", "tgamma",
                                       "flipSign", "keepSign", "logicNot", "bitwiseNot", "incr", "decr"};

double (*p_func[NUM_FUNC])(double) = {fabs, exp, log, log2, log10,
                                      sqrt, cbrt, sin, cos, tan, 
                                      asin, acos, atan, sinh,
                                      cosh, tanh, asinh, acosh, atanh,
                                      ceil, floor, round, trunc, tgamma, 
                                      flipSign, keepSign, logicNot, bitwiseNot, incr, decr};

float call_func(int func_idx, float arg) {
    return p_func[func_idx](arg);
}

int get_func_idx(char* input_function) {
    for (int i = 0; i < NUM_FUNC; i++)
        if (strcmp(input_function, func_names[i]) == 0)
            return i; 
    cerr << "<Unknown function typed>\n";
}

float str_to_float(char* str) {
    float num = 0, pow10 = 0.1;
    bool dot = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) 
        if (str[i] == '.') 
            dot = 1;
        else if (dot) {
            num += pow10 * (str[i] - '0');
            pow10 /= 10;
        }
        else 
            num = num * 10 + (str[i] - '0');
    
    return num;
}

bool equal(float a, float b) {
    return fabs(a - b) < EPS;
}

float apply_operator(float a, float b, int idx) {
    switch (idx) {
      //case  1: return (int)a + 1;
      //case  2: return (int)a - 1;
      //case  3: return !((int)a != 0);
      //case  4: return ~(int)a; 
        case  5: return pow(a, b);
        case  6: return a * b;
        case  7: return a / b;
        case  8: return (int)a % (int)b;
        case  9: return a + b;
        case 10: return a - b;
        case 11: return (int)a >> (int)b;
        case 12: return (int)a << (int)b;
        case 13: return a - b < 0;
        case 14: return (a - b < 0 || equal(a, b));
        case 15: return a - b > 0;
        case 16: return (a - b > 0 || equal(a, b));
        case 17: return equal(a, b);
        case 18: return !equal(a, b);
        case 19: return (int)a & (int)b;
        case 20: return (int)a ^ (int)b;
        case 21: return (int)a | (int)b;
        case 22: return a != 0 && b != 0;
        case 23: return a != 0 || b != 0;
        case 24: return (int)a / (int)b;
    }
}

void convInfix2Postfix(TokenList& ifix) {
    Token* token = ifix.token; 
    
    TokenList pfix;
    int& sz = pfix.num_tokens;
    stack<Token> ts;
    
    //a solution for unary operators
    for (int i = 0; i < ifix.num_tokens; i++)
        if (token[i].id == OPRT) 
            if (i == 0 || token[i + 1].id == OPRT || token[i - 1].id == OPEN_BR) {
                if (token[i].op_id == 10)
                    strcpy(token[i].str, "flipSign");
                if (token[i].op_id == 9)
                    strcpy(token[i].str, "keepSign");
                if (token[i].op_id == 1)
                    strcpy(token[i].str, "incr");
                if (token[i].op_id == 2)
                    strcpy(token[i].str, "decr");
                if (token[i].op_id == 3)
                    strcpy(token[i].str, "logicNot");
                if (token[i].op_id == 4)
                    strcpy(token[i].str, "bitwiseNot");
                if (token[i].op_id == 4)
                    strcpy(token[i].str, "bitwiseNot");
                
                token[i].id = FUNC;
                token[i].len = strlen(token[i].str);
                token[i].op_id = 0;
                if (token[i + 1].id == OPRT)
                    i++;
            }

    for (int i = 0; i < ifix.num_tokens; i++) {
        if (token[i].id == CONST || token[i].id == VAR)
            pfix.token[sz++] = token[i];

        else if (token[i].id == FUNC || token[i].id == OPEN_BR) 
            ts.push(token[i]);

        else if (token[i].id == CLOSED_BR) {
            while ((ts.top()).id != OPEN_BR) 
                pfix.token[sz++] = ts.top(), ts.pop();
            ts.pop();
        }

        else if (token[i].id == OPRT) {
            if (ts.empty() || precedence[token[i].op_id] < precedence[ts.top().op_id])
                ts.push(token[i]);
            else {
                while (!ts.empty() && precedence[token[i].op_id] >= precedence[ts.top().op_id]) {
                    pfix.token[sz++] = ts.top();
                    ts.pop();
                }
                ts.push(token[i]);
            }
        }
    }

    while (!ts.empty()) {
        pfix.token[sz++] = ts.top();
        ts.pop();
    }

    ifix = pfix;
}

float evalPostfix(TokenList& tl) { 
    stack<float> values;
    Token* token = tl.token;

    for (int i = 0; i < tl.num_tokens; i++) {
        if (token[i].id == CONST)
            values.push(str_to_float(token[i].str));
        else if (token[i].id == VAR)
            values.push(0);
        else if (token[i].id == OPRT) {
            float val2 = values.top(); values.pop();
            float val1 = values.top(); values.pop();
            values.push(apply_operator(val1, val2, token[i].op_id));
        }
        else if (token[i].id == FUNC) {
            float val = values.top(); values.pop();
            values.push(call_func(get_func_idx(token[i].str), val));
        }
    }

    return values.top();
}

float expression_eval(char* expr) {
    TokenList tl;
    tokenize(expr, tl);
    
    if (is_expr_valid(tl)) {
        print_TokenList(tl);
        convInfix2Postfix(tl);
        print_TokenList(tl);
        return evalPostfix(tl);
    }

    return ERROR;
}
