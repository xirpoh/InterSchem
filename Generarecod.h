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
void tabulation(int tablevel) {
	for (int i = 0; i < tablevel; i++) {
		cout << "  ";
	} {}
}
bool EuEntry(int id, int branch = 0) {
	for (auto a : uEntries) {
		if (a.id == id && a.branch == branch) return 1;
	}
	return 0;
}
bool isused(int id, int branch = 0, bool deb = 0) {
	if (deb) cout << "id " << id  << ' ';
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
void thecode(int id, int tablevel = 0, bool deb = 0) {
	deb = 0;
	if (b[id].type == START|| b[id].type == STOP) {
		adduentry(id);
		thecode(b[id].next, tablevel, deb);
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
				tabulation(tablevel);
				cout << "do{\n";
				tabulation(tablevel + 1);
				if (b[id].type == READ) cout << "cin>>";
				else if (b[id].type == WRITE) cout << "cout<<(";
				cout << b[id].container << "";
				if (b[id].type == WRITE) cout << "cout<<);\n";
				else cout << ";\n";
				thecode(b[id].next, tablevel + 1, deb);
				tabulation(tablevel);
				cout << "}\n";
				tabulation(tablevel);
				cout << "while(" << pathtostring(wid) << ");\n";
			}
			else if (deb) cout << "<used>\n";
			return;
		}
		else {
			if (deb) cout << "<dec wentry>\n";
			int wbranch = wEntries[wid].branch;
			adduentry(id, wbranch);
			if (!u1dec && swbranch == 0) {
				tabulation(tablevel);
				cout << "while(";
				cout << b[id].container;
				cout << "){\n";
				thecode(b[id].next, tablevel + 1, deb);
				tabulation(tablevel);
				cout << "}\n";
				if (!u2dec) {
					tabulation(tablevel);
					cout << "else" <<"{\n";
					thecode(b[id].nextF, tablevel + 1, deb);
					tabulation(tablevel);
					cout << "}\n";
				}
			}
			else if (!u2dec && swbranch == 1) {
				tabulation(tablevel);
				cout << "while(!(";
				cout << b[id].container;
				cout << ")){\n";
				thecode(b[id].nextF, tablevel + 1, deb);
				tabulation(tablevel);
				cout << "}\n";
				if (!u1dec) {
					tabulation(tablevel);
					cout << "else"<< "{\n";
					thecode(b[id].next, tablevel + 1, deb);
					tabulation(tablevel);
					cout << "}\n";
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
				tabulation(tablevel);
				cout << "cin>>" << b[id].container << ";\n";
				if ((EwEntry(b[id].next, 1) == -1 || !EuEntry(b[id].next, 1)) ||
					(EwEntry(b[id].next, 0) == -1 || !EuEntry(b[id].next, 0))) thecode(b[id].next, tablevel, deb);
			}
			if (b[id].type == EXPR || b[id].type == WRITE) {
				if (deb) cout << "<write||expr>\n";
				adduentry(id);
				tabulation(tablevel);
				if (b[id].type == WRITE) cout << "cout<<";
				cout << b[id].container;
				//if (b[id].type == WRITE) cout << ")";
				cout << ";\n";

				if ((EwEntry(b[id].next, 1) == -1 || !EuEntry(b[id].next, 1)) ||
					(EwEntry(b[id].next, 0) == -1 || !EuEntry(b[id].next, 0))) thecode(b[id].next, tablevel, deb);
			}
		}
		else if (deb) cout << "<used>\n";
	}
	else {
		if (deb) cout << "<decision>\n";
		if (!u1dec) {
			if (deb) cout << "<!u1dec>\n";
			tabulation(tablevel);
			cout << "if(" << b[id].container << "){\n";
			if ((EwEntry(b[id].next, 1) == -1 || !EuEntry(b[id].next, 1)) ||
				(EwEntry(b[id].next, 0) == -1 || !EuEntry(b[id].next, 0))) thecode(b[id].next, tablevel + 1, deb);
			tabulation(tablevel);
			cout << "}\n";
		}
		else if (deb) cout << "<u1dec>\n";
		if (!u2dec) {
			if (deb) cout << "<!u2dec>\n";
			if (u1dec) {
				tabulation(tablevel);
				cout << "if(!(" << b[id].container << ")){\n";
				if ((EwEntry(b[id].nextF, 1) == -1 || !EuEntry(b[id].nextF, 1)) ||
					(EwEntry(b[id].nextF, 0) == -1 || !EuEntry(b[id].nextF, 0))) thecode(b[id].nextF, tablevel + 1, deb);
				tabulation(tablevel);
				cout << "}\n";
			}
			else {
				tabulation(tablevel);
				cout << "else" << "{\n";
				if ((EwEntry(b[id].nextF, 1) == -1 || !EuEntry(b[id].nextF, 1)) ||
					(EwEntry(b[id].nextF, 0) == -1 || !EuEntry(b[id].nextF, 0))) thecode(b[id].nextF, tablevel + 1, deb);
				tabulation(tablevel);
				cout << "}\n";
			}
		}
	}
}
void thecodelaunch(int id, bool deb = 0) {
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
	thecode(id, 0, 0);
	if (deb) cout << "finish\n";
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
	for (int i = 0; i < mxBLK * 2; i++) vizitat[i] = 0;
	system("CLS");
	thecodelaunch(0);
}
