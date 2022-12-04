#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "winbgim.h"
#include <math.h>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

#include "config.h"
#include "struct.h"
#include "Generarecod.h"
#include "blk.h"
#include "interpretor.h"

int main() {
    initwindow(width, height);
    createBlk(START, 500, 100);
    createBlk(DECISION, 400, 300, "4*5==20&&(6-7==-2||!0)");
    createBlk(STOP, 200, 500);
    createBlk(STOP, 600, 500);

    b[0].next = 1;
    b[1].next = 2;
    b[1].nextF = 3;

    drawScheme();
    
    while (1) {
        if (iskeypressed()) {
        
        }
        if (ismouseclick(WM_LBUTTONDBLCLK)) {
            doubleLeftClick();
        }
        else if (ismouseclick(WM_LBUTTONDOWN)) {
            if (button()) {
                debug(1);
            }
            else if (mouseHover() == 0)
                leftClick();
        }
        else if (ismouseclick(WM_RBUTTONDOWN)) {
            rightClick();
        }
        delay(2 * DELAY);
    }
    
    getch();
    closegraph();
    return 0;
}

