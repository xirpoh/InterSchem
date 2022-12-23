#define varcolorr 135
#define varcolorg 205
#define varcolorb 231
#define constcolorr 165
#define constcolorg 199
#define constcolorb 169
#define funccolorr 213
#define funccolorg 210
#define funccolorb 152
#define vartypecolorr 67
#define vartypecolorg 137
#define vartypecolorb 202
#define predefcolorr 184
#define predefcolorg 138
#define predefcolorb 207
#define includecolorr 148
#define includecolorg 151
#define includecolorb 154
#define stringcolorr 209
#define stringcolorg 144
#define stringcolorb 109
int writtenlength = 0;
int writtenheight = 0;
int skipleft = 0;
int skipleftaux = skipleft;
int skiptop = 0;
int skiptopaux = skiptop;
bool waswritten = 0;
int inputwritten = 0;
int inputvoffset = 0;
string inputfilename = "";
int filenamesize = 0;

int EwEntry(int id, int branch = 0) {
	for (int i = 0; i < wEntries.size(); i++) {
		if (wEntries[i].beginning == id && wEntries[i].branch == branch) return i;
	}
	return -1;
}
bool ifinpath(vector<ifentry> path, int id, bool branch) {
	for (auto e : path) {
		if (e.id == id && e.branch == branch) return 1;
	}
	return 0;

}
void addwEntry(vector<ifentry> path, int wid, int cnext, bool branch = 0, bool deb = 0) {
	if (wid >= 0) {
		wEntries[wid].path.push_back(*(new ifentry{ -1,0 }));
		for (int i = 0; i < path.size(); i++) {
			wEntries[wid].path.push_back(path[i]);
		}
		if (deb)printf(" 1.Adding wentries to %d branch %d \n", cnext, branch);
	}
	else {
		vector<ifentry>* v = new vector<ifentry>;
		for (int i = 0; i < path.size(); i++) {
			v->push_back(path[i]);
		}
		wEntries.push_back(*(new whileentry{ cnext,branch,*v }));
		if (deb)printf(" 2.Adding wentries to %d branch %d \n", cnext, branch);
	}
}
void cautawhileentries(int id, vector<ifentry> path, int deb = 0) {
	if (id < 0) return;
	if (deb) printf("id %d next %d nextF %d\n", id, b[id].next, b[id].nextF);
	blk& c = b[id];
	if (c.type == DECISION) {
		if (deb) cout << "decision ";
		if (c.next >= 0) {
			vizitat[id * 2] = true;
			if (deb) printf(" %d e vizitat ", id * 2);
			if (deb) cout << "next >=0 ";
			ifentry* e = new ifentry{ id, 0 };
			if (!ifinpath(path, id, 0)) path.push_back(*e);
			if (vizitat[c.next * 2] == 0 && vizitat[c.next * 2 + 1] == 0) {
				if (deb) cout << "next nevizitat\n";
				cautawhileentries(c.next, path, deb);
			}
			else {
				if (deb) cout << "next vizitat ";
				if (vizitat[c.next * 2]) {
					int wid = EwEntry(c.next);
					addwEntry(path, wid, c.next);
				}
				else if (vizitat[c.next * 2 + 1]) {
					int wid = EwEntry(c.next, 1);
					addwEntry(path, wid, c.next, 1);
				}
				if (deb) cout << '\n';
			}
			path.pop_back();
		}
		else {
			uEntries.push_back(*(new usedentry{ id,0 }));
		}
		if (c.nextF >= 0) {
			vizitat[id * 2 + 1] = true;
			if (deb) printf(" %d e vizitat ", id * 2 + 1);
			if (deb) cout << "nextF >=0 ";
			ifentry* e = new ifentry{ id, 1 };
			if (!ifinpath(path, id, 1)) path.push_back(*e);
			if (vizitat[c.nextF * 2] == 0 && vizitat[c.nextF * 2 + 1] == 0) {
				if (deb) cout << "nextF nevizitat\n ";
				cautawhileentries(c.nextF, path, deb);
			}
			else {
				if (deb) cout << "nextF vizitat ";
				if (vizitat[c.nextF * 2]) {
					int wid = EwEntry(c.nextF);
					addwEntry(path, wid, c.nextF);
				}
				else if (vizitat[c.nextF * 2 + 1]) {
					int wid = EwEntry(c.nextF, 1);
					addwEntry(path, wid, c.nextF, 1);
				}
				if (deb) cout << '\n';
			}
			path.pop_back();
		}
		else {
			uEntries.push_back(*(new usedentry{ id,1 }));
		}
	}
	else {
		if (deb) cout << "not decision\n";
		vizitat[id * 2] = true;
		if (c.next >= 0) {
			if (deb) printf("vizitat[%d] %d, vizitat[%d] %d\n ", c.next * 2, vizitat[c.next * 2], c.next * 2 + 1, vizitat[c.next * 2 + 1]);
			if (vizitat[c.next * 2]) {
				if (deb)cout << " addwEntry " << c.next << '\n';
				int wid = EwEntry(c.next);
				addwEntry(path, wid, c.next, 0);
			}
			else if (vizitat[c.next * 2 + 1]) {
				if (deb)cout << " addwEntry " << c.next << '\n';
				int wid = EwEntry(c.next, 1);
				addwEntry(path, wid, c.next, 1);
			}
			else {
				if (deb)cout << " cauta in next\n";
				cautawhileentries(c.next, path, deb);
			}
		}
		else if (c.nextF >= 0) {
			if (deb) printf("vizitat[%d] %d, vizitat[%d] %d \n", c.nextF * 2, vizitat[c.nextF * 2], c.nextF * 2 + 1, vizitat[c.nextF * 2 + 1]);
			if (vizitat[c.nextF * 2]) {
				if (deb)cout << " addwEntry " << c.nextF << '\n';
				int wid = EwEntry(c.nextF);
				addwEntry(path, wid, c.nextF, 0);
			}
			else if (vizitat[c.nextF * 2 + 1]) {
				if (deb)cout << " addwEntry " << c.nextF << '\n';
				int wid = EwEntry(c.nextF, 1);
				addwEntry(path, wid, c.nextF, 1);
			}
			else {
				if (deb)cout << " cauta in nextF\n";
				cautawhileentries(c.nextF, path, deb);
			}
		}
	}
}
string pathtostring(int wid) {
	string s = "";
	for (int i = 0; i < wEntries[wid].path.size(); i++) {
		if (wEntries[wid].path[i].id == -1) {
			s += " || ";
			continue;
		}
		if (wEntries[wid].path[i].branch == 1) s += "!(";
		s += b[wEntries[wid].path[i].id].container;
		if (wEntries[wid].path[i].branch == 1) s += ")";
		if (i != wEntries[wid].path.size() - 1 && wEntries[wid].path[i + 1].id != -1) s += " && ";
	}
	return s;
}
void tabulation(int tablevel, bool mod = 0, string filename = "") { 
	if (mod == 0 && filename == "") {
		for (int i = 0; i < tablevel; i++) {
			cout << " ";
		}
	}
	else if (mod == 1 && filename == "") {
		for (int i = 0; i < tablevel * 2; i++) {
			if (skipleftaux > 0) skipleftaux--;			
			else {
				outtextxy(codpaneltopoffset + writtenlength, codpanelleftoffset + writtenheight, (char*)" ");
				writtenlength += textwidth((char*)" ");
			}
		}
	}
	else if (filename != "") {
		string eventualfilename = "savedcpps\\";
		eventualfilename += filename;
		FILE* fptr = fopen(stringToChar(eventualfilename), "a");
		for (int i = 0; i < tablevel * 2; i++) fprintf(fptr, " ");
		fclose(fptr);
	}
}
bool EuEntry(int id, int branch = 0) {
	for (auto a : uEntries) {
		if (a.id == id && a.branch == branch) return 1;
	}
	return 0;
}
bool isused(int id, int branch = 0, bool deb = 0) {
	if (deb) cout << "id " << id << ' ';
	if (b[id].type != DECISION) {
		if (branch == 1) return 1;
		if (EuEntry(id, branch)) {
			if (deb) cout << "is eEntry, not decision\n";
			return 1;
		}
		else return 0;
	}
	else {
		if (deb) cout << "is decision ";
		if (branch == 0) {
			if (b[id].next == -1) {
				if (deb) cout << " next id =-1\n";
				return 1;
			}
			if (isused(b[id].next, 0, deb) && isused(b[id].next, 1, deb)) {
				if (deb) cout << "isused next\n";
				return 1;
			}
		}
		else {
			if (b[id].nextF == -1) {
				if (deb) cout << "nextf id ==-1\n";
				return 1;
			}
			if (isused(b[id].nextF, 0, deb) && isused(b[id].nextF, 1, deb)) {
				if (deb) cout << "isused nextF\n";
				return 1;
			}
		}
	}
	return 0;
}
void adduentry(int id, bool branch = 0) {
	uEntries.push_back(*(new usedentry{ id,branch }));
}
int isconstant(string s, int current) {
	int end = current - 1;

	for (int i = current; i < s.size(); i++) {
		if (s[i] > 47 && s[i] < 58) end++;
		else break;
	}
	return end;
}
int isvariable(string s, int current) {
	int end = current - 1;
	string var = "";
	for (int i = current; i < s.size(); i++) {
		if ((s[i] >= 65 && s[i] <= 90) ||
			(s[i] >= 97 && s[i] <= 122)) {
			end++; var += s[i];
		}
		else break;
	}
	if (end == current - 1) return end;
	for (int i = 0; i < mxBLK; i++) {
		if (strncmp(stringToChar(var), stringToChar(b[i].container), var.size()) == 0) {
			return end;
		};
	}
	return current - 1;
}
void scriesequence(string sequence, int lp, int rp, int mod = -1) {
	if (mod == -1) {
		for (int i = lp; i <= rp; i++) {
			cout << sequence[i];
		}
	}
}
vector<string> predefwords = { "if","do","while","return","else" };
vector<string> vartypes = { "int","float","using","namespace" };
vector<string> functions = { "main","abs" };
int ismemberofvector(string s, int current, vector <string>& checkin) {
	string var = "";
	char sample[10];

	for (string element : checkin) {
		strncpy(sample, stringToChar(s) + current, element.size());
		if (strncmp(sample, stringToChar(element), element.size()) == 0) {
			return current + element.size() - 1;
		}
	}
	return current - 1;
}

