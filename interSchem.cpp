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
#include "ConnectorsDrawing.h"
#include "blk.h"
#include "interpretor.h"
#include "export.h"

int main() {
    initwindow(width, height, "InterSchem");
    import_scheme((char*)"fibonacci.sch");
    drawScheme();
    line(width, 0, width, height);

    while (1) {
        if (kbhit()) {
            switch (getch()) {
                case 'r':
                    runInterpretor(1);
                    break;
                case 's':
                    export_scheme();
                    break;
                case 'o':
                    import_scheme();
                    break;
                case 'n':
                    resetScheme();
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
            if (interpretor_menu()) {

            }
            else if (button()) {
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

