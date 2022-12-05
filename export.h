#ifndef __EXPORT_H__
#define __EXPORT_H__

void export_scheme(char* file_path = NULL) {
    char file_name[30];
    if (file_path == NULL) {
        cout << "Save scheme as: ";
        scanf("%s", &file_name);
    }
    else 
        strcpy(file_name, file_path);
    
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
    }

    fclose(fp);
}

void import_scheme(char* file_path = NULL) {
    char file_name[30];
    if (file_path == NULL) {
        cout << "Open scheme: ";
        scanf("%s", &file_name);
    }
    else 
        strcpy(file_name, file_path);
    
    char folder[30] = "schemes\\";
    strcat(folder, file_name);

    FILE* fp = fopen(folder, "r");

    if (fp == NULL) {
        cout << "File failed to open\n";
        return;
    }

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
    }

    fclose(fp);
    drawScheme();
}

#endif
