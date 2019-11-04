#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits> 
#include <string>
#include <algorithm> 
#include <ios> 
#include <cctype>
using namespace std;

//capitalizes entire string
string capitalize(string s) {
	for (int i = 0; i < s.length(); ++i) s[i]=toupper(s[i]);
	return s;
}

void printLongSep() {
	cout << "=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=\n";
}

void printShortSep() {
	cout << "-----------------------------------\n";
}

void printCurlySep() {
	cout << "~-~-~-~-~-~-~-~-~-~-~-~-~\n";
}

//error message if a file fails to open
void errorCannotOpen(string filename) {
	printLongSep();
	cout << "ERROR: Unable to open " << filename << ".\n";
	cout << "\n";
	cout << "TIPS:\n";
	cout << "Please check if " << filename << " is in the 'pkmn-data' folder.\n";
	cout << "Please check if " << filename << " can be opened.\n";
	cout << "\n";
	cout << "If " << filename << " is missing or damaged, please re-install from https://github.com/IrishMorales/text-based-pokemon-battle";
	printLongSep();
	exit(1);
}

//checks if data files are complete
void checkData(ifstream& pkmnList, ifstream& pkmnMoves) {
	//open pkmnList.txt
	pkmnList.open("pkmn-data/pkmnList.txt");
	if (!pkmnList) errorCannotOpen("pkmnList.txt");
	pkmnList.close();
	
	//open pkmnMoves.txt
	pkmnMoves.open("pkmn-data/pkmnMoves.txt");
	if (!pkmnMoves) errorCannotOpen("pkmnMoves.txt");
	pkmnMoves.close();
}

class Pkmn {
	
	public:
		Pkmn (ifstream& pkmnList, ifstream& pkmnMoves);
		
		//pkmn info
		string name;
		
		//pkmn functions
		int setHP (int lvl, ifstream& pkmnList);
		int setStat (int lvl, ifstream& pkmnList);
		int dmg (int lvl, int movePWR, int ATK1, int DEF2);
		int modMult (int statMult, int movePWR);
		int modStat (int bstat, int statMult);
		int setAEStat (int mult);
		void debug (Pkmn pkmn1, Pkmn pkmn2);
		
	private:
		//pkmn type
		string type1;
		string type2;
		
		//pkmn moveset and info
		string move[4];
		string moveType[4];
		string moveCat[4]; //Physical/Special
		int movePP[4];
		int movebasePP[4];
		int movePWR[4];
		int moveACC[4];
		int moveRep1[4];
		int moveRep2[4];
		
		//pkmn stats - baseStat affected by multiplier
		int lvl;
		int HP;
		int ATK;
		int DEF;
		int SATK;
		int SDEF;
		int SPD;
		
		//pkmn hidden stats
		int ACC = 1; //100/100
		int EVA = 1; //100/100
		
		//pkmn base stats - stat when multiplier is 0
		int baseATK;
		int baseDEF;
		int baseSATK;
		int baseSDEF;
		int baseSPD;
		
		//pkmn stat multipliers
		int ATKmult = 0;
		int DEFmult = 0;
		int SATKmult = 0;
		int SDEFmult = 0;
		int SPDmult = 0;
		int ACCmult = 0;
		int EVAmult = 0;
};

//compute HP with base HP and lvl
int Pkmn::setHP(int lvl, ifstream& pkmnList) {
	string tmpLine;
	getline (pkmnList, tmpLine, ',');
	
	int bHP = stoi(tmpLine);
	int HP = static_cast<int>(((2*bHP*lvl)/100) + 10 + lvl);
	
	return HP;
}
		
//compute stat with base stat and lvl
int Pkmn::setStat(int lvl, ifstream& pkmnList) {
	string tmpLine;
	getline (pkmnList, tmpLine, ',');
	
	int bStat = stoi(tmpLine);
	int stat = static_cast<int>(((2*bStat*lvl)/100) + 5);
	
	return stat;
}
		
