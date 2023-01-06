char* stringToChar(string s) {
    char* c = (char*)malloc(s.size() + 1);
    int i = 0;
    for (; i < s.size(); i++)
        c[i] = s[i];
    c[i] = '\0';
    return c;
}

bool cphidden = 1;


int* formpoly(int x, int y, int w, int h) {
    int* poly = (int*)malloc(8 * 4);
    poly[0] = x;
    poly[1] = y;
    poly[2] = x + w;
    poly[3] = y;
    poly[4] = x + w;
    poly[5] = y + h;
    poly[6] = x;
    poly[7] = y + h;
    return poly;
}
void cleancp() {
    int* poly = formpoly(cpx, cpy, cpx + cpwidth, cpy + cpheight);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, BLACK);
    fillpoly(4, poly);
    setbkcolor(BG);
    setcolor(BLK_STROKE);
}
void colorcp() {
    int* poly = formpoly(cpx, cpy, cpwidth, cpheight);
    setfillstyle(SOLID_FILL, COLOR(cpbg));
    setcolor(COLOR(cpbg));
    fillpoly(4, poly);
    setbkcolor(BG);
    setcolor(BLK_STROKE);
}
void drawcodebutton() {
     
    if (cphidden == 0) {
        int* poly = formpoly(buttonx, buttony, buttonw + cpwidth, buttonh + cpheight);
        setfillstyle(SOLID_FILL, COLOR(cpbg));
        setcolor(COLOR(cpbg));
        fillpoly(4, poly);
        setbkcolor(BLACK);
        char filenamel[] = "arrowimages\\arrowbuttonl.jpg";
        readimagefile(filenamel, lbuttonx, lbuttony, lbuttonx + lbuttonw, lbuttony + lbuttonh);
        char filenamer[] = "arrowimages\\arrowbuttonr.jpg";
        readimagefile(filenamer, rbuttonx, rbuttony, rbuttonx + rbuttonw, rbuttony + rbuttonh);
        char filenamed[] = "arrowimages\\arrowbuttond.jpg";
        readimagefile(filenamed, dbuttonx, dbuttony, dbuttonx + dbuttonw, dbuttony + dbuttonh);
        char filenameu[] = "arrowimages\\arrowbuttonu.jpg";
        readimagefile(filenameu, ubuttonx, ubuttony, ubuttonx + ubuttonw, ubuttony + ubuttonh);
        char filenamemb[] = "arrowimages\\mainbutton.jpg";
        readimagefile(filenamemb, buttonx, buttony, buttonx + buttonw, buttony + buttonh);
        char filenamedownload[] = "arrowimages\\savecode.jpg";
        readimagefile(filenamedownload, dlbuttonx, dlbuttony, dlbuttonx + dlbuttonw, dlbuttony + dlbuttonh);
        poly = formpoly(inputx, inputy + updowncut, inputw, inputh - 2 * updowncut);
        setfillstyle(SOLID_FILL, COLOR(inputbg));
        fillpoly(4, poly); 
        char filenamehidebutton[] = "arrowimages\\hidebutton.jpg";
        readimagefile(filenamehidebutton, hcpx, hcpy, hcpx + hcpw, hcpy + hcph);
    }
    else { 
        char filenameshowbutton[] = "arrowimages\\showbutton.jpg";
        readimagefile(filenameshowbutton, shcpx, shcpy, shcpx + shcpw, shcpy + shcph);
    }
    setbkcolor(BG);
    setcolor(BLK_STROKE);
}
void generalwindow(char* message, int gwwidth = 300, int gwheight = 300, int cx = width/2,int cy=height/2) {
            
    int barwidth = 5; 
    printf("%d %d %d",textwidth(message),gwwidth,gwheight);
    if (textwidth(message) > gwwidth -( 20 + 2 * barwidth)) 
        gwwidth = textwidth(message) + 20 + 2 * barwidth;
    void* savearea;
    int x, y, qx,qy,qw,qh,qprop=2;
    x = cx - gwwidth / 2;
    y = cy - gwheight / 2;
    qy = y + barwidth;
    qx = x + gwwidth - barwidth - gwwidth / 10*qprop;
    qw = gwwidth / 10 * qprop;
    qh = gwwidth / 10 * qprop;
    int size = imagesize(x,y,x+gwwidth,y+gwheight);

    savearea = malloc(size);
    setcolor(MAGENTA);
    setfillstyle(SOLID_FILL, WHITE);
    setbkcolor(WHITE);
    int* poly = formpoly(x, y, gwwidth, gwheight);
        
    
    getimage(x, y, x + gwwidth, y + gwheight, savearea);

    fillpoly(4, poly);
    setfillstyle(SOLID_FILL, COLOR(105,105,105));
    bar(x, y, x + barwidth, y + gwheight);
    bar(x, y, x + gwwidth, y+barwidth );
    bar(x+gwwidth-barwidth, y, x + gwwidth, y + gwheight);
    bar(x , y+gwheight-barwidth, x + gwwidth, y + gwheight);  
    setcolor(RED); 
    circle(qx+qw/2,qy+qh/2,qw/2-3);
    line(qx + qw / 4, qy + qw / 4, qx + qw * 3 / 4, qy + qw * 3 / 4);
    line(qx + qw*3 / 4, qy + qw / 4, qx + qw  / 4, qy + qw * 3 / 4);
    
    outtextxy(cx-textwidth(message)/2, cy - 8, message);
    char* c = new char;
    while (1) {        
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx, my;
            mx = mousex(), my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            if (mx >= qx && mx <= qx + qw && my >= qy && my <= qy + qh) {
                break;
            }
            else if (mx >= x && mx <= x + gwwidth && my >= y && my <= y + gwheight) {
            }
            else {
                setfillstyle(SOLID_FILL,RED);
                bar(x, y, x + barwidth, y + gwheight);
                bar(x, y, x + gwwidth, y + barwidth);
                bar(x + gwwidth - barwidth, y, x + gwwidth, y + gwheight);
                bar(x, y + gwheight - barwidth, x + gwwidth, y + gwheight);
                delay(500);
                while (!ismouseclick(WM_LBUTTONUP)) {
                    delay(50);
                }
                while (ismouseclick(WM_LBUTTONUP)) {
                    clearmouseclick(WM_LBUTTONUP);
                }
                
                setfillstyle(SOLID_FILL, COLOR(105, 105, 105));
                bar(x, y, x + barwidth, y + gwheight);
                bar(x, y, x + gwwidth, y + barwidth);
                bar(x + gwwidth - barwidth, y, x + gwwidth, y + gwheight);
                bar(x, y + gwheight - barwidth, x + gwwidth, y + gwheight);
            }
        }
        while (ismouseclick(WM_LBUTTONUP)) {
            clearmouseclick(WM_LBUTTONUP);
        }
        delay(100);
    } 
    putimage(x, y, savearea,0); 
    setcolor(BLK_STROKE);
    setbkcolor(BG);
}

