#ifndef __BLK_H__
#define __BLK_H__

char scheme_name[50] = "untitled.sch";

blk newBlk(int x, int y, int w, int h, char type) {
    blk b = { x, y, w, h, type, BLK_STROKE };
    return b;
}

void resizeBlk(blk& bl) {
    char* txt = stringToChar(bl.container);
    if (textwidth(txt) > bl.w - 2 * BOUND)
        bl.w = textwidth(txt) + 3 * BOUND;
}

void createBlk(char type, int x, int y, string cont = "") {
    blk& bl = b[blkSize] = newBlk(x - blkW / 2, y - blkH / 2, blkW, blkH, type);
    if (type == START)
        bl.container = "START";
    else if (type == STOP)
        bl.container = "STOP";
    else
        bl.container = cont;
    
    resizeBlk(bl); 
    bl.next = -1;
    bl.nextF = -1;
    bl.id = blkSize++;
}

void blkconn(int& x, int& y, blk b, int br = 0) {
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
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BG);
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

        fillcircle(b.x, b.y + b.h / 2, CNNT_R, BG);
        fillcircle(b.x + b.w, b.y + b.h / 2, CNNT_R, BG);

        outtextxy(b.x - 4, cy1 - 25, (char*)"T");
        outtextxy(cx1 - 4, cy1 - 25, (char*)"F");
    }
    else if (b.type == EXPR) {
        //rectangle(b.x, b.y, b.x + b.w, b.y + b.h);
        roundedRect(b.x, b.y, b.x + b.w, b.y + b.h);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BG);
    }
    else if (b.type == READ) {
        line(b.x + BOUND, b.y, b.x + b.w - BOUND, b.y);
        line(b.x + b.w - BOUND, b.y, b.x + b.w, b.y + b.h);
        line(b.x + b.w, b.y + b.h, b.x, b.y + b.h);
        line(b.x, b.y + b.h, b.x + BOUND, b.y);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BG);
    }
    else if (b.type == WRITE) {
        line(b.x, b.y, b.x + b.w, b.y);
        line(b.x + b.w, b.y, b.x + b.w - BOUND, b.y + b.h);
        line(b.x + b.w - BOUND, b.y + b.h, b.x + BOUND, b.y + b.h);
        line(b.x + BOUND, b.y + b.h, b.x, b.y);
        fillcircle(b.x + b.w / 2, b.y + b.h, CNNT_R, BG);
    }

    outtextxy(b.x + b.w / 2 - textwidth(txt) / 2, b.y + b.h / 2 - textheight(txt) / 2, txt);

    free(txt);
}

void drawBlk(blk& b) {
    setcolor(b.color);
    _drawBlk(b);
}

void drawBlks() {
    for (int i = 0; i < blkSize; i++)
        drawBlk(b[i]);
}

void clearBlk(blk b) {
    setcolor(BG);
    _drawBlk(b);
}

void clearBlks() {
    for (int i = 0; i < blkSize; i++)
        clearBlk(b[i]);
}

void drawScheme() {
    //cleardevice();
    setfillstyle(SOLID_FILL, BG);
    //bar(0, 0, WIDTH, HEIGHT);
    bar(0, 0, width, height);
    readimagefile("images\\IDLE.jpg", WIDTH - 306, HEIGHT - 70, WIDTH, HEIGHT);
    readimagefile("images\\FILE.jpg", WIDTH - intermW - filemenuW, HEIGHT - intermH, 
                                      WIDTH - intermW, HEIGHT);

    drawCnnt(CNNT_STROKE);
    drawBlks();
    
    setcolor(WHITE);
    outtextxy(BOUND_TXT / 2, HEIGHT - BOUND_TXT - textheight(scheme_name), scheme_name);

    drawcodebutton();
}

void resetScheme() {
    blkSize = 0;
    strcpy(scheme_name, "untitled.sch");
    drawScheme();
}

bool btween(int a, int b, int c) {
    return b < a&& a < c;
}

int selectBlk(int x, int y) {
    for (int i = 0; i < blkSize; i++)
        if (btween(x, b[i].x, b[i].x + b[i].w) && btween(y, b[i].y, b[i].y + b[i].h))
            return i;
    return -1;
}