int gray(string s, int current) {
	int end = current - 1;
	char sample[10];
	if (s[current] == '#') {
		strncpy(sample, stringToChar(s) + current + 1, 7);
		if (strncmp(sample, stringToChar("include"), 7) == 0) {
			return current + 7;
		}
	}
	return current - 1;
}

int orangeseq(string s, int current) {
	int end = current - 1;
	char sample[10];

	if (s[current] == '<') {
		int end = current - 1;
		for (int i = current + 1; i < s.size(); i++) {
			if (s[i] == '>') { end = i; break; }
		}
		return end;
	}
	if (s[current] == '\"') {
		int end = current - 1;
		for (int i = current + 1; i < s.size(); i++) {
			if (s[i] == '\"') { end = i; break; }
		}
		return end;
	}

	return current - 1;
}
struct rgbcolor {
	int r, g, b;
};
void readfilename() {
	char* c = new char;
	*c = getch();
	setcolor(COLOR(inputfg));
	setbkcolor(COLOR(inputbg));
	if (inputvoffset < 0) return;
	while (*c != 13) {
		c[1] = '\0';
		if (*c == 8) {
			setcolor(COLOR(inputbg));
			setbkcolor(COLOR(inputbg));
			if (filenamesize == 0) {
				*c = getch();
				setcolor(COLOR(inputfg));
				setbkcolor(COLOR(inputbg));
				continue;
			}
			filenamesize--;
			*c = inputfilename[filenamesize];
			cout << "Decreased filenameesize: " << filenamesize << '\n';
			if (inputwritten != 0) inputwritten -= textwidth(c);
			else {
				inputvoffset -= 16;
				for (int i = 0; i < filenamesize; i++) {
					*c = inputfilename[i];
					inputwritten += textwidth(c);
					cout << "Inputwritten: " << inputwritten << '\n';
				}
			}
			*c = inputfilename[filenamesize];
			outtextxy(inputx + inputwritten + inputinpadding, inputy + inputinpadding + inputvoffset, c);
			setcolor(COLOR(inputfg));
			setbkcolor(COLOR(inputbg));

		}
		else {
			if (inputwritten > inputw - 2 * inputinpadding - 8) {
				if (inputvoffset < inputh - 2 * inputinpadding - 32) {
					inputwritten = 0;
					inputvoffset += 16;
				}
				else {
					*c = getch();
					continue;
				}
			}
			if (filenamesize < inputfilename.size()) inputfilename[filenamesize] = *c;
			else inputfilename += *c;
			filenamesize++;
			cout << "Increasedfilenamesize: " << filenamesize << '\n';
			outtextxy(inputx + inputwritten + inputinpadding, inputy + inputinpadding + inputvoffset, c);
			inputwritten += textwidth(c);

			cout << "Inputwritten: " << inputwritten << '\n';
		}

		*c = getch();
	}
	setcolor(BLK_STROKE);
	setbkcolor(BG);
}
struct writeparam {
	int& vertoffset;
	int& horizoffset;
	int& vertskip;
	int& vertskipaux;
	int& horizskip;
	int& horizskipaux;
	int bg;
	double x, y;
	int pwidth, pheight;
	int inpadding;
	vector<char> *writtencontent; 
};

