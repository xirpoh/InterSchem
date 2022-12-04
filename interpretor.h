#ifndef __INTERPRETOR_H__
#define __INTERPRETOR_H__

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
    string s = str;
    remove(s.begin(), s.end(), ' ');
    return s;
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
    cout << "STOP\n";
}

int run_decision_blk(blk& bl) {
    float result = expression_eval(bl.container);
    if (result != _ERROR)
        return result != 0;
    return _ERROR;
}

void run_expression_blk(blk& bl) {
    string instr = remove_spaces(bl.container);
    //cout << instr << "\n";
    int eq = instr.find('=');
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
}

void run_read_blk(blk& bl) {
    string instr = remove_spaces(bl.container);
    if (!isalpha(instr[0])) {
        cout << "Variable names need to start with with letters\n";
        return;
    }
    for (int i = 0; i < instr.size(); i++)
        if (!isalpha(instr[i]) && !isdigit(instr[i])) {
            cout << "Variable names can only contain letters and digits\n";
            return;
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
}

void run_write_blk(blk& bl) {
    if (bl.container[0] == '"')
        for (int i = 1; i < bl.container.size() - 1; i++)
            cout << bl.container[i];
    else {
        //string var_name = remove_spaces(bl.container);
        //cout << *get_reg(var_name);
        float result = expression_eval(bl.container);
        //float result = _ERROR;
        if (result != _ERROR)
            cout << result;
    }
    cout << '\n';
}

void drawNextCnnt(blk& b1, blk& b2, bool brnch = 0) {
    drawaux(b1, b2, LIGHTMAGENTA, b1.type, brnch);
    delay(STEP_DELAY);
    drawaux(b1, b2, BLK_STROKE, b1.type, brnch);
}

void wait_key(int& mode) {
    while (1) {
        if (kbhit()) {
            char key = getch();
            if (key == 'n' || key == 'm') {
                if (key == 'm')
                    mode = !mode;
                break;
            }
        }
    }
}

void runInterpretor() {
    regSize = 0;
    int mode = 1;
    int curr_blk = find_start_blk();
    if (curr_blk == -1) {
        cout << "<START block is missing>\n";
        return;
    }
        
    blk cblk = b[curr_blk]; 
    bool done = 0;
    while (!done) {
        if (kbhit()) {
            char key = getch();
            if (key == 'q') {
                done = 1;
                continue;
            }
            if (key == 'm')
                mode = !mode;
        }
        
        clearBlk(cblk);
        cblk.color = LIGHTGREEN;
        drawBlk(cblk);
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
            if (isTrue == _ERROR) { 
                done = 1;
                break;
            }
            
            if (mode) wait_key(mode);
            deselectBlk(cblk);
            drawBlk(cblk);

            if (isTrue != 0) {
                if (cblk.next != -1) {
                    drawNextCnnt(cblk, b[cblk.next]);
                    cblk = b[cblk.next];
                }
            }
            else {
                if (cblk.nextF != -1) {
                    drawNextCnnt(cblk, b[cblk.nextF], 1);
                    cblk = b[cblk.nextF];
                }
            }
        }
        else if (cblk.next != -1) { 
            drawNextCnnt(cblk, b[cblk.next]);
            cblk = b[cblk.next];
        }
        
    }

    cout << "\n";
}
    
bool iskeypressed() {
}

#endif

