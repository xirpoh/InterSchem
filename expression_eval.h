#ifndef __EXPRESSION_EVAL_H__
#define __EXPRESSION_EVAL_H__

#include "tokenizer.h"

#define NUM_FUNC 30
#define NAME_LEN 15
#define EPS 1e-5
#define _ERROR -404

double flipSign(double x) { return -x; }
double keepSign(double x) { return x; }
double logicNot(double x) { return !((int)x != 0); }
double bitwiseNot(double x) { return ~(int)x; }
double incr(double x) { return x; };
double decr(double x) { return x; };

string func_names[NUM_FUNC + 1] = {"fabs", "exp", "log", "log2", "log10",
                                       "sqrt", "cbrt", "sin", "cos", "tan", 
                                       "asin", "acos", "atan", "sinh", 
                                       "cosh", "tanh", "asinh", "acosh", "atanh", 
                                       "ceil", "floor", "round", "trunc", "tgamma",
                                       "flipSign", "keepSign", "logicNot", "bitwiseNot", "incr", "decr"};

double (*p_func[NUM_FUNC + 1])(double) = {fabs, exp, log, log2, log10,
                                      sqrt, cbrt, sin, cos, tan, 
                                      asin, acos, atan, sinh,
                                      cosh, tanh, asinh, acosh, atanh,
                                      ceil, floor, round, trunc, tgamma, 
                                      flipSign, keepSign, logicNot, bitwiseNot, incr, decr};

float call_func(int func_idx, float arg) {
    return p_func[func_idx](arg);
}

int get_func_idx(string input_function) {
    for (int i = 0; i < NUM_FUNC; i++)
        if (input_function == func_names[i])
            return i; 
    cout << "<Unknown function typed>\n";
    return _ERROR;
}

float str_to_float(string str) {
    float num = 0, pow10 = 0.1;
    bool dot = 0;
    int len = str.size();
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
    TokenList pfix;
    int& sz = pfix.num_tokens;
    stack<Token> ts;
    
    //Identify unary operators
    for (int i = 0; i < ifix.num_tokens; i++) 
        if (ifix.token[i].id == OPRT && ifix.token[i].op_id < 5) {
                if (ifix.token[i].op_id == 1)
                    ifix.token[i].str = "incr";
                if (ifix.token[i].op_id == 2)
                    ifix.token[i].str = "decr";
                if (ifix.token[i].op_id == 3)
                    ifix.token[i].str = "logicNot";
                if (ifix.token[i].op_id == 4)
                    ifix.token[i].str = "bitwiseNot";

                ifix.token[i].id = FUNC;
                ifix.token[i].op_id = 0;
                ifix.token[i].len = ifix.token[i].str.size();
            }

    for (int i = 0; i < ifix.num_tokens; i++)
        if (ifix.token[i].id == OPRT) 
            if (i == 0 || ifix.token[i - 1].id == OPRT || ifix.token[i - 1].id == OPEN_BR) {
                if (ifix.token[i].op_id == 10)
                    ifix.token[i].str = "flipSign";
                if (ifix.token[i].op_id == 9)
                    ifix.token[i].str = "keepSign";

                ifix.token[i].id = FUNC;
                ifix.token[i].len = ifix.token[i].str.size();
            }
    //

    for (int i = 0; i < ifix.num_tokens; i++) {
        if (ifix.token[i].id == _CONST || ifix.token[i].id == VAR)
            pfix.token[sz++] = ifix.token[i];

        else if (ifix.token[i].id == FUNC || ifix.token[i].id == OPEN_BR) 
            ts.push(ifix.token[i]);

        else if (ifix.token[i].id == CLOSED_BR) {
            while ((ts.top()).id != OPEN_BR) 
                pfix.token[sz++] = ts.top(), ts.pop();
            ts.pop();
        }

        else if (ifix.token[i].id == OPRT) {
            if (ts.empty() || precedence[ifix.token[i].op_id] < precedence[ts.top().op_id])
                ts.push(ifix.token[i]);
            else {
                while (!ts.empty() && precedence[ifix.token[i].op_id] >= precedence[ts.top().op_id]) {
                    pfix.token[sz++] = ts.top();
                    ts.pop();
                }
                ts.push(ifix.token[i]);
            }
        }
    }

    while (!ts.empty()) {
        pfix.token[sz++] = ts.top();
        ts.pop();
    }
    
    ifix.num_tokens = pfix.num_tokens;
    for (int i = 0; i < pfix.num_tokens; i++)
        ifix.token[i] = pfix.token[i];
}

float evalPostfix(TokenList& tl) { 
    stack<float> values;

    for (int i = 0; i < tl.num_tokens; i++) {
        if (tl.token[i].id == _CONST)
            values.push(str_to_float(tl.token[i].str));
        else if (tl.token[i].id == VAR) {
            float* reg = get_reg(tl.token[i].str);
            if (reg == NULL) {
                cout << "error: " << tl.token[i].str << " was not declared";
                return _ERROR;
            }
            if (tl.token[i + 1].str == "incr")
                *reg += 1;
            if (tl.token[i + 1].str == "decr")
                *reg -= 1;
            values.push(*reg);
        }
        else if (tl.token[i].id == OPRT) {
            float val2 = values.top(); values.pop();
            float val1 = values.top(); values.pop();
            values.push(apply_operator(val1, val2, tl.token[i].op_id));
        }
        else if (tl.token[i].id == FUNC) {
            float val = values.top(); values.pop();
            int func_idx = get_func_idx(tl.token[i].str);
            if (func_idx != _ERROR)
                values.push(call_func(func_idx, val));
            else
                return _ERROR;
        }
    }

    return values.top();
}

float expression_eval(string expr) {
    TokenList tl;
    tokenize(expr, tl);
    //print_TokenList(tl);
    
    if (is_expr_valid(tl)) {
        convInfix2Postfix(tl);
        //print_TokenList(tl);
        return evalPostfix(tl);
    }

    return _ERROR;
}

#endif
