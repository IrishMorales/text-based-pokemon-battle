#include <iostream>
#include <algorithm>
#include "pkmn.h"
using namespace std;

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

void Pkmn::printPkmnInfo() {
	cout << name << "\n";
	cout << "HP: " << HP << " ATK: " << ATK << " DEF: " << DEF << " SATK: " << SATK << " SDEF: " << SDEF << " SPD: " << SPD << "\n";
}

void Pkmn::printPkmnMoves(Pkmn pkmn) {
	printCurlySep();
	cout << pkmn.name << "'s Moveset:\n";
	cout << " [#]  Move\n";
	for (int i = 0; i < 4; ++i) {
		cout << " [" << i << "]  " << pkmn.move[i] << "    PP: " << pkmn.movePP[i] << "/" << pkmn.movebasePP[i] << "\n";
	}
	printCurlySep();
}

//DEBUG TOOLS: print pkmn info
void Pkmn::debug(Pkmn pkmn1, Pkmn pkmn2) {
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
		move[i] = getValidMove(name, pkmnMoves, move, false);
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

//non-member functions

//capitalize entire string
string capitalize(string s) {
	for (unsigned int i = 0; i < s.length(); ++i) s[i]=toupper(s[i]);
	return s;
}

//print separators
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

//get next item from stream
string getNext(ifstream& file) {
	string tmpLine;
	getline (file, tmpLine, ',');
	return tmpLine;
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

//check if move is valid
bool checkValidMove(string tmpMove, ifstream& pkmnMoves, string move[4], bool inBattle) {
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
string getValidMove(string name, ifstream& pkmnMoves, string move[4], bool inBattle) {
	string tmpMove;
	bool validMove = false;
	
	while (!validMove) {
		cout << "Please input any valid move for " << name << ".\n";
		cout << "YOU: ";
		getline (cin, tmpMove);
		cout << "\n";
		tmpMove = capitalize(tmpMove);
		validMove = checkValidMove(tmpMove, pkmnMoves, move, inBattle);
	}
	
	cout << name << " learned " << tmpMove << "!\n";
	return tmpMove;
}
