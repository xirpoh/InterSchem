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
#include "export.h"

int main() {
    initwindow(width, height, "InterSchem");
    char file_name[30] = "fibonacci.sch";
    import_scheme(file_name);
    drawScheme();
     
    while (1) {
        if (kbhit()) {
            switch (getch()) {
                case 'r':
                    runInterpretor();
                    break;
                case 'e':
                    export_scheme(file_name);
                    break;
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