//compute move damage with attacker's lvl, movePWR, attack, and opponent's defense
int Pkmn::dmg(int lvl, int movePWR, int ATK1, int DEF2) {
	int finalDmg = static_cast<int>(((2*lvl / 5 + 2) * movePWR * (ATK1/DEF2))/50 + 2);
	return finalDmg;
}
		
//modify pkmn stat multipliers with stat-changing moves
int Pkmn::modMult(int statMult, int movePWR) {
	statMult += movePWR;
	if (statMult < -6) statMult = -6;
	else if (statMult > 6) statMult = 6;
	return statMult;
}
		
//modify pkmn stat with stat multipliers
int Pkmn::modStat(int bstat, int statMult) {
	int finalStat = bstat * max(2, 2 + statMult)/max(2, 2-statMult);
	return finalStat;
}
		
//called after modMult() and before modStat()
//changes multiplier to a number between .33 - 3
int Pkmn::setAEStat(int mult) {
	int val = 1;
	
	switch (mult) {
		case -6:
			val = static_cast<int>(33/100);
			break;
		case -5:
			val = static_cast<int>(36/100);
			break;
		case -4:
			val = static_cast<int>(43/100);
			break;
		case -3:
			val = static_cast<int>(50/100);
			break;
		case -2:
			val = static_cast<int>(60/100);
			break;
		case -1:
			val = static_cast<int>(75/100);
			break;
		case 0:
			break;
		case 1:
			val = static_cast<int>(133/100);
			break;
		case 2:
			val = static_cast<int>(166/100);
			break;
		case 3:
			val = static_cast<int>(200/100);
			break;
		case 4:
			val = static_cast<int>(233/100);
			break;
		case 5:
			val = static_cast<int>(266/100);
			break;
		case 6:
			val = static_cast<int>(300/100);
			break;
	}
	
	return val;
}

//return search pointer to beginning of ifstream file
void returnToBegin(ifstream& file) {
	file.clear();
	file.seekg(0, ios::beg);
}

//check if name is valid
bool checkValidName(string tmpName, ifstream& pkmnList) {
	//if name not all alphabetical char
	if (tmpName.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) {
		cout << "That's one weird Pokemon. Try again?\n";
		cout << "\n";
		return false;
	}
	
	else {
		returnToBegin(pkmnList);
		string tmpLine;
		
		//search message
		cout << "Finding a wild " << tmpName << "...\n";
		
		//search pkmnList for name
		while (getline(pkmnList, tmpLine, ',')) {	
	        if (tmpLine == tmpName) {
	        	return true;
	    	}
		}
		
		//if name not found in pkmnList.txt
		cout << "Looks like " << tmpName << " isn't a Pokemon. :(\n";
		cout << "TIPS: Check spelling or see pkmnList.txt in folder pkmn-data for valid Pokemon.\n";
		cout << "\n";
		return false;
	}
}

//prompt user for valid name
string getValidName(ifstream& pkmnList) {
	string tmpName;
	bool validName = false;
	
	while (!validName) {
		cout << "Please input a Pokemon.\n";
		cout << "YOU: ";
		getline (cin, tmpName);
		cout << "\n";
		tmpName = capitalize(tmpName);
		validName = checkValidName(tmpName, pkmnList);
	}
	
	cout << "Found " << tmpName << "!\n";
	return tmpName;
}

//check if lvl is valid
bool checkValidLvl(string tmpLvl) {
	//lvl must be within 1-100
	if (tmpLvl.find_first_not_of("1234567890") != std::string::npos || !(stoi(tmpLvl) > 0 && stoi(tmpLvl) < 101)) {
		cout << "Level '" << tmpLvl << "' doesn't seem right.\n";
		cout << "\n";
		return false;
	}
	
	return true;
}