void deselectBlk(blk& b) {
    b.color = BLK_STROKE;
}

void deselectBlks() {
    for (int i = 0; i < blkSize; i++)
        deselectBlk(b[i]);
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

bool isBlkInside(blk& bl) {
    if (bl.x - BOUND < 0 || bl.y - BOUND < 0)
        return 0;
    if (bl.x + bl.w + BOUND > WIDTH || bl.y + bl.h + BOUND > HEIGHT - BOUND_TXT)
        return 0;
    return 1;
}

void leftClick() {
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);

    int mx = mousex(), my = mousey(), idx = selectBlk(mx, my);

    if (mx > WIDTH || my > HEIGHT - intermH)
        return;

    if (idx >= 0) {
        blk& bl = b[idx];
        bl.color = LIGHTBLUE;
        drawBlk(bl);
        //drawCnnt(BG);

        int dx = bl.x - mx, dy = bl.y - my;
        int offX[mxBLK], offY[mxBLK];
        for (int i = 0; i < blkSize; i++)
            offX[i] = b[i].x - bl.x, offY[i] = b[i].y - bl.y;
        
        while (!ismouseclick(WM_LBUTTONUP)) {
            if (mousex() + dx == bl.x && mousey() + dy == bl.y)
                continue;
             
            //clearBlk(bl);
            drawCnnt(BG);
            for (int i = 0; i < blkSize; i++)
                if (b[i].color == LIGHTBLUE)
                    clearBlk(b[i]);

            blk _cpy = bl;
            bl.x = mousex() + dx;
            bl.y = mousey() + dy;

            if (areBlksInter(idx) || !isBlkInside(bl))
                bl = _cpy;
            
            for (int i = 0; i < blkSize; i++)
                if (i != idx && b[i].color == LIGHTBLUE) {
                    blk tmp_cpy = b[i];
                    b[i].x = bl.x + offX[i], b[i].y = bl.y + offY[i];
                    if (areBlksInter(i))
                        b[i] = tmp_cpy;
                }

            //setcolor(BG); 
            drawCnnt(CNNT_STROKE);
            //drawBlk(bl);
            for (int i = 0; i < blkSize; i++)
                if (b[i].color == LIGHTBLUE)
                    drawBlk(b[i]);
            delay(DELAY);
        }

        deselectBlks();
        drawBlks();
    }
    else {
        int dx = mx, dy = my;
        while (!ismouseclick(WM_LBUTTONUP)) {
            dx = mousex(), dy = mousey();
            setcolor(LIGHTBLUE);
            /*if (dx > mx)
                rectangle(mx, my, dx, dy);
            else 
                rectangle(dx, my, mx, dy);*/
            rectangle(min(mx, dx), min(my, dy), max(mx, dx), max(my, dy));
            delay(DELAY);
            setcolor(BG);
            rectangle(min(mx, dx), min(my, dy), max(mx, dx), max(my, dy));
            /*if (dx > mx)
                rectangle(mx, my, dx, dy);
            else 
                rectangle(dx, my, mx, dy);*/
        }

        blk select = newBlk(min(mx, dx), min(my, dy), abs(mx - dx), abs(my - dy), EXPR);
        bool just_selected = 0;
        for (int i = 0; i < blkSize; i++)
            if (interTest(select, b[i])) {
                b[i].color = (b[i].color == LIGHTBLUE) ? BLK_STROKE : LIGHTBLUE;
                if (b[i].color == LIGHTBLUE)
                    just_selected = 1;
            }
        if (!just_selected)
            deselectBlks();
        drawScheme();
    }

}

void drawTextBlk(char* bar, blk& bl, int color) {
    setcolor(color);
    outtextxy(bl.x + bl.w / 2 - textwidth(bar) / 2,
        bl.y + bl.h / 2 - textheight(bar) / 2, bar);
}

void drawText(char* txt, int x, int y, int color) {
    setcolor(color);
    outtextxy(x, y, txt);
    setcolor(BG);
}

bool wait_key() {
    for (int t = 0; t < KEY_WAIT; t++)
        if (kbhit())
            return 1;
    return 0;
}

