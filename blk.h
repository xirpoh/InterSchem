#ifndef __BLK_H__
#define __BLK_H__

#include <stdlib.h>

blk newBlk(int x, int y, int w, int h, char type) {
    blk b = {x, y, w, h, type, BLK_STROKE};
    return b;
}

void createBlk(char type, int x, int y) {
    blk& bl = b[blkSize] = newBlk(x - blkW / 2, y - blkH / 2, blkW, blkH, type);
    if (type == START)
        bl.container = "START";
    else if (type == STOP)
        bl.container = "STOP";
    /*else if (type == DECISION)
        bl.container = "IF";
    else if(type == EXPR) 
        bl.container = "EXPR";
    else if(type == READ) 
        bl.container = "READ";
    else if(type == WRITE) 
        bl.container = "WRITE";
    */
    blkSize++;
}

void blkconn(int &x, int &y, blk b, int br = 0) {
    if (br == 2) {
        x = b.x + b.w / 2;
        y = b.y;
    }
    else {
        if (b.type == DECISION) {
            x = b.x + (br == 1) * b.w;
            y = b.y + b.h / 2;
        }
        else {
            x = b.x + b.w / 2;
            y = b.y + b.h;
        }
    }
}

void fillcircle(int x, int y, int radius, int fill) {
    setfillstyle(SOLID_FILL, fill);
    fillellipse(x, y, radius, radius);
    circle(x, y, radius);
}

char* stringToChar(string s) {
    char* c = (char*)malloc(s.size() + 1);
    int i = 0;
    for (; i < s.size(); i++)
        c[i] = s[i];
    c[i] = '\0';
    return c;
}

void roundedRect(int x1, int y1, int x2, int y2) {
    line(x1 + EDGE_R, y1, x2 - EDGE_R, y1);
    line(x2, y1 + EDGE_R, x2, y2 - EDGE_R);
    line(x1 + EDGE_R, y2, x2 - EDGE_R, y2);
    line(x1, y1 + EDGE_R, x1, y2 - EDGE_R);

    arc(x1 + EDGE_R, y1 + EDGE_R, 90, 180, EDGE_R);
    arc(x2 - EDGE_R, y1 + EDGE_R, 0, 90, EDGE_R);
    arc(x2 - EDGE_R, y2 - EDGE_R, 270, 0, EDGE_R);
    arc(x1 + EDGE_R, y2 - EDGE_R, 180, 270, EDGE_R);
}

void _drawBlk(blk b) {

    char* txt = stringToChar(b.container);

    if (b.type == START) {
        //rectangle(b.x, b.y, b.x + b.w, b.y + b.h);
        roundedRect(b.x, b.y, b.x + b.w, b.y + b.h);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BLACK);
    }
    else if (b.type == STOP) {
        //rectangle(b.x, b.y, b.x + b.w, b.y + b.h);
        roundedRect(b.x, b.y, b.x + b.w, b.y + b.h);
    }
    else if (b.type == DECISION) {  

        int cx = b.x + b.w / 2, cy = b.y + b.h,
            cx1 = b.x + b.w, cy1 = b.y + b.h / 2;
        
        line(cx, b.y, cx1, cy1 - CNNT_R);    
        line(cx1, cy1 + CNNT_R, cx, cy);
        line(cx, cy, b.x, cy1 + CNNT_R);
        line(b.x, cy1 - CNNT_R, cx, b.y);

        fillcircle(b.x, b.y + b.h / 2, CNNT_R, BLACK);
        fillcircle(b.x + b.w, b.y + b.h / 2, CNNT_R, BLACK);

        outtextxy(b.x - 4, cy1 - 25, (char*)"T");
        outtextxy(cx1 - 4, cy1 - 25, (char*)"F");
    }
    else if (b.type == EXPR) {
        //rectangle(b.x, b.y, b.x + b.w, b.y + b.h);
        roundedRect(b.x, b.y, b.x + b.w, b.y + b.h);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BLACK);
    }
    else if (b.type == READ) {
        line(b.x + BOUND, b.y, b.x + b.w - BOUND, b.y);
        line(b.x + b.w - BOUND, b.y, b.x + b.w, b.y + b.h);
        line(b.x + b.w, b.y + b.h, b.x, b.y + b.h);
        line(b.x, b.y + b.h, b.x + BOUND, b.y);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BLACK);
    }
    else if (b.type == WRITE) {
        line(b.x, b.y, b.x + b.w, b.y);
        line(b.x + b.w, b.y, b.x + b.w - BOUND, b.y + b.h);
        line(b.x + b.w - BOUND, b.y + b.h, b.x + BOUND, b.y + b.h);
        line(b.x + BOUND, b.y + b.h, b.x, b.y);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BLACK);
    }

    outtextxy(b.x + b.w / 2 - textwidth(txt) / 2, b.y + b.h / 2 - textheight(txt) / 2, txt);

    free(txt);
}