void writeaccordingtocase(int caz, string sequence, int& lp, int rp,
	writeparam* wpar, rgbcolor* rgbc = NULL) {
	bool jmp = 0;
	bool wrote = 0;
	bool res = 1;
	if (rp == -1) {
		res = 0;
		rp = sequence.size()-1;
	}
	//cout <<"The seq "<< sequence << '\n';
	int& vertoffset = wpar->vertoffset;
	int& horizoffset = wpar->horizoffset;
	int& vertskip = wpar->vertskip;
	int& vertskipaux = wpar->vertskipaux;
	int& horizskip = wpar->horizskip;
	int& horizskipaux = wpar->horizskipaux;	
	int pheight = wpar->pheight;
	int pwidth = wpar->pwidth;
	int inpadding = wpar->inpadding;
	int px = wpar->x; 
	int py = wpar->y;
	int bgcolor = wpar->bg; 
	//cout <<"Init hof "<< horizoffset << '\n';
	vector<char> *wcontent = wpar->writtencontent;
	if(horizoffset==0) horizoffset += inpadding;
	if (vertoffset == 0) vertoffset += inpadding;
	if (vertoffset > pheight - 16 - inpadding *2) {
		if (res)lp = rp + 1; 
		return;
	} 
	switch (caz)
	{
	case 0:
		char snum[4];
		int color;
		/*if (color != -1) {
			//itoa(color, snum, 10);
			cout << "\u001b[38;5;";
			cout << snum;
			cout << 'm';
		}*/
		scriesequence(sequence, lp, rp);
		//cout << "\033[0m";
		break;
	case 1:
		if (vertskipaux > 0) {
			for (int i = lp; i < rp + 1; i++)
				if (sequence[i] == '\n')vertskipaux--;
			horizoffset = inpadding; 
			horizskipaux = horizskip;
			break;
		} 
		setbkcolor(bgcolor);
		if (rgbc != NULL) setcolor(COLOR(rgbc->r, rgbc->g, rgbc->b));
		else setcolor(COLOR(255, 255, 255));
		char seq[2];
		for (int i = lp; i <= rp; i++) {			
				//seq[0] = sequence[i]
				strncpy(seq, stringToChar(sequence) + i, 1);
				seq[1] = '\0';
				//cout << "seq[0] "<<seq[0] << '\n';
				if (seq[0] == 8) {
					setcolor(bgcolor);
					setbkcolor(bgcolor);
					int size = wcontent->size();
					//cout <<"size"<< size << '\n'; 
					if (size > 0) {
						seq[0] = (*wcontent)[size - 1];
						size--;
						wcontent->pop_back();
						//cout <<"aft pop seq[0] "<< seq[0] << '\n';
						if (seq[0] != '\n') {
							cout << "case 1" << '\n';  
							horizoffset -= textwidth(seq);							
							//cout<<"tw "<<textwidth(seq)<<'\n';							
							//cout <<"hoff - tw "<< horizoffset << '\n';
							outtextxy(px + horizoffset,
								py + vertoffset, seq);
						}
						else {
							//cout << "case 2" << '\n';  
							horizoffset = inpadding; 							
							seq[0] = (*wcontent)[size - 1];
							while (size > 0&&seq[0]!='\n') {
								//cout << "decsize "<<size << ' ' <<"seq "<< (int)seq << '\n';
								horizoffset += textwidth(seq);
								size--;
								seq[0] = (*wcontent)[size - 1];
							}
							vertoffset -= 16;
						}
					}
				}
				else if (seq[0] == '\n') {   

					//cout << "case 3" << '\n';
					//wcontent->push_back('\n'); 
					horizskipaux = horizskip;
					vertoffset += 16;
					horizoffset = inpadding;
				}
				else if (horizoffset < pwidth - 2 * inpadding) {
				if (horizskipaux > 0) {
					//cout << "case 4" << '\n';
					horizskipaux--; 
				}
				else {
					//cout << "case 5" << '\n';
					//cout << horizoffset << '\n';
					wcontent->push_back(seq[0]);
					outtextxy(px + horizoffset,
						py + vertoffset, seq); 
					horizoffset += textwidth(seq);
					waswritten = 1;
				}
				}	
				//careful aici
				else {
					//cout << "case 6" << '\n';
					wcontent->push_back(10);					
					horizoffset = inpadding;
					vertoffset += 16;
					outtextxy(px + horizoffset,
						py + vertoffset, seq);
					wcontent->push_back(seq[0]);
					horizoffset += textwidth(seq);
				}
		}
		setcolor(COLOR(255, 255, 255));
		break; 
	}  
	if(res)lp = rp + 1; 
	setbkcolor(BG);
	setcolor(BLK_STROKE);
}
void scriecod(string sequence, bool mod = 0, string filename = "") {
	bool deb = 0;
	if (deb)printf("Scriecod, sequence= \n%s\n", stringToChar(sequence));
	int lp = 0, rp = 0;
	int caz;  
	static writeparam* stdwpar = new writeparam{ writtenheight,writtenlength,skiptop,skiptopaux,
		skipleft,skipleftaux, COLOR(cpbg),codpanelleftoffset, codpaneltopoffset,cpwidth,cpheight,padding};	
	vector<char> *wc = new vector<char>;
	stdwpar->writtencontent = wc;
	if (mod == 0 && filename == "") caz = 0;
	else if (mod == 1 && filename == "") caz = 1;
	else caz = 2;
	if (filename != "") {
		string eventualfilename = "savedcpps\\";
		eventualfilename += filename;
		FILE* fptr = fopen(stringToChar(eventualfilename), "a");
		fprintf(fptr, stringToChar(sequence));
		fclose(fptr);
		return;
	}
	while (lp < sequence.size()) {
		if ((rp = isconstant(sequence, lp)) != lp - 1) {
			if (deb)cout << "const\n";

			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ constcolorr,constcolorg,constcolorb });
		}
		else if ((rp = ismemberofvector(sequence, lp, vartypes)) != lp - 1) {
			if (deb)cout << "vartypet\n";
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ vartypecolorr,vartypecolorg,vartypecolorb });
		}
		else if ((rp = ismemberofvector(sequence, lp, functions)) != lp - 1) {
			if (deb)cout << "func\n";
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ funccolorr,funccolorg,funccolorb });
		}
		else if ((rp = ismemberofvector(sequence, lp, predefwords)) != lp - 1) {
			if (deb)cout << "predef\n";
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ predefcolorr,predefcolorg,predefcolorb });
		}
		else if ((rp = isvariable(sequence, lp)) != lp - 1) {
			if (deb)cout << "var\n";
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ varcolorr,varcolorg,varcolorb });
		}
		else if ((rp = orangeseq(sequence, lp)) != lp - 1) {
			if (deb)cout << "orange\n";
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ stringcolorr,stringcolorg,stringcolorb });
		}
		else if ((rp = gray(sequence, lp)) != lp - 1) {
			if (deb)cout << "gray\n";
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ includecolorr,includecolorg,includecolorb });
		}
		else {
			if (deb)cout << "else\n";
			rp = lp;
			writeaccordingtocase(caz, sequence, lp, rp, stdwpar);
		}

	}

}
void thecode(int id, int tablevel = 0, string filename = "", bool mod = 0, bool deb = 0) {
	deb = 0;
	if (deb)printf("The code, i= %d\n", id);
	if (b[id].type == START || b[id].type == STOP) {
		adduentry(id);
		thecode(b[id].next, tablevel, filename, mod, deb);
		return;
	}


	if (deb) cout << "id " << id << ' ';
	int u1 = EuEntry(id), u2 = EuEntry(id, 1);
	int u1dec = isused(id), u2dec = isused(id, 1);
	int wid;
	int swbranch;
	if (b[id].type == DECISION) {
		if (deb) cout << " decision ";
		if (EwEntry(id) != -1) {
			wid = EwEntry(id);
			swbranch = 0;
		}
		else {
			wid = EwEntry(id, 1);
			swbranch = 1;
		}
	}
	else wid = EwEntry(id);
	if (deb) cout << "wid " << wid << '\n';
	if (wid >= 0) {
		if (b[id].type != DECISION) {
			if (deb) cout << "<notdec wentry>\n";
			if (!u1) {
				adduentry(id);
				tabulation(tablevel, mod, filename);
				scriecod("do{\n", mod, filename);
				tabulation(tablevel + 1, mod, filename);
				if (b[id].type == READ) scriecod("cin>>", mod, filename);
				else if (b[id].type == WRITE) scriecod("cout<<(", mod, filename);
				scriecod(b[id].container, mod, filename);
				if (b[id].type == WRITE)  scriecod("cout<<);\n", mod, filename);
				else scriecod(";\n", mod, filename);
				thecode(b[id].next, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				tabulation(tablevel, mod, filename);
				scriecod("while(", mod, filename);
				scriecod(pathtostring(wid), mod, filename);
				scriecod(");\n", mod, filename);
			}
			else if (deb) cout << "<used>\n";
			return;
		}
		else {
			if (deb) cout << "<dec wentry>\n";
			int wbranch = wEntries[wid].branch;
			adduentry(id, wbranch);
			if (!u1dec && swbranch == 0) {
				tabulation(tablevel, mod, filename);
				scriecod("while(", mod, filename);
				scriecod(b[id].container, mod, filename);
				scriecod("){\n", mod, filename);
				thecode(b[id].next, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				if (!u2dec) {
					tabulation(tablevel, mod, filename);
					scriecod("if(!(", mod, filename);
					scriecod(b[id].container, mod, filename);
					scriecod(")){\n", mod, filename);
					thecode(b[id].nextF, tablevel + 1, filename, mod, deb);
					tabulation(tablevel, mod, filename);
					scriecod("}\n", mod, filename);
				}
			}
			else if (!u2dec && swbranch == 1) {
				tabulation(tablevel, mod, filename);
				scriecod("while(!(", mod, filename);
				scriecod(b[id].container, mod, filename);
				scriecod(")){\n", mod, filename);
				thecode(b[id].nextF, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				if (!u1dec) {
					tabulation(tablevel, mod, filename);
					scriecod("if(", mod, filename);
					scriecod(b[id].container, mod, filename);
					scriecod("){\n", mod, filename);
					thecode(b[id].next, tablevel + 1, filename, mod, deb);
					tabulation(tablevel, mod, filename);
					scriecod("}\n", mod, filename);
				}
			}
			return;
		}

	}
	else if (deb) cout << "<not wentry>\n";
	if (b[id].type != DECISION) {
		if (deb) cout << "<not decision>\n";
		if (!u1) {
			if (deb) cout << "<not used>\n";
			if (b[id].type == READ) {
				if (deb) cout << "<read>\n";
				adduentry(id);
				tabulation(tablevel, mod, filename);
				//cout << "cin>>" << b[id].container << ";\n";
				scriecod("cin>>", mod, filename);
				scriecod(b[id].container, mod, filename);
				scriecod(";\n", mod, filename);

				if ((EwEntry(b[id].next, 1) == -1 || !EuEntry(b[id].next, 1)) ||
					(EwEntry(b[id].next, 0) == -1 || !EuEntry(b[id].next, 0))) thecode(b[id].next, tablevel, filename, mod, deb);
			}
			if (b[id].type == EXPR || b[id].type == WRITE) {
				if (deb) cout << "<write||expr>\n";
				adduentry(id);
				tabulation(tablevel, mod, filename);
				if (b[id].type == WRITE) scriecod("cout<<(", mod, filename);
				scriecod(b[id].container, mod, filename);
				if (b[id].type == WRITE) scriecod(")", mod, filename);
				scriecod(";\n", mod, filename);

				if ((EwEntry(b[id].next, 1) == -1 || !EuEntry(b[id].next, 1)) ||
					(EwEntry(b[id].next, 0) == -1 || !EuEntry(b[id].next, 0))) thecode(b[id].next, tablevel, filename, mod, deb);
			}
		}
		else if (deb) cout << "<used>\n";
	}
	else {
		if (deb) cout << "<decision>\n";
		if (!u1dec) {
			if (deb) cout << "<!u1dec>\n";
			tabulation(tablevel, mod, filename);
			scriecod("if(", mod, filename);
			scriecod(b[id].container, mod, filename);
			scriecod("){\n", mod, filename);
			if ((EwEntry(b[id].next, 1) == -1 || !EuEntry(b[id].next, 1)) ||
				(EwEntry(b[id].next, 0) == -1 || !EuEntry(b[id].next, 0))) thecode(b[id].next, tablevel + 1, filename, mod, deb);
			tabulation(tablevel, mod, filename);
			scriecod("}\n", mod, filename);
		}
		else if (deb) cout << "<u1dec>\n";
		if (!u2dec) {
			if (deb) cout << "<!u2dec>\n";
			if (u1dec) {
				tabulation(tablevel, mod, filename);
				scriecod("if(!(", mod, filename);
				scriecod(b[id].container, mod, filename);
				scriecod(")){\n", mod, filename);
				if ((EwEntry(b[id].nextF, 1) == -1 || !EuEntry(b[id].nextF, 1)) ||
					(EwEntry(b[id].nextF, 0) == -1 || !EuEntry(b[id].nextF, 0))) thecode(b[id].nextF, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
			}
			else {
				tabulation(tablevel, mod, filename);
				scriecod("else{\n", mod, filename);
				if ((EwEntry(b[id].nextF, 1) == -1 || !EuEntry(b[id].nextF, 1)) ||
					(EwEntry(b[id].nextF, 0) == -1 || !EuEntry(b[id].nextF, 0))) thecode(b[id].nextF, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
			}
		}
	}
}
void thecodelaunch(int id, string filename = "", bool mod = 0, bool deb = 0) {

	string cppform = "#include <iostream>\nusing namespace std;\n\nint main(){\n";
	wEntries.clear();
	uEntries.clear();
	vector<int> variables;
	string variablesline = "";
	for (int i = 0; i < blkSize; i++) {
		if (b[i].type == READ) {
			variables.push_back(i);
		}
	}
	if (variables.size() != 0) {
		variablesline += "float ";
		for (int i = 0; i < variables.size(); i++) {
			variablesline += b[variables[i]].container;
			if (i < variables.size() - 1) variablesline += ", ";
		}
		variablesline += ";";
	}
	scriecod(cppform, mod, filename); 
	tabulation(1, mod, filename);
	if (deb)cout << "ji";
	scriecod(variablesline + '\n', mod, filename);
	vector<ifentry>* v = new vector<ifentry>;
	cautawhileentries(id, *v, 0);
	for (int i = 0; i < blkSize; i++) {
		if (b[i].type == STOP) {
			adduentry(i, 0);
			adduentry(i, 1);
		}
	}
	if (deb) printf("%d of wentries\n", wEntries.size());
	for (int i = 0; i < wEntries.size(); i++) {
		if (deb) cout << wEntries[i].beginning << ' ';
		for (auto a : wEntries[i].path) {
			if (deb) cout << b[a.id].container << '\n';
		}
	}
	if (deb) printf("%d of uentries\n", uEntries.size());
	for (auto a : uEntries) {
		if (deb) cout << a.id << ' ' << a.branch << '\n';
	}
	thecode(id, 1, filename, mod);
	scriecod("\n", mod, filename);
	tabulation(1, mod, filename);
	scriecod("return 0;\n}", mod, filename);
	if (deb) cout << "finish\n";
}
void debug(int func) {


	for (int i = 0; i < mxBLK * 2; i++) vizitat[i] = 0;
	bool mod = 1;
	writtenlength = 0;
	writtenheight = 0;
	skiptopaux = skiptop;
	skipleftaux = skipleft;
	thecodelaunch(0, "", mod);
}
void cleancodepanel() {

}
bool button() {	
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);
	int x = mousex(), y = mousey();
	if (x <= (buttonx + buttonw) && x >= buttonx && y >= buttony && y <= (buttonh + buttony)) {
		system("CLS");
		colorcp();
		skiptop = 0;
		skiptopaux = skiptop;
		skipleft = 0;
		skipleftaux = 0;
		debug(1);
		return true;
	}
	else if (x <= (lbuttonx + lbuttonw) && x >= lbuttonx && y >= lbuttony && y <= (lbuttonh + lbuttony)) {
		if (skipleft > 0) {
			system("CLS");
			skipleft--;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (x <= (rbuttonx + rbuttonw) && x >= rbuttonx && y >= rbuttony && y <= (rbuttonh + rbuttony)) {
		if (skipleft < cpwidth / 8) {
			system("CLS");
			skipleft++;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (x <= (ubuttonx + ubuttonw) && x >= ubuttonx && y >= ubuttony && y <= (ubuttonh + ubuttony)) {
		if (skiptop > 0) {
			system("CLS");
			skiptop--;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (x <= (dbuttonx + dbuttonw) && x >= dbuttonx && y >= dbuttony && y <= (dbuttonh + dbuttony)) {
		if (skiptop < cpheight / 16) {
			system("CLS");
			skiptop++;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (cphidden == 0 && x <= (inputx + inputw) && x >= inputx && y >= inputy && y <= (inputh + inputy)) {
		readfilename();
		return true;
	}
	else if (cphidden == 0 && x <= (dlbuttonx + dlbuttonw) && x >= dlbuttonx && y >= dlbuttony && y <= (dlbuttonh + dlbuttony)) {
		int fs = filenamesize;
		string fn;
		for (int i = 0; i < filenamesize; i++) {
			fn += inputfilename[i];
		}
		if (fs > 4 && fn[fs - 4] == '.' && ((fn[fs - 1] == 't' && fn[fs - 2] == 'x' && fn[fs - 3] == 't') ||
			(fn[fs - 1] == 'p' && fn[fs - 2] == 'p' && fn[fs - 3] == 'c'))) {
			cout << "aici\n";

			string eventualfilename = "savedcpps\\";
			eventualfilename += fn;
			FILE* fptr = fopen(stringToChar(eventualfilename), "w");
			fprintf(fptr, "");
			fclose(fptr);
			thecodelaunch(0, fn);
		}
		else generalwindow((char*)"Invaid filename!", 150, 150);
		return true;
	}
	else if (cphidden == 0 && x <= (hcpx + hcpw) && x >= hcpx && y >= hcpy && y <= (hcph + hcpy)) {
		cphidden = 1;
		drawScheme();
		return true;
	}
	else if (cphidden == 1 && x <= (shcpx + shcpw) && x >= shcpx && y >= shcpy && y <= (shcph + shcpy)) {
		cphidden = 0;
		drawScheme();
		return true;
	}

	return false;
}