//prompt user for valid lvl
int getValidLvl(string name) {
	string tmpLvl;
	bool validLvl = false;
	
	cout << "What level will " << name << " be?\n";
	
	while (!validLvl) {
		cout << "Please input a level within 1-100.\n";
		cout << "YOU: ";
		getline (cin, tmpLvl);
		cout << "\n";
		validLvl = checkValidLvl(tmpLvl);
	}
	
	cout << "Level " << stoi(tmpLvl) << " set!\n";
	return stoi(tmpLvl);
}

//get type from stream
string getNext(ifstream& file) {
	string tmpLine;
	getline (file, tmpLine, ',');
	return tmpLine;
}

//check if move is valid
bool checkValidMove(string tmpMove, ifstream& pkmnMoves, string move[4]) {
	//if move not all alphabetical char
	if (tmpMove.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ '-") != std::string::npos) {
		cout << "Hey there! You probably used some weird number or punctuation. Try again?\n";
		cout << "\n";
		return false;
	}
	//if move already learned
	else if (find(move, move + 4, tmpMove) != move + 4) {
		cout << "This Pokemon already knows " << tmpMove << "!\n";
		cout << "\n";
		return false;
	}
	
	else {
		returnToBegin(pkmnMoves);
		string tmpLine;
		
		//search pkmnMoves for move
		while (getline(pkmnMoves, tmpLine, ',')) {	
	        if (tmpLine == tmpMove) {
	        	return true;
	    	}
		}
		
		//if move not found in pkmnMoves.txt
		cout << "Sorry, we couldn't find that! :(\n";
		cout << "TIPS: Check spelling or see pkmnMoves.txt in folder pkmn-data for valid moves.\n";
		cout << "\n";
		return false;
	}
}

//prompt user for valid move
string getValidMove(string name, ifstream& pkmnMoves, string move[4]) {
	string tmpMove;
	bool validMove = false;
	
	while (!validMove) {
		cout << "Please input any valid move for " << name << ".\n";
		cout << "YOU: ";
		getline (cin, tmpMove);
		cout << "\n";
		tmpMove = capitalize(tmpMove);
		validMove = checkValidMove(tmpMove, pkmnMoves, move);
	}
	
	cout << name << " learned " << tmpMove << "!\n";
	return tmpMove;
}

Pkmn::Pkmn(ifstream& pkmnList, ifstream& pkmnMoves) {
	//set pkmn
	pkmnList.open("pkmn-data/pkmnList.txt");
	
	printShortSep();
	name = getValidName(pkmnList);
	printShortSep();
	lvl = getValidLvl(name);
	
	type1 = getNext(pkmnList);
	type2 = getNext(pkmnList);
	
	HP = setHP (lvl, pkmnList);
	ATK = setStat (lvl, pkmnList);
	baseATK = ATK;
	DEF = setStat (lvl, pkmnList);
	baseDEF = DEF;
	SATK = setStat (lvl, pkmnList);
	baseSATK = SATK;
	SDEF = setStat (lvl, pkmnList);
	baseSDEF = SDEF;
	SPD = setStat (lvl, pkmnList);
	baseSPD = SPD;
	
	pkmnList.close();
    
    //set pkmn moveset
    pkmnMoves.open("pkmn-data/pkmnMoves.txt");
    
	for (int i = 0; i < 4; ++i) {
		printShortSep();
		move[i] = getValidMove(name, pkmnMoves, move);
	    moveType[i] = getNext(pkmnMoves);
	    moveCat[i] = getNext(pkmnMoves);
		movebasePP[i] = stoi(getNext(pkmnMoves));
		movePP[i] = movebasePP[i];
		movePWR[i] = stoi(getNext(pkmnMoves));
		moveACC[i] = stoi(getNext(pkmnMoves));
				
		if (moveCat[i] == "PREPR" || moveCat[i] == "PREP") {
			moveRep1[i] = stoi(getNext(pkmnMoves));
			if (moveCat[i] == "PREPR") {
					moveRep2[i] = stoi(getNext(pkmnMoves)) -1;
			}
		}
	}
	
	printShortSep();
	pkmnMoves.close();
	
	cout << name << " is ready!\n";
}

