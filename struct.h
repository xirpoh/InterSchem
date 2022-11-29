#ifndef __STRUCT_H__
#define __STRUCT_H__

struct blk {
    int x, y;
    int w, h;
    char type, color;
    string container;

    int next, nextF;
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

//cnnt c[mxCNT];
//int cnntSize = 0;

reg r[mxREG];
int regSize = 0;

#endif
