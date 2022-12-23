#ifndef __INTERPRETOR_H__
#define __INTERPRETOR_H__

#include <time.h>

float* get_reg(string name) {
    for (int i = 0; i < regSize; i++)
        if (name == r[i].name)
            return &r[i].val;
    return NULL;
}

int get_reg_idx(string name) {
    for (int i = 0; i < regSize; i++)
        if (name == r[i].name)
            return i;
    return -1;
}

#include "expression_eval.h"

string remove_spaces(string str) {
    string s = "";
    for (int i = 0; i < str.size(); i++)
        if (str[i] != ' ')
            s += str[i];
    return s;
}

string remove_leading_spaces(string& str) {
    int idx = 0;
    while (str[idx] == ' ') idx++;
    str = str.substr(idx, str.size() - idx);
}

int find_start_blk() {
    for (int i = 0; i < blkSize; i++)
        if (b[i].type == START)
            return i;
    return -1;
}

void run_start_blk(blk& bl) {
    cout << "START\n";
}

void run_stop_blk(blk& bl) {
    cout << "\nSTOP\n";
}

int run_decision_blk(blk& bl) {
    float result = expression_eval(bl.container);
    if (result != _ERROR)
        return result != 0;
    return _ERROR;
}

bool _run_expression(string instr) {
    int eq = instr.find('=');

    if (eq == -1) {
        float res = expression_eval(instr); 
        return 1;
    }
    
    string comp_op[11] = {"+", "-", "**", "*", "//" , "/", "%", ">>", "<<", "&", "|"};
    for (int i = 0; i < 11; i++) {
        int pos = instr.find(comp_op[i]);
        if (pos != -1 && pos < eq) {
            string var_name = instr.substr(0, pos);
            string _instr = instr.substr(pos + comp_op[i].size() + 1, instr.size() - pos - 1 + comp_op[i].size());
            //cout << var_name << " " << _instr << "\n";
            float *reg = get_reg(var_name);
            if (reg != NULL) {
                *reg = apply_operator(*reg, expression_eval(_instr), get_oprt_idx(comp_op[i]) + 1);
            }
            else {
                cout << var_name << ": undeclared variable";
                return 0;
            }
            return 1;
        }
    }

    string var_name = instr.substr(0, eq);
    instr = instr.substr(eq + 1, instr.size() - eq - 1);
    float *reg = get_reg(var_name);
    if (reg != NULL)
        *reg = expression_eval(instr);
    else { 
        r[regSize].name = var_name;
        r[regSize].val = expression_eval(instr);
        regSize++;
        //cout << var_name << ": undeclared variable";
    }

    return 1;
}

void run_expression_blk(blk& bl) {
    string cont = remove_spaces(bl.container);
    cont += ",";
    int separator = cont.find(',');
    
    while (separator != -1) {
        string instr = cont.substr(0, separator);
        
        if (!_run_expression(instr))
            return;

        cont = cont.substr(separator + 1, cont.size() - separator - 1);
        separator = cont.find(',');
    }
}

bool _run_read_reg(string instr) {
    if (!isalpha(instr[0]) && instr[0] != '_') {
        cout << "Variable names need to start with a letter or with an underscore\n";
        return 0;
    }
    for (int i = 0; i < instr.size(); i++)
        if (instr[i] != '_' && !isalpha(instr[i]) && !isdigit(instr[i])) {
            cout << "Variable names can only contain letters, digits or the underscore\n";
            return 0;
        }
    
    int reg_idx = get_reg_idx(instr);
    cout << instr << " <- "; 
    if (reg_idx != -1) {
        cin >> r[reg_idx].val;
    }
    else {
        r[regSize].name = instr;
        cin >> r[regSize].val;
        regSize++;
    }

    return 1;
}

void run_read_blk(blk& bl) {
    string cont = remove_spaces(bl.container);
    cont += ",";
    int separator = cont.find(',');
    
    while (separator != -1) {
        string instr = cont.substr(0, separator);
        
        if (!_run_read_reg(instr))
            return;

        cont = cont.substr(separator + 1, cont.size() - separator - 1);
        separator = cont.find(',');
    }
}

