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
#include "ConnectorsDrawing.h"
#include "CodePanel.h"
#include "blk.h"
#include "Generarecod.h"
#include "export.h"
#include "interpretor.h"

int main() {
    initwindow(width, height, "InterSchem");
    //import_scheme((char*)"stackSort.sch");
    drawScheme();

    while (1) {
        if (kbhit()) {
            switch (getch()) {
                //case 'r':
                //    runInterpretor(1);
                //    break;
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
            if (interpretor_menu());
            else if (button());
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