void writeContainer(blk& bl) {
    int x, y;
    char bar[maxLEN], key, tmp[2], caret[2] = "_";
    bar[0] = '\0';
    
    if (bl.container.size()) {
        char* txt = stringToChar(bl.container);
        strcpy(bar, txt);
        free(txt);
        bl.container = "";
    }
    drawTextBlk(bar, bl, HTXT);

    while (1) {
        while (1) {
            drawText(caret, bl.x + bl.w / 2 + textwidth(bar) / 2, bl.y + bl.h / 2 - textheight(caret) / 2, WHITE);
            if (wait_key()) break;
            drawText(caret, bl.x + bl.w / 2 + textwidth(bar) / 2, bl.y + bl.h / 2 - textheight(caret) / 2, BG);
            if (wait_key()) break;
        }

        int len = strlen(bar);
        key = getch();
        if (key == ENTER_KEY || key == ESCAPE_KEY)
            break;
        else if (key == BACKSPACE_KEY) {
            if (bl.w > blkW && textwidth(bar) + 3 * BOUND_TXT < bl.w) {
                clearBlk(bl);
                drawCnnt(BG);
                bl.x += BOUND_TXT / 2;
                bl.w -= BOUND_TXT;
                drawBlk(bl);
                drawCnnt(CNNT_STROKE);
            }

            if (len) {
                drawTextBlk(bar, bl, BG);
                drawText(caret, bl.x + bl.w / 2 + textwidth(bar) / 2, bl.y + bl.h / 2 - textheight(caret) / 2, BG);
                bar[len - 1] = '\0';
                drawTextBlk(bar, bl, HTXT);
                drawText(caret, bl.x + bl.w / 2 + textwidth(bar) / 2, bl.y + bl.h / 2 - textheight(caret) / 2, WHITE);
            }

            continue;
        }

        if (len >= maxLEN)
            continue;

        tmp[0] = key;
        tmp[1] = '\0';
        strcat(bar, tmp);
        drawTextBlk(bar, bl, HTXT);
        drawText(caret, bl.x + bl.w / 2 + textwidth(bar) / 2, bl.y + bl.h / 2 - textheight(caret) / 2, WHITE);
    
        if (textwidth(bar) + BOUND_TXT > bl.w) {
            clearBlk(bl);
            drawCnnt(BG);
            bl.x -= BOUND_TXT;
            bl.w += BOUND_TXT * 2;
            drawBlk(bl);
            drawCnnt(CNNT_STROKE);
        }
        delay(DELAY);
    }
    drawText(caret, bl.x + bl.w / 2 + textwidth(bar) / 2, bl.y + bl.h / 2 - textheight(caret) / 2, BG);
    
    string cont(bar);
    bl.container = cont;
    drawBlk(bl);
}