struct panelline {
    string content;
    int size;
    int horizwritten;
};/*
void trimpanelline(panelline& pl) {

    pl.content[pl.content.size() - 1] = "";
    pl.size--;
}*/
struct panelcontent {
    vector<panelline> lines;
};
struct gtopparam { 
    int vertwritten =0; 
    int skiph =0; 
    int skipv = 0; 
    panelcontent pc;
    int currentline = skipv;
};
void generaltextoutput(string message,int cx,int cy, int pwdith, int pheight,
    gtopparam& details) {
    char* c; 
    int x = cx - pwdith / 2;
    int y = cy - pheight / 2;
    int &vertwritten = details.vertwritten;
    int &skiph = details.skiph;
    int &skipv= details.vertwritten;
    int &currentline = details.currentline;
    panelcontent &pc = details.pc;    
    

    if (message == "") {
        for (int i = skipv;i< pc.lines.size(); i++) {
            if ((vertwritten < pheight - 16)) {
                outtextxy(x, y+vertwritten, stringToChar(pc.lines[i].content));                
                currentline=i;
                vertwritten += 1;
            }
            else break;
        }
        skipv -= 1;
    }            
    else if (message == "\n") {
            string* ns = new string;
            *ns = "";
            panelline* npl = new panelline;
            npl->content = *ns;
            npl->horizwritten = 0;
            npl->size = 0;
            pc.lines.push_back(*npl);
        }
    else {
        cout << currentline << '\n';
        cout << skipv << '\n';
        outtextxy(x + textwidth(stringToChar(pc.lines[currentline].content)),
            y + (currentline - skipv) * 16, stringToChar(message));
        pc.lines[currentline].content += message;

    }            
}