void print_qoute_string(string instr) {
    for (int i = 1; i < instr.size() - 1; i++) 
        if (instr[i] == '\\' && instr[i + 1] == 'n')
            cout << '\n', i++;
        else
            cout << instr[i];
}

void run_write_blk(blk& bl) {
    string cont = bl.container;
    cont += ",";
    int separator = cont.find(',');

    while (separator != -1) {
        string instr = cont.substr(0, separator);
        remove_leading_spaces(instr);
        if (instr[0] == '"')
            print_qoute_string(instr);
        else {
            float result = expression_eval(instr);
            if (result != _ERROR)
                cout << result;
        }
        cont = cont.substr(separator + 1, cont.size() - separator - 1);
        separator = cont.find(',');
    }
}

void drawNextCnnt(blk& b1, blk& b2, bool brnch = 0) {
    //drawaux(b1, b2, LIGHTMAGENTA, b1.type, b1.id, brnch);
    //delay(STEP_DELAY);
    //drawaux(b1, b2, BLK_STROKE, b1.type, b1.id, brnch);
    drawCnnt(CNNT_STROKE, b1.id, brnch, LIGHTMAGENTA);
    delay(STEP_DELAY);
    drawCnnt(CNNT_STROKE, b1.id, brnch, CNNT_STROKE);
}

void draw_regPanel(int color) {
    setcolor(color);
    int varX = WIDTH - 120, varY = 20;

    for (int i = 0; i < regSize; i++) {
        char* line = stringToChar(r[i].name);
        
        char val_buffer[20];
        sprintf(val_buffer, "%g", floor(100 * r[i].val) / 100);
        
        strcat(line, " <- ");
        strcat(line, val_buffer);
        strcat(line, "        ");
        outtextxy(varX, varY * (i + 1), line);
        free(line);
    }
}

char get_button_key(int mx, int my, bool exec = 0) {
    clearmouseclick(WM_LBUTTONDOWN);
    if (mx > WIDTH - intermW - filemenuW && mx < WIDTH &&
        my > HEIGHT - intermH && my < HEIGHT) {
        mx -= WIDTH - intermW - filemenuW;
        my -= HEIGHT - intermH;

        int dw = (intermW + 236) / 7;
        string keys = "NOSqnmf";
        int idx = floor((float)mx / dw);
        if (idx < 3 && exec)
            return '%';
        return keys[idx];
    }

    return '%';
}

void interbutton(const char* image_name) {
    readimagefile(image_name, WIDTH - intermW, HEIGHT - intermH, WIDTH, HEIGHT);
}

void wait_key(int& mode) {
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            char key = get_button_key(mousex(), mousey(), 1);
            if (key != '%') {
                if (key == 'm') {
                    mode = !mode;
                    if (mode) {
                        STEP_DELAY = SLOW_STEP;
                        interbutton("images\\PAUSE.jpg");
                        delay(ANIM_DELAY * 2);
                        interbutton("images\\IDLE.jpg");
                    }
                    else { 
                        STEP_DELAY = RUN_STEP;
                        interbutton("images\\RUN.jpg");
                    }
                }
                else if (key == 'n') {
                    interbutton("images\\STEP.jpg");
                    delay(ANIM_DELAY * 2);
                    interbutton("images\\IDLE.jpg");
                }
                if (key == 'q') {
                    interbutton("images\\STOP.jpg");
                    delay(ANIM_DELAY * 2);
                    interbutton("images\\IDLE.jpg");
                    mode = 2;
                }
                if (key == 'f') {
                    STEP_DELAY = FAST_STEP;
                    interbutton("images\\FAST.jpg");
                    mode = 0;
                }
                break;
            }
        }
    }
}

void reset_reg_oprt() {
    for (int i = 0; i < regSize; i++)
        apply_post_op[i] = 0;
}

void apply_reg_oprt() {
    for (int i = 0; i < regSize; i++)
        r[i].val += apply_post_op[i];
}

