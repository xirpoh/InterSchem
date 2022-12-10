#ifndef __EXPORT_H__
#define __EXPORT_H__

char open[] = "Open scheme: ";
char save[] = "Save scheme as: ";

void writeText(char* str, int x, int y, int color) {
    char bar[maxLEN], key, tmp[2], caret[2] = "_";
    bar[0] = '\0';

    while (1) {
       
        //Blinking caret
        while (1) {
            drawText(caret, x + textwidth(bar), y, WHITE);
            if (wait_key()) break;
            drawText(caret, x + textwidth(bar), y, BG);
            if (wait_key()) break;
        }
        //
        
        int len = strlen(bar);
        key = getch();
        if (key == ENTER_KEY || key == ESCAPE_KEY)
            break;
        else if (key == BACKSPACE_KEY) {
            if (len) {
                drawText(bar, x, y, BG);
                drawText(caret, x + textwidth(bar), y, BG);
                bar[len - 1] = '\0';
                drawText(bar, x, y, color);
                drawText(caret, x + textwidth(bar), y, WHITE);
            }

            continue;
        }

        if (len >= maxLEN)
            continue;

        tmp[0] = key;
        tmp[1] = '\0';
        strcat(bar, tmp);
        drawText(bar, x, y, color);
        drawText(caret, x + textwidth(bar), y, WHITE);
        delay(DELAY);
    }
    
    if (key == ENTER_KEY)
        strcpy(str, bar);
}

void export_scheme(char* file_path = NULL) {
    char file_name[30] = "";
    if (file_path == NULL) {
        //cout << "Save scheme as: ";
        //scanf("%s", &file_name);
        outtextxy(BOUND_TXT / 2, height - BOUND_TXT / 2 - textheight(save), save);
        writeText(file_name, BOUND_TXT / 2 + textwidth(save), height - BOUND_TXT / 2 - textheight(save), LIGHTGREEN);
    }
    else 
        strcpy(file_name, file_path);
    
    if (!strlen(file_name)) {
        drawScheme();
        return;
    }
    
    strcpy(scheme_name, file_name);
    
    char folder[30] = "schemes\\";
    strcat(folder, file_name);

    FILE* fp = fopen(folder, "w");
    fprintf(fp, "%d\n", blkSize);

    for (int i = 0; i < blkSize; i++) {
        fprintf(fp, "%d\n", b[i].type);

        fprintf(fp, "%s\n", stringToChar(b[i].container));

        fprintf(fp, "%d %d\n", b[i].x, b[i].y);
        fprintf(fp, "%d %d\n", b[i].w, b[i].h);
        fprintf(fp, "%d %d\n", b[i].next, b[i].nextF);
        fprintf(fp, "%d\n", b[i].id);
    }
    
    fclose(fp);
    drawScheme();
}

void import_scheme(char* file_path = NULL) {
    char file_name[30];
    if (file_path == NULL) {
        //cout << "Open scheme: ";
        //scanf("%s", &file_name);
        //outtextxy(BOUND_TXT, height - BOUND_TXT - textheight(scheme_name), scheme_name);
        outtextxy(BOUND_TXT / 2, height - BOUND_TXT / 2 - textheight(open), open);
        writeText(file_name, BOUND_TXT / 2 + textwidth(open), height - BOUND_TXT / 2 - textheight(file_name), LIGHTGREEN);
    }
    else 
        strcpy(file_name, file_path);
    
    
    char folder[30] = "schemes\\";
    strcat(folder, file_name);

    FILE* fp = fopen(folder, "r");

    if (fp == NULL) {
        cout << "File failed to open\n";
        drawScheme();
        return;
    }
    else 
        strcpy(scheme_name, file_name);

    fscanf(fp, "%d\n", &blkSize);
    
    char line[255];

    for (int i = 0; i < blkSize; i++) {
        b[i].color = BLK_STROKE;
        fscanf(fp, "%d\n", &b[i].type);

        fgets(line, 255, fp);
        string cont(line);
        cont.pop_back();
        b[i].container = cont;

        fscanf(fp, "%d %d\n", &b[i].x, &b[i].y);
        fscanf(fp, "%d %d\n", &b[i].w, &b[i].h);
        fscanf(fp, "%d %d\n", &b[i].next, &b[i].nextF);
        fscanf(fp, "%d\n", &b[i].id);
    }

    fclose(fp);
    drawScheme();
}

#endif