void drawBlk(blk b) {
    setcolor(b.color);
    _drawBlk(b);
}

void clearBlk(blk b) {
    setcolor(0);
    _drawBlk(b);
}
int cadran(blk& b1, blk& b2) {
    
    int cx1, cy1, cx2, cy2, coltx1, coltx2;
    if ((b1.type == START || b1.type == EXPR || b1.type == READ || b1.type == WRITE)) {

        cx1 = b1.x + b1.w / 2;
        cy1 = b1.y + b1.h;
        cx2 = b2.x + b2.w / 2;
        cy2 = b2.y;


        //1
        if (cx2 > cx1 && cy2 < cy1) {
            coltx1 = b1.x + b1.w;
            coltx2 = b2.x;
            if (coltx1 < coltx2) return 10;
            else return 11;
        }
        //4
        if (cx2 > cx1 && cy2 > cy1) {
            coltx1 = b1.x + b1.w;
            coltx2 = b2.x;
            if (coltx1 < coltx2) return 40;
            else return 41;
        }
        //2
        if (cx2 < cx1 && cy2 < cy1) {
            coltx1 = b1.x;
            coltx2 = b2.x + b2.w;
            if (coltx1 > coltx2) return 20;
            else return 21;
        }
        //3
        if (cx2 < cx1 && cy2 > cy1) {
            coltx1 = b1.x;
            coltx2 = b2.x + b2.w;
            if (coltx1 > coltx2) return 30;
            else return 31;
        }
    }
    else {
        cx1 = b1.x + b1.w / 2;
        cy1 = b1.y + b1.h;
        cx2 = b2.x + b2.w / 2;
        cy2 = b2.y;


        //1
        if (cx2 > cx1 && cy2 < cy1) {
            coltx1 = b1.x + b1.w;
            coltx2 = b2.x;
            if (coltx1 < coltx2) return 10;
            else return 11;
        }
        //4
        if (cx2 > cx1 && cy2 > cy1) {
            coltx1 = b1.x + b1.w;
            coltx2 = b2.x;
            if (coltx1 < coltx2) return 40;
            else return 41;
        }
        //2
        if (cx2 < cx1 && cy2 < cy1) {
            coltx1 = b1.x;
            coltx2 = b2.x + b2.w;
            if (coltx1 > coltx2) return 20;
            else return 21;
        }
        //3
        if (cx2 < cx1 && cy2 > cy1) {
            coltx1 = b1.x;
            coltx2 = b2.x + b2.w;
            if (coltx1 > coltx2) return 30;
            else return 31;
        }
    }
}
void drawlines(int p0x,int p0y, int p1x, int p1y, 
    int p2x, int p2y, int p3x, int p3y, int p4x, int p4y,int p5x=0,int p5y=0 ) {    
    if (p5x == 0) {
        line(p0x, p0y, p1x, p1y);
        line(p1x, p1y, p2x, p2y);
        line(p2x, p2y, p3x, p3y);
        line(p3x, p3y, p4x, p4y);
    }
    else {
        line(p0x, p0y,p5x,p5y);
        line(p5x, p5y, p1x, p1y);
        line(p1x, p1y, p2x, p2y);
        line(p2x, p2y, p3x, p3y);
        line(p3x, p3y, p4x, p4y);
    }
    
}
void drawaux(blk& b1,blk& b2, int color,int type, bool side) {
    int caz; 
    caz = cadran(b1, b2);
    int p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y = 0,p5x,p5y;
    if (type==3) {
        if (side == 0) {
            p5x = b1.x;
            p5y = b1.y + b1.h + DECRIGHTBIAS;
            p0x = b1.x;
            p0y = b1.y + b1.h / 2;
        }
        else {
            p5x = b1.x+b1.w;
            p5y = b1.y + b1.h + DECRIGHTBIAS;
            p0x = b1.x + b1.w;
            p0y = b1.y + b1.h / 2;
        }        
    }
    else {
        p0x = b1.x + b1.w/2;
        p0y = b1.y + b1.h;
    }
    if (caz % 10 == 0) {
        if (caz / 10 == 1) {
            p1x = b1.x + b1.w + (b2.x - b1.x - b1.w) / 2;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 4) {
            p1x = b1.x + b1.w + (b2.x - b1.x - b1.w) / 2;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 2) {
            p1x = b2.x + b2.w + (b1.x - b2.x - b2.w) / 2;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 3) {
            p1x = b2.x + b2.w + (b1.x - b2.x - b2.w) / 2;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
    }
    else {
        if (caz / 10 == 1) {
            p1x = b1.x + b1.w + OVERLAPBIAS;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 4) {
            p1x = b1.x + b1.w + OVERLAPBIAS;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 2) {
            p1x = b1.x - OVERLAPBIAS;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 3) {
            p1x = b2.x - OVERLAPBIAS;
            p1y = b1.y + b1.h + DECRIGHTBIAS;
            p2x = p1x;
            p2y = b2.y - DECRIGHTBIAS;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
    }
    setcolor(color);
    if (p4y != 0) {
        if (b1.type == DECISION) {
            drawlines(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y,p5x,p5y);
        }
        else drawlines(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y);
    }
        
}

void drawCnnt_old(int color) {
    setcolor(color);
    for (int i = 0; i < blkSize; i++) {
        if (b[i].next > 0) {
            blk& b1 = b[i];
            blk& b2 = b[b[i].next];
            if (b1.type == DECISION)
                line(b1.x, b1.y + b1.h / 2, b2.x + b2.w / 2, b2.y);
            else 
                line(b1.x + b1.w / 2, b1.y + b1.h, b2.x + b2.w / 2, b2.y);
            
        }
        if (b[i].nextF > 0) {
            blk& b1 = b[i];
            blk& b2 = b[b[i].nextF];
            line(b1.x + b1.w, b1.y + b1.h / 2, b2.x + b2.w / 2, b2.y);
        }
    }
    
}

void drawCnnt(int color) {
    int caz;
    for (int i = 0; i < blkSize; i++) {
        if (b[i].next > 0) {
            if (b[i].type == START || b[i].type == EXPR || b[i].type == READ || b[i].type == WRITE) {
                blk& b1 = b[i];
                blk& b2 = b[b[i].next];

                drawaux(b1, b2, color, 0, 0);
            }
            else {
                blk& b1 = b[i];
                blk& b2 = b[b[i].next];

                drawaux(b1, b2, color, DECISION, 0);
            }
        }   
        if (b[i].nextF > 0) {
            blk& b1 = b[i];
            blk& b2 = b[b[i].nextF];
            drawaux(b1, b2, color, DECISION, 1);
        }
    }
    
}

void drawScheme() {
    cleardevice();
    drawCnnt(BLK_STROKE);
    for (int i = 0; i < blkSize; i++)
        drawBlk(b[i]);
}

bool btween(int a, int b, int c) {
    return b < a && a < c;
}

int selectBlk(int x, int y) {
    for (int i = 0; i < blkSize; i++)
        if (btween(x, b[i].x, b[i].x + b[i].w) && btween(y, b[i].y, b[i].y + b[i].h))
            return i;
    return -1;
}

void deselectBlk(blk& b) {
    b.color = WHITE;
}

bool interTest(blk a, blk b) {
    if (a.x >= b.x + b.w + BOUND || a.y >= b.y + b.h + BOUND)
        return 0;
    if (a.x + a.w + BOUND <= b.x || a.y + a.h + BOUND <= b.y)
        return 0;
    return 1;
}

bool areBlksInter(int j) {
    for (int i = 0; i < blkSize; i++)
        if (i != j)
            if (interTest(b[j], b[i]))
                return 1;
    return 0;
}

void leftClick() {
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);

    int mx = mousex(), my = mousey(), idx = selectBlk(mx, my);
         
    if (idx >= 0) {
        blk& bl = b[idx];
        bl.color = YELLOW;
        drawBlk(bl);

        int dx = bl.x - mx, dy = bl.y - my;
        while (!ismouseclick(WM_LBUTTONUP)) {
            if (mousex() + dx == bl.x && mousey() + dy == bl.y)
                continue;

            drawCnnt(BG);
            clearBlk(bl);
            
            blk cpy = bl;
            bl.x = mousex() + dx;
            bl.y = mousey() + dy;

            if (areBlksInter(idx))
                bl = cpy;

            setcolor(BLACK);
            drawCnnt(BLK_STROKE);
            drawBlk(bl);
            delay(DELAY);
        }

        deselectBlk(bl);
        drawBlk(bl);
    }
}

void writeContainer(blk& bl) {
    char bar[maxLEN], key, tmp[2];
    bar[0] = '\0';

    while (1) {
        int len = strlen(bar);
        key = getch();
        if (key == ENTER_KEY)
            break;
        else if (key == BACKSPACE_KEY) {
            if (len) {
                setcolor(BG);
                outtextxy(bl.x + bl.w / 2 - textwidth(bar) / 2, 
                          bl.y + bl.h / 2 - textheight(bar) / 2, bar);
                bar[len - 1] = '\0';
                setcolor(BLK_STROKE);
                outtextxy(bl.x + bl.w / 2 - textwidth(bar) / 2, 
                          bl.y + bl.h / 2 - textheight(bar) / 2, bar);
            }

            if (bl.w > blkW && textwidth(bar) + BOUND_TXT < bl.w) {
                clearBlk(bl);
                bl.x += BOUND_TXT / 2;
                bl.w -= BOUND_TXT;
                drawBlk(bl);
            }

            continue;
        }
        
        if (len >= maxLEN)
            continue;

        tmp[0] = key;
        tmp[1] = '\0';
        strcat(bar, tmp);
        setcolor(BLK_STROKE);
        outtextxy(bl.x + bl.w / 2 - textwidth(bar) / 2, 
                  bl.y + bl.h / 2 - textheight(bar) / 2, bar);
        
        if (textwidth(bar) + BOUND_TXT > bl.w) {
            clearBlk(bl);
            bl.x -= BOUND_TXT;
            bl.w += BOUND_TXT * 2;
            drawBlk(bl);
        }
        delay(DELAY);
    }
    
    string cont(bar);
    bl.container = cont;
}

void rightClick() {
    int mx = mousex(), my = mousey();
    int idx = selectBlk(mx, my);

    if (idx >= 0) {
        b[idx].color = RED;
        drawBlk(b[idx]);
        while (!ismouseclick(WM_RBUTTONUP)) {
            delay(DELAY);
        }
        delay(100);
        clearBlk(b[idx]);

        for (int i = idx + 1; i < blkSize; i++)
            b[i - 1] = b[i];
        blkSize--;

        for (int i = 0; i < blkSize; i++) {
            if (b[i].next == idx)
                b[i].next = 0;
            if (b[i].nextF == idx)
                b[i].nextF = 0;
            if (b[i].next > idx)
                b[i].next--;
            if (b[i].nextF > idx)
                b[i].nextF--;
        }

        drawScheme();
    }
    else {

        char type = max(1, min(getch() - '0', 6));

        createBlk(type, mousex(), mousey());
        blk& bl = b[blkSize - 1];

        if (areBlksInter(blkSize - 1))
            blkSize--;
        else {
            drawBlk(bl);
            if (bl.type != START && bl.type != STOP)
                writeContainer(bl);
        }
    }

    clearmouseclick(WM_RBUTTONDOWN);
    clearmouseclick(WM_RBUTTONUP);
}

int dist(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void createCnnt(int source, int br) {
    clearmouseclick(WM_LBUTTONDOWN);
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(DELAY);
    }
    int idx = selectBlk(mousex(), mousey());
    if (idx >= 0 && idx != source && b[idx].type != START) {
        if (br)
            b[source].nextF = idx;
        else
            b[source].next = idx;
    }
}

bool mouseHoverEvent(int source, int cx, int cy, int br = 0) {
    if (br == 1 && b[source].nextF)
        return 0;
    if (br == 0 && b[source].next)
        return 0;

    if (dist(mousex(), mousey(), cx, cy) < CNNT_R) {
        fillcircle(cx, cy, CNNT_R, YELLOW);
        createCnnt(source, br);
        fillcircle(cx, cy, CNNT_R, BLACK);
        clearmouseclick(WM_LBUTTONDOWN);
        return 1;
    }
    return 0;
}

bool mouseHover() {
    
    for (int i = 0; i < blkSize; i++)
        if (b[i].type == START || b[i].type == EXPR  || b[i].type == READ || b[i].type == WRITE) {
            int cx = b[i].x + b[i].w / 2, cy = b[i].y + b[i].h;
            if (mouseHoverEvent(i, cx, cy)) {
                drawCnnt(WHITE);
                return 1;
            }
        }
        else if (b[i].type == DECISION) {
            int cx = b[i].x, cy = b[i].y + b[i].h / 2;
            if (mouseHoverEvent(i, cx, cy)) {
                drawCnnt(WHITE);
                return 1;
            }

            cx = b[i].x + b[i].w, cy = b[i].y + b[i].h / 2;
            if (mouseHoverEvent(i, cx, cy, 1)) {
                drawCnnt(WHITE);
                return 1;
            }
        }

    clearmouseclick(WM_LBUTTONDOWN);
    return 0;
}

#endif
