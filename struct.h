#ifndef __STRUCT_H__
#define __STRUCT_H__
struct blk {

    int x, y;
    int w, h;
    char type, color;
    string container;

    int next, nextF;
};
struct ifentry {
    int id;
    bool branch;
};
struct whileentry {
    int beginning;
    vector<ifentry> path;
};
struct usedentry {
    int id;
    bool branch;
};
//struct cnnt {
//    int blkA, blkB;
//};

struct reg {
    string var;
    float val;
};

blk b[mxBLK];
int blkSize = 0;

vector<whileentry> wEntries;
int wSize = 0;

vector<usedentry> uEntries;
int usize = 0;

bool vizitat[mxBLK]={0};
//cnnt c[mxCNT];
//int cnntSize = 0;

reg r[mxREG];
int regSize = 0;

#endif