int dist(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void removeBlk(int idx) {
    b[idx].color = RED;
    drawBlk(b[idx]);
    while (!ismouseclick(WM_RBUTTONUP)) {
        delay(DELAY);
    }
    delay(ANIM_DELAY);
    clearBlk(b[idx]);
    for (int i = idx + 1; i < blkSize; i++) {
        b[i].id--;
        b[i - 1] = b[i];
    }

    blkSize--;

    for (int i = 0; i < blkSize; i++) {
        if (b[i].next == idx)
            b[i].next = -1;
        if (b[i].nextF == idx)
            b[i].nextF = -1;
        if (b[i].next > idx)
            b[i].next--;
        if (b[i].nextF > idx)
            b[i].nextF--;

    }
    drawScheme();
}

bool removeCnnt(int mx, int my) {
    for (int i = 0; i < blkSize; i++)
        if (b[i].type == START || b[i].type == EXPR || b[i].type == READ || b[i].type == WRITE) {
            int cx = b[i].x + b[i].w / 2, cy = b[i].y + b[i].h;
            if (dist(mousex(), mousey(), cx, cy) < CNNT_R) {
                drawCnnt(CNNT_STROKE, i, 0, RED);
                while (!ismouseclick(WM_RBUTTONUP)) {
                    delay(DELAY);
                }
                delay(ANIM_DELAY);
                drawCnnt(BG);
                b[i].next = -1;
                drawCnnt(CNNT_STROKE);
                return 1;
            }
        }
        else if (b[i].type == DECISION) {
            int cx = b[i].x, cy = b[i].y + b[i].h / 2;
            if (dist(mousex(), mousey(), cx, cy) < CNNT_R) {
                drawCnnt(CNNT_STROKE, i, 0, RED);
                while (!ismouseclick(WM_RBUTTONUP)) {
                    delay(DELAY);
                }
                drawCnnt(BG);
                b[i].next = -1;
                drawCnnt(CNNT_STROKE);

                return 1;
            }

            cx = b[i].x + b[i].w, cy = b[i].y + b[i].h / 2;
            if (dist(mousex(), mousey(), cx, cy) < CNNT_R) {
                drawCnnt(CNNT_STROKE, i, 1, RED);
                while (!ismouseclick(WM_RBUTTONUP)) {
                    delay(DELAY);
                }
                delay(ANIM_DELAY);
                drawCnnt(BG);
                b[i].nextF = -1;
                drawCnnt(CNNT_STROKE);
                return 1;
            }
        }

    return 0;
}
void rightClick() {
    int mx = mousex(), my = mousey();

    if (removeCnnt(mx, my)) {
        deselectBlks(); drawBlks();
        clearmouseclick(WM_RBUTTONDOWN);
        clearmouseclick(WM_RBUTTONUP);
        return;
    }

    int idx = selectBlk(mx, my);

    if (idx >= 0) {
        if (b[idx].color == LIGHTBLUE)
            deselectBlk(b[idx]), drawBlk(b[idx]);
        else 
            removeBlk(idx);
    }
    else {
        deselectBlks(); drawBlks();
        char type;
        while (1) {
            type = getch();
            if (type > 48 && type < 55) break;
            else delay(10);
        }
        createBlk(type - 48, mx, my);
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


void createCnnt(int source, int br) {
    clearmouseclick(WM_LBUTTONDOWN);
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(DELAY);
    }
    int idx = selectBlk(mousex(), mousey());
    if (idx >= 0 && b[idx].type != START) {
        if (idx == source) {
            if (b[source].type != DECISION) return;
        }
        if (br)
            b[source].nextF = idx;
        else
            b[source].next = idx;
    }
}

bool mouseHoverEvent(int source, int cx, int cy, int br = 0) {
    if (br == 1 && b[source].nextF != -1)
        return 0;
    if (br == 0 && b[source].next != -1)
        return 0;

    if (dist(mousex(), mousey(), cx, cy) < CNNT_R) { 
        fillcircle(cx, cy, CNNT_R, YELLOW);
        createCnnt(source, br);
        fillcircle(cx, cy, CNNT_R, BG);
        setcolor(BLK_STROKE);
        circle(cx, cy, CNNT_R);
        clearmouseclick(WM_LBUTTONDOWN);
        drawCnnt(BG);
        return 1;
    }
    return 0;
}

bool mouseHover() {
    
    for (int i = 0; i < blkSize; i++)
        if (b[i].type == START || b[i].type == EXPR || b[i].type == READ || b[i].type == WRITE) {
            int cx = b[i].x + b[i].w / 2, cy = b[i].y + b[i].h;
            if (mouseHoverEvent(i, cx, cy)) {
                drawCnnt(CNNT_STROKE);
                deselectBlks(); drawBlks();
                return 1;
            }
        }
        else if (b[i].type == DECISION) {
            int cx = b[i].x, cy = b[i].y + b[i].h / 2;
            if (mouseHoverEvent(i, cx, cy)) {
                drawCnnt(CNNT_STROKE);
                deselectBlks(); drawBlks();
                return 1;
            }

            cx = b[i].x + b[i].w, cy = b[i].y + b[i].h / 2;
            if (mouseHoverEvent(i, cx, cy, 1)) {
                drawCnnt(CNNT_STROKE);
                deselectBlks(); drawBlks();
                return 1;
            }
        }

    clearmouseclick(WM_LBUTTONDOWN);
    return 0;
}

void doubleLeftClick() {
    deselectBlks(), drawBlks();
    int mx = mousex(), my = mousey();
    int idx = selectBlk(mousex(), mousey());
    if (idx >= 0 && b[idx].type != START && b[idx].type != STOP) {
        writeContainer(b[idx]);
    }
    clearmouseclick(WM_LBUTTONDBLCLK);
}

#endif
