#ifndef __INTERPRETOR_H__
#define __INTERPRETOR_H__

int find_start_blk() {
    for (int i = 0; i < blkSize; i++)
        if (b[i].type == START)
            return i;
    return -1;
}

float* get_reg(string name) {
    for (int i = 0; i < regSize; i++)
        if (name == r[i].name)
            return &r[i].val;
    return NULL;
}

void run_decision_blk(blk& bl) {

}

void run_expression_blk(blk& bl) {

}

string remove_spaces(string str) {
    string s = str;
    remove(s.begin(), s.end(), ' ');
    return s;
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

    r[regSize].name = instr;
    cout << instr << " <- "; cin >> r[regSize].val;
    regSize++;
}

void run_write_blk(blk& bl) {
    if (bl.container[0] == '"')
        for (int i = 1; i < bl.container.size() - 1; i++)
            cout << bl.container[i];
    else {
        string var_name = remove_spaces(bl.container);
        float* val = get_reg(var_name);
        if (val == NULL)
            cout << "'" << var_name << "' was not declared";
        else 
            cout << *val;
    }
    cout << '\n';
}
void runScheme() {
    for (int i = 0; i < blkSize; i++)
        if (b[i].type == READ)
            run_read_blk(b[i]);
        else if (b[i].type == WRITE)
            run_write_blk(b[i]);
}

#endif