void runInterpretor(int _mode) {
    while (!stackA.empty()) stackA.pop();
    while (!stackB.empty()) stackB.pop();

    srand(time(0));
    draw_regPanel(BG);
    if (_mode)
        interbutton("images\\STEP.jpg"), STEP_DELAY = SLOW_STEP;
    else 
        interbutton("images\\RUN.jpg"), STEP_DELAY = RUN_STEP; 
    regSize = 0;
    int mode = _mode;
    int curr_blk = find_start_blk();
    if (curr_blk == -1) {
        cout << "<START block is missing>\n";
        return;
    }
        
    blk cblk = b[curr_blk]; 
    bool done = 0;
    while (!done && mode < 2) {

        reset_reg_oprt();

        if (ismouseclick(WM_LBUTTONDOWN)) {
            char key = get_button_key(mousex(), mousey(), 1);
            if (key == 'q') {
                interbutton("images\\STOP.jpg");
                delay(ANIM_DELAY * 2);
                interbutton("images\\IDLE.jpg");
                done = 1;
                continue;
            }
            if (mode && key == 'm' || !mode && key == 'n') {
                mode = !mode;
                if (mode) {
                    STEP_DELAY = SLOW_STEP;
                    interbutton("images\\PAUSE.jpg");
                    delay(ANIM_DELAY * 2);
                    interbutton("images\\IDLE.jpg");
                }
                else { 
                    STEP_DELAY = RUN_STEP;
                    interbutton("images\\RUN.jpg");
                }
            }
            if (key == 'f') {
                STEP_DELAY = FAST_STEP;
                interbutton("images\\FAST.jpg");
                mode = 0;
            }
        }
        
        clearBlk(cblk);
        cblk.color = LIGHTGREEN;
        drawBlk(cblk);
        draw_regPanel(WHITE);
        delay(STEP_DELAY);
        
        if(cblk.type == START) {
            run_start_blk(cblk);
        }
        
        if (cblk.type == STOP) {
            run_stop_blk(cblk);
            deselectBlk(cblk);
            drawBlk(cblk);
            done = 1;
        }
        
        if (cblk.type == EXPR) {
            run_expression_blk(cblk);
        }
        
        if (cblk.type == READ) {
            run_read_blk(cblk);
        }
        
        if (cblk.type == WRITE) {
            run_write_blk(cblk);
        }
        
        if (cblk.type != DECISION && cblk.type != STOP) {
            if (mode) wait_key(mode);
            deselectBlk(cblk);
            drawBlk(cblk);
        }
        
        if (cblk.type == DECISION) {
            int isTrue = run_decision_blk(cblk);
            
            if (mode) wait_key(mode);
            deselectBlk(cblk);
            drawBlk(cblk);
            
            if (isTrue == _ERROR) { 
                done = 1;
                break;
            }

            if (isTrue != 0) {
                if (cblk.next != -1 && mode < 2) {
                    drawNextCnnt(cblk, b[cblk.next]);
                    cblk = b[cblk.next];
                }
            }
            else {
                if (cblk.nextF != -1 && mode < 2) {
                    drawNextCnnt(cblk, b[cblk.nextF], 1);
                    cblk = b[cblk.nextF];
                }
            }
        }
        else if (cblk.next != -1 && mode < 2) { 
            drawNextCnnt(cblk, b[cblk.next]);
            cblk = b[cblk.next];
        }
        
        apply_reg_oprt();
        if (!done)
            draw_regPanel(BG);
    }

    cout << "\n";
    interbutton("images\\IDLE.jpg");
}
    
bool interpretor_menu() {
    char key = get_button_key(mousex(), mousey());
    if (key != '%')
        deselectBlks(); drawBlks();


    switch (key) {
        case 'N':
            resetScheme();
            return 1;
        case 'O':
            import_scheme();
            return 1;
        case 'S':
            export_scheme();
            return 1;
        case 'n':
            runInterpretor(1);
            return 1;
        case 'm':
            runInterpretor(0);
            return 1;
        default:
            return 0;
    }

    /*if (key == 'n') {
        runInterpretor(1);
        return 1;
    }
    if (key == 'm') {
        runInterpretor(0);
        return 1;
    }
    return 0;*/
}

#endif

