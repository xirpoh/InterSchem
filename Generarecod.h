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
char predecessors(int id) {
	char counter = 0;
	for (int i = 0; i < blkSize; i++) {
		if (b[i].next == id || b[i].nextF == id)  counter++;
	}
	return counter;
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
char lastifentryinpath(vector<ifentry> path) {

	for (int i = path.size() - 1; i >= 0; i--) {
		if (b[path[i].id].type == DECISION) return path[i].id;
	}
	return -1;
}
 
void tabulation(int tablevel, bool mod = 0, string filename = "") {
	if (mod == 0 && filename == "") {
		for (int i = 0; i < tablevel; i++) {
			cout << "  ";
		}
	}
	else if (mod == 1 && filename == "") {
		for (int i = 0; i < tablevel * 2; i++) {
			if (skipleftaux > 0) skipleftaux--;
			else {
				outtextxy(codpaneltopoffset + writtenlength, codpanelleftoffset + writtenheight, (char*)" ");
				writtenlength += textwidth((char*)"  ");
			}
		}
	}
	else if (filename != "") {
		string eventualfilename = "savedcpps\\";
		eventualfilename += filename;
		FILE* fptr = fopen(stringToChar(eventualfilename), "a");
		for (int i = 0; i < tablevel * 2; i++) fprintf(fptr, "  ");
		fclose(fptr);
	}
}
void showpath(vector<ifentry> path, int tablevel = 0) {
	cout << "\n";
	tabulation(tablevel);
	cout << "THE PATH : \n";
	tabulation(tablevel);
	for (int i = 0; i < path.size(); i++) {		
		cout << stringToChar(b[path[i].id].container) << "  ";
	}
	cout << "\n";
	tabulation(tablevel);
	cout << "THE PATH : \n";
}

void DeleteUEntry(int id, bool branch) {
	int start;
	bool gasit = 0;
	for (int i = 0; i < uEntries.size(); i++) {
		if (uEntries[i].id == id && uEntries[i].branch == branch) {
			start = i;
			gasit = 1;
			break;
		}
	}
	if (!gasit) return;
	for (int i = start; i < uEntries.size() - 1; i++) {
		uEntries[i] = uEntries[i + 1];
	}
	uEntries.pop_back();

}
void DeleteifEntry(int id, bool branch,vector<ifentry> &path) {
	int start;
	bool gasit = 0;
	for (int i = 0; i < path.size(); i++) {
		if (path[i].id == id && path[i].branch == branch) {
			start = i;
			gasit = 1;
			break;
		}
	}
	if (!gasit) return;
	for (int i = start; i < path.size() - 1; i++) {
		path[i] = path[i + 1];
	}
	path.pop_back();

}
void cautawhileentries(int id, vector<ifentry> &path, int tablevel = 0, bool deb = 0) {
	deb = 0;
	if (id < 0) return;
	if (deb) {
		cout << '\n';
		tabulation(tablevel);
		printf("Current:%s,id:%d;\n",stringToChar(b[id].container), id);
		tabulation(tablevel);
		printf("Next:%s,id:%d;\n", stringToChar(b[b[id].next].container), b[id].next);
		tabulation(tablevel);
		printf("Next:%s,id:%d;\n", stringToChar(b[b[id].nextF].container), b[id].nextF); 

	}
	blk& c = b[id];
	if (c.type != DECISION) {
		ifentry* e = new ifentry{ id, 0 };
		if (!ifinpath(path, id, 0)) path.push_back(*e);
	}
	if(deb)showpath(path,tablevel);
	if (c.type == DECISION) { 
		if (deb) tabulation(tablevel);
		if (deb) cout << "decision ";
		if (c.next >= 0) {
			vizitat[id * 2] = true;
			if (deb) tabulation(tablevel);
			if (deb) printf(" %d e vizitat ", id * 2);
			if (deb) tabulation(tablevel);
			if (deb) cout << "next >=0 ";
			ifentry* e = new ifentry{ id, 0 };
			if (!ifinpath(path, id, 0)) path.push_back(*e);
			if(deb)showpath(path,tablevel);
			if (vizitat[c.next * 2] == 0 && vizitat[c.next * 2 + 1] == 0) {
				if (deb) tabulation(tablevel);
				if (deb) cout << "next nevizitat\n";
				cautawhileentries(c.next, path,tablevel+3, deb);
			}
			else {
				if (deb) tabulation(tablevel);
				if (deb) cout << "next vizitat ";
				if (vizitat[c.next * 2]) {
					int wid = EwEntry(c.next);
					if (deb) tabulation(tablevel);
					if (deb)cout << " \naddwEntry branch 0" << c.next << '\n';
					addwEntry(path, wid, c.next);
				}
				else if (vizitat[c.next * 2 + 1]) {
					int wid = EwEntry(c.next, 1);
					if (deb) tabulation(tablevel);
					if (deb)cout << "\n addwEntry branch 1 " << c.next << " 1 " << '\n';
					addwEntry(path, wid, c.next, 1);
				}
				if (deb) tabulation(tablevel);
				if (deb) cout << '\n';
			}
			if (deb) tabulation(tablevel);
			if(deb)showpath(path,tablevel);
			if (deb) tabulation(tablevel);
			if (deb) printf("path.back.contnt: %s\n",stringToChar(b[path.back().id].container));
			while (path.back().id != id) {				
				if (deb) tabulation(tablevel);
				if (deb) printf("sterge vizitat de %d, branch %d\n", path.back().id, path.back().branch);
				DeleteUEntry(path.back().id, path.back().branch);
				vizitat[2*path.back().id + path.back().branch] = 0;
				path.pop_back();
			}
			vizitat[2 * path.back().id + path.back().branch] = 0;
			path.pop_back();
		}
		else {
			uEntries.push_back(*(new usedentry{ id,0 }));
		}
		if (c.nextF >= 0) {
			vizitat[id * 2 + 1] = true;
			if (deb) tabulation(tablevel);
			if (deb) printf(" %d e vizitat ", id * 2 + 1);
			if (deb) tabulation(tablevel);
			if (deb) cout << "nextF >=0 ";
			ifentry* e = new ifentry{ id, 1 };
			if (!ifinpath(path, id, 1)) path.push_back(*e);
			if(deb)showpath(path,tablevel);
			if (vizitat[c.nextF * 2] == 0 && vizitat[c.nextF * 2 + 1] == 0) {
				if (deb) tabulation(tablevel);
				if (deb) cout << "nextF nevizitat\n ";
				cautawhileentries(c.nextF, path,tablevel+3, deb);
			}
			else {
				if (deb) tabulation(tablevel);
				if (deb) cout << "nextF vizitat ";
				//&& lastifentryinpath(path) != id
				if (vizitat[c.nextF * 2] &&vizitatfrom[c.nextF*2]!=id) {
					int wid = EwEntry(c.nextF);
					if (deb) tabulation(tablevel);
					if (deb)cout << "\n addwEntry branch 0 " << c.nextF << '\n';
					addwEntry(path, wid, c.nextF);
				}
				//&& lastifentryinpath(path) != id
				else if (vizitat[c.nextF * 2 + 1] && vizitatfrom[c.nextF * 2+1] != id) {
					int wid = EwEntry(c.nextF, 1);
					if (deb) tabulation(tablevel);
					if (deb)cout << "\n addwEntry branch 1" << c.next << " 1 " << '\n';
					addwEntry(path, wid, c.nextF, 1);
				}
				else cautawhileentries(c.nextF, path,tablevel+3,deb);
				if (deb) tabulation(tablevel);
				if (deb) cout << '\n';
			}
			if (deb) tabulation(tablevel);
			if(deb)showpath(path,tablevel);
			if (deb) printf("path.back.contnt: %s\n",stringToChar(b[path.back().id].container));
			while (path.back().id != id) {				
				if (deb) tabulation(tablevel);
				if (deb) printf("sterge vizitat de %d, branch %d\n", path.back().id,path.back().branch);
				vizitat[2*path.back().id + path.back().branch] = 0;
				DeleteUEntry(path.back().id, path.back().branch);
				path.pop_back();
			}
			vizitat[2 * path.back().id + path.back().branch] = 0;
			path.pop_back();
		}
		else {
			uEntries.push_back(*(new usedentry{ id,1 }));
		}
	}
	else {
		if (deb) tabulation(tablevel);
		if (deb) cout << "not decision\n";
		vizitat[id * 2] = true;
		if(path.size()>0) vizitatfrom[id * 2] = lastifentryinpath(path);
		if (c.next >= 0) {
			if (deb) tabulation(tablevel);
			if (deb) printf("vizitat[%d] %d, vizitat[%d] %d\n ", c.next * 2, vizitat[c.next * 2], c.next * 2 + 1, vizitat[c.next * 2 + 1]);
			if (vizitat[c.next * 2]) {
				if (deb) tabulation(tablevel);
				if (deb)cout << "\n addwEntry branch 0 " << c.next << '\n';
				int wid = EwEntry(c.next);
				addwEntry(path, wid, c.next, 0);
			}
			else if (vizitat[c.next * 2 + 1]) {
				if (deb) tabulation(tablevel);
				if (deb)cout << "\n addwEntry branch 1" << c.next << " 1 " << '\n';
				int wid = EwEntry(c.next, 1);
				addwEntry(path, wid, c.next, 1);
			}
			else {
				if (deb) tabulation(tablevel);
				if (deb)cout << " cauta in next\n";
				cautawhileentries(c.next, path,tablevel+3, deb);
			}
		}
		else if (c.nextF >= 0) {
			if (deb) tabulation(tablevel);
			if (deb) printf("vizitat[%d] %d, vizitat[%d] %d \n", c.nextF * 2, vizitat[c.nextF * 2], c.nextF * 2 + 1, vizitat[c.nextF * 2 + 1]);
			if (vizitat[c.nextF * 2]) {
				if (deb) tabulation(tablevel);
				if (deb)cout << "\n addwEntry branch 0" << c.nextF << '\n';
				int wid = EwEntry(c.nextF);
				addwEntry(path, wid, c.nextF, 0);
			}
			else if (vizitat[c.nextF * 2 + 1]) {
				if (deb) tabulation(tablevel);
				if (deb)cout << " \naddwEntry branch 1" << c.nextF << '\n';
				int wid = EwEntry(c.nextF, 1);
				addwEntry(path, wid, c.nextF, 1);
			}
			else {
				if (deb) tabulation(tablevel);
				if (deb)cout << " cauta in nextF\n";
				cautawhileentries(c.nextF, path,tablevel+3, deb);
			}
		}
	}
}
string pathtostring(int wid) {
    if (lastifentryinpath(wEntries[wid].path)==-1)
        return "1";

	string s = "";
	vector<ifentry> copy;
	int start = wEntries[wid].beginning;
	bool gasitstrat = 0;
	for (int i = 0; i < wEntries[wid].path.size(); i++) { 
		if (wEntries[wid].path[i].id == start) {
			start = i; gasitstrat=1; 
			break;
		}		
	}	
	if (!gasitstrat) start = 0;
	for (int i = start; i < wEntries[wid].path.size(); i++) {
		if (b[wEntries[wid].path[i].id].type == DECISION) copy.push_back(wEntries[wid].path[i]);		
		else if (wEntries[wid].path[i].id == -1) copy.push_back(wEntries[wid].path[i]);
	}
	
	for (int i = 0; i < copy.size(); i++) {		
		if (copy[i].id == -1) {
			s += " || ";
			continue;
		}
		if (copy[i].branch == 1) s += "!(";
		s += b[copy[i].id].container;
		if (copy[i].branch == 1) s += ")";
		if (i != copy.size() - 1 && copy[i+1].id != -1) s += " && ";
	}
	return s;
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
		if (EuEntry(id, 0)) {
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
	//if (predecessors(id) > 1) return;
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
void scriesequence(string sequence, int lp, int rp, int mod = -1) {
	if (mod == -1) {
		for (int i = lp; i <= rp; i++) {
			cout << sequence[i];
		}
	}
}
vector<string> predefwords = { "if","do","while","return","else" };
vector<string> vartypes = { "int","float","using","namespace" };
vector<string> functions = { "fabs", "exp", "log", "log2", "log10",
								   "sqrt", "cbrt", "sin", "cos", "tan",
								   "asin", "acos", "atan", "sinh",
								   "cosh", "tanh", "asinh", "acosh", "atanh",
								   "ceil", "floor", "round", "trunc", "tgamma",
								   "flipSign", "keepSign", "logicNot", "bitwiseNot", "incr", "decr", "random",
								   "push", "pushS", "pop", "popS", "top", "topS", "empty", "emptyS","main"};
bool isvariable(string sequence) {
	int max;
	string element;
	bool deb = 0;
	if (deb)cout << "Trying in variable\n";
	;
	for (int i = 0; i < mxBLK; i++) {
		if (b[i].type == READ) element = b[i].container;
		else continue;
		max = element.size() > sequence.size() ? element.size() : sequence.size();
		if (deb) printf("S: %s, %d\nelement: %s, %d\nmax: %d\n", stringToChar(sequence), sequence.size(),
			stringToChar(element), element.size(), max);
		if (strncmp(stringToChar(sequence), stringToChar(b[i].container), b[i].container.size()) == 0)
			return true;
	}
	
	return false;
}
bool ismemberofvector(string s, vector <string>& checkin) {
	int max;
	bool deb = 0;
	if (deb)cout << "Trying in membofvector\n";
	;
	for (string element : checkin) {		
		max = element.size() > s.size() ? element.size() : s.size();	
		if (deb) printf("S: %s, %d\nelement: %s, %d\nmax: %d\n", stringToChar(s), s.size(),
			stringToChar(element), element.size(), max);

		if (strncmp(stringToChar(s), stringToChar(element), max) == 0) return true;
	} 
	return false;
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
			//cout << "Decreased filenameesize: " << filenamesize << '\n';
			if (inputwritten != 0) inputwritten -= textwidth(c);
			else {
				inputvoffset -= 16;
				for (int i = 0; i < filenamesize; i++) {
					*c = inputfilename[i];
					inputwritten += textwidth(c);
					//cout << "Inputwritten: " << inputwritten << '\n';
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
			//cout << "Increasedfilenamesize: " << filenamesize << '\n';
			outtextxy(inputx + inputwritten + inputinpadding, inputy + inputinpadding + inputvoffset, c);
			inputwritten += textwidth(c);

			//cout << "Inputwritten: " << inputwritten << '\n';
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
bool isletter(char c) {

	int ascii = (int)c;
	if (ascii >= 65 && ascii <= 90 || ascii >= 97 && ascii <= 122) return true;
	return false;

}
string getsubword(string sequence, int beginning) {
	string s="";
	int length = 0;
	while (isletter(sequence[length+ beginning])) {
		s+= sequence[length+ beginning];
		length++;
	}
	s[length] = '\0';
	return s;

}
void scriecod(string sequence, bool mod = 0, string filename = "") {
	bool deb = 0;
	//if (deb)printf("Scriecod, sequence= \n%s\n", stringToChar(sequence));
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
		if(deb)cout << lp << '\n';
		string subword = getsubword(sequence, lp);
		if (subword.size() > 0) {
			if (deb)cout << "Subword: \n";
			if (deb)cout << subword << '\n';
			rp = lp + subword.size() - 1;
			if (ismemberofvector(subword, vartypes)) {
				if (deb)cout << "vartypet\n";
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ vartypecolorr,vartypecolorg,vartypecolorb });
			}
			else if (ismemberofvector(subword, predefwords)) {
				if (deb)cout << "predefword\n";
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ predefcolorr,predefcolorg,predefcolorb });
			}
			else if (isvariable(subword)) {
				if (deb)cout << "variable\n";
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ varcolorr,varcolorg,varcolorb });
			}
			else if (ismemberofvector(subword, functions)) {
				if (deb)cout << "functions\n";
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ funccolorr,funccolorg,funccolorb });
			} 
			else {
				if (deb)cout << "Caz default\n";				
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar);
			}
		}
		else {
			if (deb)cout << "Not Subword\n";
			if ((rp = isconstant(sequence, lp)) != lp - 1) {
				if (deb)cout << "const\n";
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar, new rgbcolor{ constcolorr,constcolorg,constcolorb });
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
				if (deb)cout << "Caz default\n";
				rp = lp;
				writeaccordingtocase(caz, sequence, lp, rp, stdwpar);
			}
		}
		 

	}

}
void thecode(int id,char restriction,vector<ifentry> &path,  int tablevel = 0, string filename = "", bool mod = 0, bool deb = 0) {
	deb =0;
	static int counterc = 0;
	counterc++;
	//if (counterc % 20 == 0) getch();
	if (deb) showpath(path);
	if (deb) cout<<'\n';
	if (deb)printf("The code, i= %d, cont: %s\n", id,stringToChar(b[id].container));
	if (b[id].type == START || b[id].type == STOP) {
		adduentry(id);
		thecode(b[id].next,0, path, tablevel, filename, mod, deb);
		return;
	}
	if (deb) cout << "id " << id << ' ';
	bool u1 = EuEntry(id), u2 = EuEntry(id, 1);
	bool isnextwentry = (EwEntry(b[id].next) != -1 || EwEntry(b[id].nextF) != -1);
	if (predecessors(id) > 1 &&!isnextwentry ) {
		u1 = 0; u2 = 0;
	}
	if(deb)cout << "pred: " << (int)predecessors(id) << '\n';
	if (deb)cout << "u1: " << u1<< '\n';
	if (deb) cout << "Calculate u1dec\n";
	bool u1dec = isused(id);
	if (deb) cout << "u1dec: "<<u1dec<<'\n';
	if (deb) cout << "Calculate u2dec\n";
	bool u2dec = isused(id, 1);
	if (restriction == 1) {
		u2 = 1;
		u2dec = 1;
	}
	else if (restriction == 2) {
		u1 = 1;
		u1dec = 1;
	}
	if (deb) cout << "u2dec: " << u2dec << '\n';
	int wid=-1;
	int swbranch;
	if (b[id].type == DECISION) {
		if (deb) cout << " decision ";
		if (EwEntry(id) != -1 && restriction != 2) {
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
				path.push_back(*(new ifentry{ id,0 }));
				adduentry(id);
				tabulation(tablevel, mod, filename);
				scriecod("do{\n", mod, filename);
				tabulation(tablevel + 1, mod, filename);
				if (b[id].type == READ) scriecod("cin>>", mod, filename);
				else if (b[id].type == WRITE) scriecod("cout<<(", mod, filename);
				scriecod(b[id].container, mod, filename);
				if (b[id].type == WRITE)  scriecod("cout<<);\n", mod, filename);
				else scriecod(";\n", mod, filename);
				thecode(b[id].next,0, path, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				tabulation(tablevel, mod, filename);
				scriecod("while(", mod, filename);
				scriecod(pathtostring(wid), mod, filename);
				scriecod(");\n", mod, filename);
				DeleteUEntry(id,0); 
				DeleteifEntry(id, 0, path);
			}
			else if (deb) cout << "<used>\n";
			return;
		}
		else {
			if (deb) cout << "<dec wentry>\n";
			int wbranch = wEntries[wid].branch;
			adduentry(id, wbranch);
			if(swbranch==1) path.push_back(*(new ifentry{ id,1}));	 
			else if (swbranch == 0) path.push_back(*(new ifentry{ id,0 }));
			if (!u1dec && swbranch == 0) {
				tabulation(tablevel, mod, filename);
				scriecod("while(", mod, filename); 
				scriecod(pathtostring(wid), mod, filename);
				scriecod("){\n", mod, filename);
				thecode(b[id].next,0, path, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				u2dec = isused(id, 1);
				if (restriction == 1) u2dec = 1;
				if (deb) cout << "Cont: " << b[id].container<<'\n';
				if (deb) cout << "Aafter !udec; u2dec: " << u2dec << '\n';
				isnextwentry = (EwEntry(b[id].next) != -1 || EwEntry(b[id].nextF) != -1);
				if (deb) cout << "isnextwentry: " << isnextwentry << '\n';
				if (deb) cout << "wentries: " << '\n';
				if (deb)for (int i = 0; i < wEntries.size(); i++) {
					cout << b[wEntries[i].beginning].container << "  ";
				}
				if (deb) cout << '\n'; 
				while (path.back().id != id) {
					if (deb) tabulation(tablevel); 
					DeleteUEntry(path.back().id, path.back().branch);
					vizitat[2 * path.back().id + path.back().branch] = 0;
					path.pop_back();
				}
				vizitat[2 * path.back().id + path.back().branch] = 0;
				path.pop_back();
				int wid1 = EwEntry(id, 1);
				if (deb) cout << "Seeing if branch false is whileentry: " <<'\n';
				if (!u2dec && wid1 == -1) {				
					if (deb) cout << "It isn't\n";
					tabulation(tablevel, mod, filename);
					scriecod("if(!(", mod, filename);
					scriecod(b[id].container, mod, filename);
					scriecod(")){\n", mod, filename);
					thecode(b[id].nextF,0, path, tablevel + 1, filename, mod, deb);
					tabulation(tablevel, mod, filename);
					scriecod("}\n", mod, filename);
				}
				else if(!EuEntry(id,1)) {
					if (deb) cout << "It is\n";
					thecode(id, 2, path, tablevel + 1, filename, mod, deb);
				}
			}
			else if (!u2dec && swbranch == 1) {
				tabulation(tablevel, mod, filename);
				scriecod("while(", mod, filename); 
				scriecod(pathtostring(wid), mod, filename);
				scriecod("){\n", mod, filename);
				thecode(b[id].nextF,0, path, tablevel + 1, filename, mod, deb);
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				while (path.back().id != id) {
					if (deb) tabulation(tablevel);
					if (deb) printf("sterge vizitat de %d, branch %d\n", path.back().id, path.back().branch);
					DeleteUEntry(path.back().id, path.back().branch);
					vizitat[2 * path.back().id + path.back().branch] = 0;
					path.pop_back();
				}
				vizitat[2 * path.back().id + path.back().branch] = 0;
				path.pop_back();
				int wid0 = EwEntry(id, 0);
				if (deb) cout << "Seeing if branch true is whileentry: " << '\n';
				if (!u1dec && wid0==-1 ) {
					if (deb) cout << "It isn't\n";
					tabulation(tablevel, mod, filename);
					scriecod("if(", mod, filename);
					scriecod(b[id].container, mod, filename);
					scriecod("){\n", mod, filename);
					thecode(b[id].next,0, path, tablevel + 1, filename, mod, deb);
					tabulation(tablevel, mod, filename);
					scriecod("}\n", mod, filename);
				}
				else if (!EuEntry(id, 0)) {
					if (deb) cout << "It is\n";
					thecode(id, 1, path, tablevel + 1, filename, mod, deb);
					
				}
			} 
			return;
		}

	}
	else if (deb) cout << "<not wentry>\n";
	if (b[id].type != DECISION) {
		if (deb) cout << "<not decision>\n";
		if (!u1|| predecessors(id) > 1) {
			path.push_back(*(new ifentry{ id,0 }));
			if (deb) cout << "<not used>\n";
			if (b[id].type == READ) {
				if (deb) cout << "<read>\n";
				adduentry(id);
				tabulation(tablevel, mod, filename); 
				scriecod("cin>>", mod, filename);
				scriecod(b[id].container, mod, filename);
				scriecod(";\n", mod, filename); 
				if (!ifinpath(path, b[id].next, 0) && !ifinpath(path, b[id].next, 1)) {
					thecode(b[id].next, 0, path, tablevel, filename, mod, deb);
				}									
				DeleteUEntry(id, 0);
			}
			if (b[id].type == EXPR || b[id].type == WRITE) {
				if (deb) cout << "<write||expr>\n";
				adduentry(id);
				tabulation(tablevel, mod, filename);
				if (b[id].type == WRITE) scriecod("cout<<(", mod, filename);
				scriecod(b[id].container, mod, filename);
				if (b[id].type == WRITE) scriecod(")", mod, filename);
				scriecod(";\n", mod, filename); 
				if (!ifinpath(path, b[id].next, 0) && !ifinpath(path, b[id].next, 1)) {
					thecode(b[id].next, 0, path, tablevel, filename, mod, deb);
				}
				DeleteUEntry(id, 0);
			}
			DeleteifEntry(id, 0, path);
		}
		else if (deb) cout << "<used>\n";
	}
	else {
		if (deb) cout << "<decision>\n";
		if (!u1dec) {
			path.push_back(*(new ifentry{ id,0 }));
			if (deb) cout << "<!u1dec>\n";
			tabulation(tablevel, mod, filename);
			scriecod("if(", mod, filename);
			scriecod(b[id].container, mod, filename);
			scriecod("){\n", mod, filename); 
			if (!ifinpath(path, b[id].next, 0) && !ifinpath(path, b[id].next, 1)) {
				thecode(b[id].next, 0, path, tablevel+1, filename, mod, deb);
			} 
			tabulation(tablevel, mod, filename);
			scriecod("}\n", mod, filename);
			while (path.back().id != id) {
				if (deb) tabulation(tablevel); 
				DeleteUEntry(path.back().id, path.back().branch);
				vizitat[2 * path.back().id + path.back().branch] = 0;
				path.pop_back();
			}
			vizitat[2 * path.back().id + path.back().branch] = 0;
			path.pop_back();
		}
		else if (deb) cout << "<u1dec>\n";
		u2dec = isused(id, 1);
		if (restriction == 1) u2dec = 1;
		if (deb) cout << "Cont: " << b[id].container << '\n';
		if (deb) cout << "after !u1dec; u2dec: " << u2dec << '\n';
		if (deb) {
			cout << "EwEntryb[id].next" << EwEntry(b[id].next) << '\n';
			cout << "EwEntryb[id].nextF" << EwEntry(b[id].nextF) << '\n';
			cout << (-1 || -1) << '\n';
		}
		isnextwentry = (EwEntry(b[id].next)!=-1 || EwEntry(b[id].nextF)!=-1);
		if (deb) cout << "isnextwentry: " << isnextwentry << '\n';
		if (deb) cout << "wentries: "  << '\n';
		if(deb)for (int i = 0; i < wEntries.size(); i++) {
			cout << b[wEntries[i].beginning].container << "  ";
		}
		if (deb) cout << '\n';
		if (!u2dec|| predecessors(b[id].nextF) > 1&&!isnextwentry&& b[id].nextF != -1) {
			if (deb) cout << "<!u2dec>\n";
			if (u1dec) {
				path.push_back(*(new ifentry{ id,1 }));
				tabulation(tablevel, mod, filename);
				scriecod("if(!(", mod, filename);
				scriecod(b[id].container, mod, filename);
				scriecod(")){\n", mod, filename); 
				if (!ifinpath(path, b[id].nextF, 0) && !ifinpath(path, b[id].nextF, 1)) {
					thecode(b[id].nextF, 0, path, tablevel+1, filename, mod, deb);
				}
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				while (path.back().id != id) {
					if (deb) tabulation(tablevel); 
					DeleteUEntry(path.back().id, path.back().branch);
					vizitat[2 * path.back().id + path.back().branch] = 0;
					path.pop_back();
				}
				vizitat[2 * path.back().id + path.back().branch] = 0;
				path.pop_back();
			}
			else {
				path.push_back(*(new ifentry{ id,1 }));
				tabulation(tablevel, mod, filename);
				scriecod("else{\n", mod, filename); 
				if (!ifinpath(path, b[id].nextF, 0) && !ifinpath(path, b[id].nextF, 1)) {
					thecode(b[id].nextF, 0, path, tablevel+1, filename, mod, deb);
				} 
				tabulation(tablevel, mod, filename);
				scriecod("}\n", mod, filename);
				while (path.back().id != id) {
					if (deb) tabulation(tablevel); 
					DeleteUEntry(path.back().id, path.back().branch);
					vizitat[2 * path.back().id + path.back().branch] = 0;
					path.pop_back();
				}
				vizitat[2 * path.back().id + path.back().branch] = 0;
				path.pop_back();
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
	cautawhileentries(id, *v);
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
	vector<ifentry> vec;
	thecode(id,0,vec, 1, filename, mod);
	scriecod("\n", mod, filename);
	tabulation(1, mod, filename);
	scriecod("return 0;\n}", mod, filename);
	if (deb) cout << "finish\n";
}
void debug(int func, string filename="") {


	for (int i = 0; i < mxBLK * 2; i++) vizitat[i] = 0;
	bool mod =1;
	writtenlength = 0;
	writtenheight = 0;
	skiptopaux = skiptop;
	skipleftaux = skipleft;
	thecodelaunch(0, filename, mod);
}
void cleancodepanel() {

}
bool button() {	
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);
	int x = mousex(), y = mousey();
	if (x <= (buttonx + buttonw) && x >= buttonx && y >= buttony && y <= (buttonh + buttony)) {
		//system("CLS");
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
			//system("CLS");
			skipleft--;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (x <= (rbuttonx + rbuttonw) && x >= rbuttonx && y >= rbuttony && y <= (rbuttonh + rbuttony)) {
		if (skipleft < cpwidth / 8) {
			//system("CLS");
			skipleft++;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (x <= (ubuttonx + ubuttonw) && x >= ubuttonx && y >= ubuttony && y <= (ubuttonh + ubuttony)) {
		if (skiptop > 0) {
			//system("CLS");
			skiptop--;
			colorcp();
			debug(1);
		}
		return true;
	}
	else if (x <= (dbuttonx + dbuttonw) && x >= dbuttonx && y >= dbuttony && y <= (dbuttonh + dbuttony)) {
		if (skiptop < cpheight / 16) {
			//system("CLS");
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
			//cout << "aici\n";

			string eventualfilename = "savedcpps\\";
			eventualfilename += fn;
			FILE* fptr = fopen(stringToChar(eventualfilename), "w");
			fprintf(fptr, "");
			fclose(fptr);
			debug(0, fn);
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