//DEBUG TOOLS: print pkmn info
void Pkmn::debug(Pkmn pkmn1, Pkmn pkmn2) {
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "DEBUG INFO:" << endl;
	cout << pkmn1.name << endl;
	cout << pkmn1.lvl << endl;
	cout << pkmn1.HP << endl;
	cout << pkmn1.baseATK << endl;
	cout << pkmn1.baseDEF << endl;
	cout << pkmn1.baseSATK << endl;
	cout << pkmn1.baseSDEF << endl;
	cout << pkmn1.baseSPD << endl;
	cout << pkmn1.HP << endl;
	cout << pkmn1.ATK << endl;
	cout << pkmn1.DEF << endl;
	cout << pkmn1.SATK << endl;
	cout << pkmn1.SDEF << endl;
	cout << pkmn1.SPD << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << pkmn2.name << endl;
	cout << pkmn2.lvl << endl;
	cout << pkmn2.HP << endl;
	cout << pkmn2.baseATK << endl;
	cout << pkmn2.baseDEF << endl;
	cout << pkmn2.baseSATK << endl;
	cout << pkmn2.baseSDEF << endl;
	cout << pkmn2.baseSPD << endl;
	cout << pkmn2.HP << endl;
	cout << pkmn2.ATK << endl;
	cout << pkmn2.DEF << endl;
	cout << pkmn2.SATK << endl;
	cout << pkmn2.SDEF << endl;
	cout << pkmn2.SPD << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
} 

