#ifndef __STRUCT_H__
#define __STRUCT_H__

struct blk {
    int x, y;
    int w, h;
    char type;
    int color;

    string container;

    int next, nextF;
    int id;
};

struct ifentry {
    int id;
    bool branch;
};

struct whileentry {
    int beginning;
    bool branch;
    vector<ifentry> path;
};

struct usedentry {
    int id;
    bool branch;
}; 

struct reg {
    string name;
    float val;
};

blk b[mxBLK];
int blkSize = 0;

vector<whileentry> wEntries;
int wSize = 0;

vector<usedentry> uEntries;
int usize = 0;

bool vizitat[mxBLK*2] = { 0 };
int vizitatfrom[mxBLK*2] = { -1 };

int drawntofr[mxBLK] = { 0 };
int drawntofl[mxBLK] = { 0 };
bool ignorewhendrawingines[mxBLK] = { 0 };

reg r[mxREG];
int regSize = 0;

int apply_post_op[mxREG];

#endif
