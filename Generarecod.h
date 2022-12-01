int EwEntry(int id) {
	for (int i = 0; i < wEntries.size(); i++) {
		if (wEntries[i].beginning == id) return i;
	}
	return -1;
}
void cautawhileentries(int id, vector<ifentry> path,int deb =0) {
	if (id < 0) return;
	blk& c = b[id];
	if (c.type == DECISION) {
		if(deb) cout << "decision ";
		if (c.next >= 0) {
			if (deb) cout << "next >=0 ";
			ifentry* e = new ifentry{ id, 0 };
			path.push_back(*e);
			if (vizitat[c.next] == 0) {
				if (deb) cout << "next nevizitat\n";
				cautawhileentries(c.next, path);
			}
			else {
				if (deb) cout << "next vizitat ";
				int wid = EwEntry(c.next);
				if (wid>=0) {
					wEntries[wid].path.push_back(*(new ifentry{-1,0}));
					for (int i = 0; i < path.size(); i++) {
						wEntries[wid].path.push_back(path[i]);
					}
				}
				else {
					vector<ifentry>* v = new vector<ifentry>;
					for (int i = 0; i < path.size(); i++) {
						v->push_back(path[i]);
					}
					wEntries.push_back(*(new whileentry{ c.next,*v }));
				}											
				uEntries.push_back(*(new usedentry{ id,0 }));
				if (deb) cout << '\n';
			}
			path.pop_back(); 
		}
		else {			
			uEntries.push_back(*(new usedentry{id,0}));
		}
		if (c.nextF >= 0) {
			if (deb) cout << "nextF >=0 ";
			ifentry* e = new ifentry{ id, 1 };
			path.push_back(*e);
			if (vizitat[c.nextF] == 0) {
				if (deb) cout << "nextF nevizitat\n ";
				cautawhileentries(c.nextF, path);
			}
			else {
				if (deb) cout << "nextF vizitat ";
				int wid = EwEntry(c.nextF);
				if (wid >= 0) {
					wEntries[wid].path.push_back(*(new ifentry{ -1,0 }));
					for (int i = 0; i < path.size(); i++) {
						wEntries[wid].path.push_back(path[i]);
					}
				}
				else {
					vector<ifentry>* v = new vector<ifentry>;
					for (int i = 0; i < path.size(); i++) {
						v->push_back(path[i]);
					}
					wEntries.push_back(*(new whileentry{ c.nextF,*v }));
				}
				uEntries.push_back(*(new usedentry{ id,1 }));
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
		vizitat[id] = true;
		if (c.next >= 0) {
			cautawhileentries(c.next,path);
		}
	}	
}
string pathtostring(int wid) {
	string s = "";
	for (int i = 0; i < wEntries[wid].path.size(); i++) {
		if (wEntries[wid].path[i].id == -1) {
			s += "||";
			continue;
		}
		s += b[wEntries[wid].path[i].id].container;
		if (wEntries[wid].path[i].branch == 0) s += "T";
		else s += "F";
		if(i!=wEntries[wid].path.size()-1&&wEntries[wid].path[i+1].id != -1) s += "&&";
	} 
	return s;
}
void tabulation(int tablevel) {
	for (int i = 0; i < tablevel; i++) {
		cout << "  ";
	}{}
}
bool EuEntry(int id,int branch=0) {
	for (auto a : uEntries) {
		if (a.id == id && a.branch == branch) return 1;
	} 
	return 0;
}
bool isused(int id, int branch=0,bool deb=0) {	
	if(deb) cout <<"id "<< id << " next id " << b[id].next << ' ';
	if (b[id].type != DECISION) {
		if (branch == 1) return 1;
		if (EuEntry(id, branch)) {
			if(deb) cout << "is eEntry, not decision\n";
			return 1;
		}
		else return 0;
	}
	else {
		if(deb) cout << "is decision ";
		if (branch == 0) {
			if (b[id].next == -1) {	
				if (deb) cout << " next id =-1\n";
				return 1;
			}
			if (isused(b[id].next,0,deb)&& isused(b[id].next,1,deb)) {
				if (deb) cout << "isused next\n";
				return 1;
			}
		}
		else {
			if (b[id].nextF == -1) {
				if (deb) cout << "nextf id ==-1\n";
				return 1;
			}
			if (isused(b[id].nextF,0,deb) && isused(b[id].nextF, 1,deb)) {
				if (deb) cout << "isused nextF\n";
				return 1;
			}
		}
	}
	return 0;
}
void adduentry(int id, bool branch=0) {
	uEntries.push_back(*(new usedentry{ id,branch }));
}
void thecode(int id, int tablevel=0, bool deb=0) {
	if (b[id].type == START) {
		adduentry(id);
		thecode(b[id].next,tablevel);
		return;
	}
	if (deb) cout << id << '\n';
	if(deb) cout << "<the code>\n";
	int u1 = EuEntry(id), u2 = EuEntry(id,1);
	int u1dec = isused(id), u2dec = isused(id, 1);
	int wid = EwEntry(id);
	if (deb) cout <<"wid "<< wid<<'\n';
	if (wid>=0) {
		if (deb) cout << "<wentry>\n";
		if (!u1) { 
			adduentry(id); 
			tabulation(tablevel);
			cout << "do{\n";
			tabulation(tablevel+1);
			if(b[id].type==EXPR) cout << b[id].container << ";\n";
			else if(b[id].type == READ) {
				cout << "cin>>" << b[id].container << ";\n";
			}			
			thecode(b[id].next, tablevel + 1);
			tabulation(tablevel);
			cout << "}\n";
			tabulation(tablevel);
			cout << "while(" << pathtostring(wid) << ")\n"; 
		}
		else if (deb) cout << "<used>\n";
		return;
	}
	else if (deb) cout << "<not wentry>\n";
	if (b[id].type != DECISION) {
		if (deb) cout << "<not decision>\n";
		if (!u1) {		
			if (deb) cout << "<not used>\n";
			if (b[id].type == READ) {
				if (deb) cout << "<read>\n";
				adduentry(id);
				tabulation(tablevel);
				cout << "cin>>" << b[id].container<<";\n";
				thecode(b[id].next,tablevel);				
			}
			if (b[id].type == EXPR || b[id].type == WRITE) {
				if (deb) cout << "<write||expr>\n";
				adduentry(id);
				tabulation(tablevel);
				cout << b[id].container << '\n';
				thecode(b[id].next, tablevel);				
			}
		}
		else if (deb) cout << "<used>\n";
	}
	else {
		if (deb) cout << "<decision>\n";
		if (!u1dec) {
			if (deb) cout << "<!u1>\n";
			tabulation(tablevel);
			cout << "if(" << b[id].container << "){\n";			
			thecode(b[id].next, tablevel + 1);
			tabulation(tablevel);
			cout <<"}\n";			
		}
		else if (deb) cout << "<u1dec>\n";
		if (!u2dec) {
			if (u1dec) {
				tabulation(tablevel);
				cout << "if(!" << b[id].container << "){\n";				
				thecode(b[id].nextF, tablevel + 1);
				tabulation(tablevel);
				cout << "}\n";
			}
			else {
				tabulation(tablevel);
				cout << "else"<<"{\n";
				thecode(b[id].nextF, tablevel + 1);
				tabulation(tablevel);
				cout << "}\n";
			}
		}
	}
}
void thecodelaunch(int id) {
	vector<ifentry>* v = new vector<ifentry>; 
	cautawhileentries(id, *v);
	/*printf("%d of wentries\n", wEntries.size());
	for (int i = 0; i < wEntries.size(); i++) {
		for (auto a : wEntries[i].path) {
			cout << a.id << ' ' << b[a.id].container << '\n';
		}
	}
	printf("%d of uentries\n", uEntries.size());
	for (auto a : uEntries) {
		cout << a.id << ' ' << a.branch << '\n';
	}*/
	thecode(id);
	//cout << "finish\n";
}

bool button() {
    if (mousex() <= 50 && mousey() >= height - 50) {
        clearmouseclick(WM_LBUTTONDOWN);
        return true;
    }
    return false;
}

void debug(int func) {
    wEntries.clear();
    uEntries.clear();
    for (int i = 0; i < mxBLK; i++) vizitat[i] = 0;
    thecodelaunch(0);   
}