int main(){
	ifstream pkmnList;
	ifstream pkmnMoves;

	checkData(pkmnList, pkmnMoves);
	cout << "Welcome to the world of Pokemon!\n";
	
	printLongSep();
	cout << "Getting your Pokemon ready...\n";
	Pkmn pkmn1(pkmnList, pkmnMoves);
    
    printLongSep();
    cout << "Getting the opponent Pokemon ready...\n";
    Pkmn pkmn2(pkmnList, pkmnMoves);
	
	printLongSep();
	cout << "A wild " << pkmn2.name << " appeared!\n";
	cout << "Go! " << pkmn1.name << "!\n";
	
	/*
	bool PKMN2FIRST;
	bool inBattle=true;

	PKMN2F3:
	while (pkmn2.HP>0 && pkmn1.HP>0){
		int ind;

		cout << "--------------------------------------------------" << endl;
		cout << pkmn1.name << endl;
		cout << "HP: " << pkmn1.HP << " ATK: " << pkmn1.ATK << " DEF: " << pkmn1.DEF << " SATK: " << pkmn1.SATK << " SDEF: " << pkmn1.SDEF << " SPD: " << pkmn1.SPD << endl;
		cout << "--------------------------------------------------" << endl;
		cout << pkmn2.name << " (WILD)" << endl;
		cout << "HP: " << pkmn2.HP << " ATK: " << pkmn2.ATK << " DEF: " << pkmn2.DEF << " SATK: " << pkmn2.SATK << " SDEF: " << pkmn2.SDEF << " SPD: " << pkmn2.SPD << endl;
		cout << "--------------------------------------------------" << endl << endl;
		
		if (pkmn2.SPD>pkmn1.SPD) {
			PKMN2FIRST=true;
			goto PKMN2F1;
		}
		PKMN2F2:
		
		if(pkmn1.HP>0) {
			cout << "What will " << pkmn1.name << " do?" << endl;
			
			cout << "~-~-~-~-~-~-~-~-~-~-~-~-~" << endl;
			cout << pkmn1.name << "'s Moveset:\n";
			cout << " [#]  Move\n";
			for (int i=0; i<4; ++i) cout << " [" << i << "]  " << pkmn1.move[i] << "    PP: " << pkmn1.movePP[i] << "/" << pkmn1.movebasePP[i] << endl;
			cout << "~-~-~-~-~-~-~-~-~-~-~-~-~" << endl;
			
			ind=0;
			do { 
				if (ind > 3 || ind < 0) cout << "Input invalid, please enter the index of a valid move.\n";
				cin >> ind;
			} while(ind > 3 || ind < 0);
			
			if(pkmn1.movePP[ind]>0){ //checks if move still has PP
				cout << endl << pkmn1.name << " used " << pkmn1.move[ind] << "!" << endl;
				--pkmn1.movePP[ind];
			}
			else if(pkmn1.movePP[0]==0 && pkmn1.movePP[1]==0 && pkmn1.movePP[2]==0 && pkmn1.movePP[3]==0){ //checks if all moves are out of PP
				cout << pkmn1.name << " has no moves left!" << endl;
				cout << pkmn1.name << " used Struggle!" << endl;
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, 50, pkmn1.ATK, pkmn2.DEF);
				cout << pkmn1.name << " is hit with recoil!" << endl;
				pkmn1.HP-=(pkmn1.dmg(pkmn1.lvl, 50, pkmn1.ATK, pkmn2.DEF))/2;
			}
			else{
				cout << pkmn1.name << " tries to use " << pkmn1.move[ind] << "..." << endl;
				cout << "...But " << pkmn1.move[ind] << "is out of PP!" << endl;
				goto PKMN2F1;
			}
			
			//accuracy check
			if (rand() % 100 + 1 > (pkmn1.moveACC[ind]*(pkmn1.ACC/pkmn2.EVA))){
				cout << pkmn1.name << " missed!" << endl;
				goto PKMN2F1;
			}
			
			//checks moveCat
			string cat=pkmn1.moveCat[ind];
			
			if(cat=="PHYSICAL"){
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
			}
			
			else if(cat=="SPECIAL"){
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.SATK, pkmn2.SDEF);
			}
			
			else{ //Status
				if (cat=="ATKO"){ pkmn2.ATKmult=pkmn2.modMult(pkmn2.ATKmult, pkmn1.movePWR[ind]); pkmn2.ATK=pkmn2.modStat(pkmn2.baseATK, pkmn2.ATKmult);}
				else if (cat=="DEFO"){ pkmn2.DEFmult=pkmn2.modMult(pkmn2.DEFmult, pkmn1.movePWR[ind]); pkmn2.DEF=pkmn2.modStat(pkmn2.baseDEF, pkmn2.DEFmult);}
				else if (cat=="SATKO"){ pkmn2.SATKmult=pkmn2.modMult(pkmn2.SATKmult, pkmn1.movePWR[ind]); pkmn2.SATK=pkmn2.modStat(pkmn2.baseSATK, pkmn2.SATKmult);}
				else if (cat=="SDEFO"){ pkmn2.SDEFmult=pkmn2.modMult(pkmn2.SDEFmult, pkmn1.movePWR[ind]); pkmn2.SDEF=pkmn2.modStat(pkmn2.baseSDEF, pkmn2.SDEFmult);}
				else if (cat=="SPDO"){ pkmn2.SPDmult=pkmn2.modMult(pkmn2.SPDmult, pkmn1.movePWR[ind]); pkmn2.SPD=pkmn2.modStat(pkmn2.baseSPD, pkmn2.SPDmult);}
				else if (cat=="ATKS"){ pkmn1.ATKmult=pkmn1.modMult(pkmn1.ATKmult, pkmn1.movePWR[ind]); pkmn1.ATK=pkmn1.modStat(pkmn1.baseATK, pkmn1.ATKmult);}
				else if (cat=="DEFS"){ pkmn1.DEFmult=pkmn1.modMult(pkmn1.DEFmult, pkmn1.movePWR[ind]); pkmn1.DEF=pkmn1.modStat(pkmn1.baseDEF, pkmn1.DEFmult);}
				else if (cat=="SATKS"){ pkmn1.SATKmult=pkmn1.modMult(pkmn1.SATKmult, pkmn1.movePWR[ind]); pkmn1.SATK=pkmn1.modStat(pkmn1.baseSATK, pkmn1.SATKmult);}
				else if (cat=="SDEFS"){ pkmn1.SDEFmult=pkmn1.modMult(pkmn1.SDEFmult, pkmn1.movePWR[ind]); pkmn1.SDEF=pkmn1.modStat(pkmn1.baseSDEF, pkmn1.SDEFmult);}
				else if (cat=="SPDS"){ pkmn1.SPDmult=pkmn1.modMult(pkmn1.SPDmult, pkmn1.movePWR[ind]); pkmn1.SPD=pkmn1.modStat(pkmn1.baseSPD, pkmn1.SPDmult);}
				else if (cat=="OHKO") pkmn2.HP=0;
				else if (cat=="FLEE"){ inBattle=false; goto flee; }
				else if (cat=="SAME"){ pkmn2.HP-=pkmn1.movePWR[ind]; }
				else if (cat=="ACCS"){
					pkmn1.ACCmult+=pkmn1.movePWR[ind];
					pkmn1.ACC=pkmn1.setAEStat(pkmn1.ACCmult);
				}
				else if (cat=="EVAS"){
					pkmn1.EVAmult+=pkmn1.movePWR[ind];
					pkmn1.EVA=pkmn1.setAEStat(pkmn1.EVAmult);
				}
				else if (cat=="ACCO"){
					pkmn2.ACCmult+=pkmn1.movePWR[ind];
					pkmn2.ACC=pkmn2.setAEStat(pkmn2.ACCmult);
				}
				else if (cat=="EVAO"){
					pkmn2.EVAmult+=pkmn1.movePWR[ind];
					pkmn2.EVA=pkmn2.setAEStat(pkmn2.EVAmult);
				}
				else if (cat=="PREPR"){
					int k=(rand() % pkmn1.moveRep2[ind] + pkmn1.moveRep1[ind]);
					for(int j=0; j<k; ++j) {
						pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
					}
					cout << pkmn1.move[ind] << " hit " << k << " times!" << endl;
				}
				else if (cat=="PREP"){
					for(int j=0; j<pkmn1.moveRep1[ind]; ++j) {
						pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
					}
					cout << pkmn1.move[ind] << " hit " << pkmn1.moveRep1[ind] << " times!" << endl;
				}
			}
		}
		
		if (PKMN2FIRST) goto PKMN2F3;
		PKMN2F1:
		
		if(pkmn2.HP>0) {
			
			do {
				ind=rand() % 4;
			} while (pkmn2.move[ind] == "-");
			
			cout << pkmn2.name << " used " << pkmn2.move[ind] << "!" << endl;
			string cat=pkmn2.moveCat[ind];
			
			if(cat=="PHYSICAL"){
				pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePWR[ind], pkmn2.ATK, pkmn1.DEF);
			}
			
			else if(cat=="SPECIAL"){
				pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePWR[ind], pkmn2.SATK, pkmn1.SDEF);
			}
			
			else{ //Status
				if (cat=="ATKO"){ pkmn1.ATKmult=pkmn1.modMult(pkmn1.ATKmult, pkmn2.movePWR[ind]); pkmn1.ATK=pkmn1.modStat(pkmn1.baseATK, pkmn1.ATKmult);}
				else if (cat=="DEFO"){ pkmn1.DEFmult=pkmn1.modMult(pkmn1.DEFmult, pkmn2.movePWR[ind]); pkmn1.DEF=pkmn1.modStat(pkmn1.baseDEF, pkmn1.DEFmult);}
				else if (cat=="SATKO"){ pkmn1.SATKmult=pkmn1.modMult(pkmn1.SATKmult, pkmn2.movePWR[ind]); pkmn1.SATK=pkmn1.modStat(pkmn1.baseSATK, pkmn1.SATKmult);}
				else if (cat=="SDEFO"){ pkmn1.SDEFmult=pkmn1.modMult(pkmn1.SDEFmult, pkmn2.movePWR[ind]); pkmn1.SDEF=pkmn1.modStat(pkmn1.baseSDEF, pkmn1.SDEFmult);}
				else if (cat=="SPDO"){ pkmn1.SPDmult=pkmn1.modMult(pkmn1.SPDmult, pkmn2.movePWR[ind]); pkmn1.SPD=pkmn1.modStat(pkmn1.baseSPD, pkmn1.SPDmult);}
				else if (cat=="ATKS"){ pkmn2.ATKmult=pkmn2.modMult(pkmn2.ATKmult, pkmn2.movePWR[ind]); pkmn2.ATK=pkmn2.modStat(pkmn2.baseATK, pkmn2.ATKmult);}
				else if (cat=="DEFS"){ pkmn2.DEFmult=pkmn2.modMult(pkmn2.DEFmult, pkmn2.movePWR[ind]); pkmn2.DEF=pkmn2.modStat(pkmn2.baseDEF, pkmn2.DEFmult);}
				else if (cat=="SATKS"){ pkmn2.SATKmult=pkmn2.modMult(pkmn2.SATKmult, pkmn2.movePWR[ind]); pkmn2.SATK=pkmn2.modStat(pkmn2.baseSATK, pkmn2.SATKmult);}
				else if (cat=="SDEFS"){ pkmn2.SDEFmult=pkmn2.modMult(pkmn2.SDEFmult, pkmn2.movePWR[ind]); pkmn2.SDEF=pkmn2.modStat(pkmn2.baseSDEF, pkmn2.SDEFmult);}
				else if (cat=="SPDS"){ pkmn2.SPDmult=pkmn2.modMult(pkmn2.SPDmult, pkmn2.movePWR[ind]); pkmn2.SPD=pkmn2.modStat(pkmn2.baseSPD, pkmn2.SPDmult);}
				else if (cat=="OHKO") pkmn1.HP=0;
				else if (cat=="FLEE"){ inBattle=false; goto flee; }
				else if (cat=="SAME"){ pkmn1.HP-=pkmn2.movePWR[ind]; }
				else if (cat=="ACCS"){
					pkmn2.ACCmult+=pkmn2.movePWR[ind];
					pkmn2.ACC=pkmn2.setAEStat(pkmn2.ACCmult);
				}
				else if (cat=="EVAS"){
					pkmn2.EVAmult+=pkmn2.movePWR[ind];
					pkmn2.EVA=pkmn2.setAEStat(pkmn2.EVAmult);
				}
				else if (cat=="ACCO"){
					pkmn1.ACCmult+=pkmn2.movePWR[ind];
					pkmn1.ACC=pkmn1.setAEStat(pkmn1.ACCmult);
				}
				else if (cat=="EVAO"){
					pkmn1.EVAmult+=pkmn2.movePWR[ind];
					pkmn1.EVA=pkmn1.setAEStat(pkmn1.EVAmult);
				}
				else if (cat=="PREPR"){
					int k=(rand() % pkmn2.moveRep2[ind] + pkmn2.moveRep1[ind]);
					for(int j=0; j<k; ++j) {
						pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePWR[ind], pkmn2.ATK, pkmn1.DEF);
					}
					cout << pkmn2.move[ind] << " hit " << k << " times!" << endl;
				}
				else if (cat=="PREP"){
					for(int j=0; j<pkmn2.moveRep1[ind]; ++j) {
						pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePWR[ind], pkmn2.ATK, pkmn1.DEF);
					}
					cout << pkmn2.move[ind] << " hit " << pkmn2.moveRep1[ind] << " times!" << endl;
				}
			}
		}
		
		if (PKMN2FIRST) goto PKMN2F2;
	}
	
	if (pkmn2.HP<=0){
		cout << "The wild " << pkmn2.name << " fainted!" << endl;
	}
	else{
		cout << pkmn1.name << " fainted!" << endl;
	}
	
	flee:
		if(!inBattle) cout << "The battle has ended!" << endl;
	*/
}
