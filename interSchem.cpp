#include <iostream>
#include "graphics.h"
#include "winbgim.h"
#include <math.h>
using namespace std;

#include "config.h"
#include "struct.h"
#include "blk.h"

int main() {

    createBlk(START, width / 2, 100);
    createBlk(STOP, width / 2, 500);

    initwindow(width, height);
    drawScheme();
    
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            if (mouseHover() == 0)
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
