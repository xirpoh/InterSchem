int countercb = 0;
int counterinters = 0;

int cadran(blk& b1, blk& b2, bool branch = 0) {

    int cx0, cy0, cx1, cy1, cx2, cy2, coltx1, coltx2;
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
        cx1 = b1.x;
        cy1 = b1.y + b1.h / 2;
        if (branch) cx1 += b1.w;
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
void bypassingline(int x1, int y1, int& x2, int& y2, int x3, int y3) {
    bool deb = 0, ok = 1, hihglight = 0;
    bool ignore = 0;
    bool intersecteaza = 0;
    int hoffset = 10, voffset = 10;
    bool caz, dir, dir3;
    if (x1 == x2) caz = 1;//vert
    else if (y1 == y2) caz = 0;//horiz
    else { cout << "Error: linia e diagonala\n"; return; }
    if (caz == 0) {
        if (x1 <= x2) dir = 0;//dreapta
        else dir = 1;//stanga
    }
    else {
        if (y1 <= y2) dir = 0;//jos
        else dir = 1;//sus
    }
    int a1, b1, a2, b2, y, x, cut1, aux1, cut2, aux2;
    for (int i = 0; i < blkSize; i++) {
        for (int a = 0; a < blkSize; a++) {
            if (ignorewhendrawingines[a] == 1 && ignorewhendrawingines[i] == 1) {
                ignore = 1;
                break;
            }
        }
        if (ignore) {
            ignore = 0;
            continue;

        }
        a1 = b[i].x;
        a2 = b[i].x + b[i].w;
        b1 = b[i].y;
        b2 = b[i].y + b[i].h;
        if (caz == 0) {
            y = y1;
            if ((y >= b2 + voffset) || (y <= b1 - voffset) ||
                (x1 >= a2 + hoffset) && (x2 >= a2 + hoffset) || (x1 <= a1 - hoffset) && (x2 <= a1 - hoffset)) continue; 
            if (((x1 < a2 + hoffset) && (x1 > a1 - hoffset)) ||
                ((x2 < a2 + hoffset) && (x2 > a1 - hoffset))) {
                //un punct interior folosim x3 y3 
                counterinters++;
                if (deb) {
                    cout << "blocul cu care se intersecteaza e " << i<<'\n';
                    printf("punct interior\n");
                    printf("dir %d\t", dir);
                    printf("caz 0\t");
                    printf("x1= %d, y1= %d, x2= %d, y2= %d, x3= %d, y3= %d\n", x1, y1, x2, y2, x3, y3);
                    printf("a1= %d, a2= %d, b1= %d, b2= %d\n", a1, a2, b1, b2);
                    if (hihglight) {
                        setfillstyle(SOLID_FILL, RED);
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                        setfillstyle(SOLID_FILL, BLACK);
                    }
                }
                ok = 0;
                dir3 = y3 > y ? 0 : 1;
                if (deb) printf("dir3= %d\n", dir3);
                if (dir3 == 0) aux1 = b2 + voffset;
                else aux1 = b1 - voffset;
                if (dir == 0) cut1 = a1 - hoffset;
                else cut1 = a2 + hoffset;
                cut2 = x3; 
                bypassingline(x1, y, cut1, y, 0, 0); 
                line(cut1, y, cut1, aux1);
                line(cut1, aux1, cut2, aux1);
                x2 = cut2;
                y2 = aux1;
                if (deb) {
                    if (hihglight) {
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                    }
                }
                break;
            }
            else if (((x1 < a1 - hoffset) && (x2 > a2 + hoffset)) ||
                ((x1 > a2 + hoffset) && (x2 < a1 - hoffset))) {
                //ambele exteriorare
                counterinters++;
                if (deb) {
                    cout << "blocul cu care se intersecteaza e " << i << '\n';
                    printf("ambele exterioare\n");
                    printf("dir %d\t", dir);
                    printf("caz 0\t");
                    printf("x1= %d, y1= %d, x2= %d, y2= %d, x3= %d, y3= %d\n", x1, y1, x2, y2, x3, y3);
                    printf("a1= %d, a2= %d, b1= %d, b2= %d\n", a1, a2, b1, b2);
                    setfillstyle(SOLID_FILL, RED);
                    if (hihglight) {
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                    }
                    setfillstyle(SOLID_FILL, BLACK);
                }
                ok = 0;
                cut1 = a1 - hoffset;
                cut2 = a2 + hoffset;
                if ((y - b1 - voffset) < (b2 + voffset - y)) aux1 = aux2 = b1 - voffset;
                else aux1 = aux2 = b2 + voffset; 
                if (dir == 0) bypassingline(x1, y, cut1, y, x3, y3);
                else bypassingline(x2, y, cut1, y, x3, y3); 
                line(cut1, y, cut1, aux1);
                line(cut1, aux1, cut2, aux2);
                line(cut2, aux2, cut2, y);
                if (dir == 0) bypassingline(cut2, y, x2, y, x3, y3);
                else bypassingline(cut2, y, x1, y, x3, y3); 
                break;
            }if (deb) {
                if (hihglight) {
                    fillellipse(x1, y1, 6, 6);
                    fillellipse(x2, y2, 6, 6);
                    fillellipse(x3, y3, 6, 6);
                }
            }
        }
        else {
            x = x1;
            if ((x >= a2 + hoffset) || (x <= a1 - voffset) || (y1 >= b2 + voffset) && (y2 >= b2 + voffset) ||
                (y1 <= b1 - voffset) && (y2 <= b1 - voffset)) continue;
            if (((y1 < b2 + voffset) && (y1 > b1 - voffset)) ||
                ((y2 < b2 + voffset) && (y2 > b1 - voffset))) {
                //un punct interior folosim x3 y3
                counterinters++;
                if (deb) {
                    cout << "blocul cu care se intersecteaza e " << i << '\n';
                    printf("punct interior\n");
                    printf("dir %d\t", dir);
                    printf("caz 1\t");
                    printf("x1= %d, y1= %d, x2= %d, y2= %d, x3= %d, y3= %d\n", x1, y1, x2, y2, x3, y3);
                    printf("a1= %d, a2= %d, b1= %d, b2= %d\n", a1, a2, b1, b2);
                    setfillstyle(SOLID_FILL, RED);
                    if (hihglight) {
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                    }
                    setfillstyle(SOLID_FILL, BLACK);
                }
                ok = 0;

                dir3 = x3 > x ? 0 : 1;
                if (deb) printf("dir3= %d\n", dir3);
                if (dir == 0) aux1 = b1 - voffset;
                else aux1 = b2 + voffset;
                if (dir3 == 0) cut1 = a2 + hoffset;
                else cut1 = a1 - hoffset;
                aux2 = y3;
                bypassingline(x, y1, x, aux1, 0, 0);
                line(x, aux1, cut1, aux1);
                line(cut1, aux1, cut1, aux2);
                x2 = cut1;
                y2 = aux2; if (deb) {
                    if (hihglight) {
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                    }
                }
                break;
            }
            else if (((y1 < b1 - voffset) && (y2 > b2 + voffset)) ||
                ((y1 > b2 + voffset) && (y2 < y1 - voffset))) {
                //ambele exteriorare
                counterinters++;
                 if (deb) { 
                    cout << "blocul cu care se intersecteaza e " << i << '\n';
                    printf("ambele exterioare\n");
                    printf("dir %d\t", dir);
                    printf("caz 1\t");
                    printf("x1= %d, y1= %d, x2= %d, y2= %d, x3= %d, y3= %d\n", x1, y1, x2, y2, x3, y3);
                    printf("a1= %d, a2= %d, b1= %d, b2= %d\n", a1, a2, b1, b2);
                    setfillstyle(SOLID_FILL, RED);
                    if (hihglight) {
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                    }
                    setfillstyle(SOLID_FILL, BLACK);
                 }


                ok = 0;
                if ((x - a1 - hoffset) < (a2 + hoffset - x)) cut1 = a1 - hoffset;
                else cut1 = a2 + hoffset;
                if (dir == 0) {
                    aux1 = b1 - voffset;
                    aux2 = b2 + voffset;
                }
                else {
                    aux2 = b1 - voffset;
                    aux1 = b2 + voffset;
                }
                bypassingline(x, y1, x, aux1, x3, y3);
                line(x, aux1, cut1, aux1);
                line(cut1, aux1, cut1, aux2);
                line(cut1, aux2, x, aux2);
                bypassingline(x, aux2, x, y2, x3, y3);
                if (deb) {
                    if (hihglight) {
                        fillellipse(x1, y1, 6, 6);
                        fillellipse(x2, y2, 6, 6);
                        fillellipse(x3, y3, 6, 6);
                    }
                }
                break;
            }
        }
    }
    if (ok) {
        countercb++;
        line(x1, y1, x2, y2);
    }
} 

void drawlines(int p0x, int p0y, int p1x, int p1y,
    int p2x, int p2y, int p3x, int p3y, int p4x, int p4y, int id, int p5x, int p5y, bool applyinend) {
     
    if (applyinend) {
        //cout << "Applyinend\n";
        line(p0x, p0y, p5x, p5y);
        bypassingline(p5x, p5y, p1x, p1y, p2x, p2y);
        bypassingline(p3x, p3y, p4x, p4y, 0, 0);
    }
    else {
        line(p0x, p0y, p5x, p5y);
        bypassingline(p5x, p5y, p1x, p1y, p2x, p2y);
        bypassingline(p1x, p1y, p2x, p2y, p3x, p3y);
        bypassingline(p2x, p2y, p3x, p3y, p4x, p4y);
        line(p3x, p3y, p4x, p4y);
    }  
    

}
void drawaux(blk& b1, blk& b2, int color, int type, bool side, int id=-1, bool branch=0, int color1 = -1) {
    int caz;
    static int counter = 0;
    caz = cadran(b1, b2);
    int p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y = 0, p5x, p5y;
    if (type == 3) {
        if (side == 0) {
            p5x = b1.x;
            p5y = b1.y + b1.h + c3c4side;
            p0x = b1.x;
            p0y = b1.y + b1.h / 2;
        }
        else {
            p5x = b1.x + b1.w;
            p5y = b1.y + b1.h + c3c4side;
            p0x = b1.x + b1.w;
            p0y = b1.y + b1.h / 2;
        }
    }
    else {
        p0x = b1.x + b1.w / 2;
        p0y = b1.y + b1.h;
        p5x = b1.x + b1.w / 2;
        p5y = b1.y + b1.h + c3c4side;

    }
    if (caz % 10 == 0) {
        if (caz / 10 == 1) {
            p1x = b1.x + b1.w + (b2.x - b1.x - b1.w) / 2;
            p1y = b1.y + b1.h + c3c4side;
            p2x = p1x;
            p2y = b2.y - c3c4side;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 4) {
            p1x = b1.x + b1.w + (b2.x - b1.x - b1.w) / 2;
            p1y = b1.y + b1.h + c3c4side;
            p2x = p1x;
            p2y = b2.y - c3c4side;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 2) {
            p1x = b2.x + b2.w + (b1.x - b2.x - b2.w) / 2;
            p1y = b1.y + b1.h + c3c4side;
            p2x = p1x;
            p2y = b2.y - c3c4side;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 3) {
            p1x = b2.x + b2.w + (b1.x - b2.x - b2.w) / 2;
            p1y = b1.y + b1.h + c3c4side;
            p2x = p1x;
            p2y = b2.y - c3c4side;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
    }
    else {
        if (caz / 10 == 1) {
            p1x = b2.x + b2.w + c1c2side;
            p1y = b1.y + b1.h + c3c4side;
            p2x = p1x;
            p2y = b2.y - c3c4side;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 4 || caz / 10 == 3) {
            p5y = b1.y + b1.h + c3c4side / 2;
            p1x = b2.x + b2.w / 2;
            p1y = b1.y + b1.h + c3c4side / 2;
            p2x = p3x = p1x;
            p2y = p3y = p1y;
            p4x = p3x;
            p4y = b2.y;
        }
        else if (caz / 10 == 2) {
            p1x = b2.x - c1c2side;
            p1y = b1.y + b1.h + c3c4side;
            p2x = p1x;
            p2y = b2.y - c3c4side;
            p3y = p2y;
            p3x = b2.x + b2.w / 2;
            p4x = p3x;
            p4y = b2.y;
        }
    }
    if (type == 3) {
        if (side == 0) {
            p1x += 5;
            p2x += 5;
        }
    }
    setcolor(color);
    if (p4y != 0) {
        if (side == 1) p5y = p1y -= 5;
        int rshift = drawntofr[b2.id];
        int lshift = drawntofl[b2.id];
        bool applyinend = 0;
        if (caz / 10 == 3 || caz / 10 == 2) {
            if (rshift == 0 && lshift != 0) rshift = ++drawntofr[b2.id];
            p3x += rshift * 5;
            p4x += rshift * 5;
            p2y = p3y -= 5 * rshift;
            drawntofr[b2.id]++;
        }
        else {
            if (lshift == 0 && rshift != 0) lshift = ++drawntofl[b2.id];
            p3x -= lshift * 5;
            p4x -= lshift * 5;
            p3y = p2y -= 5 * lshift;
            drawntofl[b2.id]++;
        } 
        if (caz % 10 == 1 && (caz / 10 == 3 || caz / 10 == 4)) {
            applyinend = 1;
            p1x = p2x = p3x;
            p2y = p3y = p1y;
            //printf("x0= %d, y0= %d, x5= %d, y5= %d, x1= %d, y1= %d\nx2= %d, y2= %d, x3= %d, y3= %d, x4= %d, y4= %d\n", p0x, p0y, p5x, p5y,p1x,p1y,p2x,p2y,p3x,p3y,p4x,p4y);
        } 
        ignorewhendrawingines[b1.id] = 1;
        ignorewhendrawingines[b2.id] = 1;
        if (b1.id == id && (b1.type != DECISION ||side==branch)) {
            setcolor(color1);
        }
        
        drawlines(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, b1.id, p5x, p5y, applyinend);
        setcolor(color);
        ignorewhendrawingines[b1.id] = 0;
        ignorewhendrawingines[b2.id] = 0;
    }

}

void drawCnnt(int color, int id=-1,bool branch = 0, int color1=-1) {
    countercb = 0;
    counterinters = 0;
    for (int i = 0; i < mxBLK; i++) {
        drawntofr[i] = drawntofl[i] = 0;
    }
    int caz;
    for (int i = 0; i < blkSize; i++) {
        if (b[i].next > 0) {
            if (b[i].type == START || b[i].type == EXPR || b[i].type == READ || b[i].type == WRITE) {
                blk& b1 = b[i];
                blk& b2 = b[b[i].next];

                drawaux(b1, b2, color, 0, 0,id,branch,color1);
            }
            else {
                blk& b1 = b[i];
                blk& b2 = b[b[i].next];

                drawaux(b1, b2, color, DECISION, 0, id, branch,color1);
            }
        }
        if (b[i].nextF > 0) {
            blk& b1 = b[i];
            blk& b2 = b[b[i].nextF];
            drawaux(b1, b2, color, DECISION, 1, id, branch,color1);
        }
    }
}

