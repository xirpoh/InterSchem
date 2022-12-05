#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "winbgim.h"
#include <math.h>
#include <vector>
#include <stack>
using namespace std;

#include "config.h"
#include "struct.h"
#include "Generarecod.h"
#include "blk.h"
#include "interpretor.h"
#include "export.h"

int main() {
    initwindow(width, height, "InterSchem");
    //import_scheme((char*)"fibonacci.sch");
    drawScheme();
     
    while (1) {
        if (kbhit()) {
            switch (getch()) {
                case 'r':
                    runInterpretor();
                    break;
                case 's':
                    export_scheme();
                    break;
                case 'o':
                    import_scheme();
                    break;
                case ESCAPE_KEY:
                    closegraph();
                    return 0;
            }
        }
        else if (ismouseclick(WM_LBUTTONDBLCLK)) {
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

