#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include "pkmn.h"
using namespace std;

//check if name is valid
bool Pkmn::checkValidName(string tmpName, ifstream& pkmnList) {
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
string Pkmn::getValidName(ifstream& pkmnList) {
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
bool Pkmn::checkValidLvl(string tmpLvl) {
	//lvl must be within 1-100
	if (tmpLvl.find_first_not_of("1234567890") != std::string::npos || !(stoi(tmpLvl) > 0 && stoi(tmpLvl) < 101)) {
		cout << "Level '" << tmpLvl << "' doesn't seem right.\n";
		cout << "\n";
		return false;
	}
	
	return true;
}

//prompt user for valid lvl
int Pkmn::getValidLvl() {
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
bool Pkmn::checkValidMove(string tmpMove, ifstream& pkmnMoves, bool inBattle) {
	string* moveIt = find(move, move + 4, tmpMove);
	
	//if move not all alphabetical char
	if (tmpMove.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ '-") != std::string::npos) {
		cout << "Hey there! You probably used some weird number or punctuation. Try again?\n";
		cout << "\n";
		return false;
	}
	
	//if move already known
	else if (moveIt != move + 4) {
		//if choosing moves, invalid if move is already known
		if (!inBattle) {
			cout << "This Pokemon already knows " << tmpMove << "!\n";
			cout << "\n";
			return false;
		}
		//if in battle, valid if move is in moveset & PP > 0
		else if (inBattle) {
			return true;
		}
	}
	
	//if move not known
	else {
		//if choosing moves, find in move list
		if (!inBattle) {
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
		//if in battle, invalid if not known
		else if (inBattle) {
			cout << "Please input a known move.\n";
			return false;
		}
	}
}

//prompt user for valid move
string Pkmn::getValidMove(ifstream& pkmnMoves, bool inBattle) {
	string tmpMove;
	bool validMove = false;
	
	while (!validMove) {
		if (!inBattle) cout << "Please input any valid move for " << name << ".\n";
		cout << "YOU: ";
		getline (cin, tmpMove);
		cout << "\n";
		tmpMove = capitalize(tmpMove);
		validMove = checkValidMove(tmpMove, pkmnMoves, inBattle);
	}
	
	if (!inBattle) cout << name << " learned " << tmpMove << "!\n";
	return tmpMove;
}

//compute HP with base HP and lvl
int Pkmn::setHP(ifstream& pkmnList) {
	string tmpLine;
	getline (pkmnList, tmpLine, ',');
	
	int bHP = stoi(tmpLine);
	int HP = static_cast<int>(((2*bHP*lvl)/100) + 10 + lvl);
	
	return HP;
}
		
//compute stat with base stat and lvl
int Pkmn::setStat(ifstream& pkmnList) {
	string tmpLine;
	getline (pkmnList, tmpLine, ',');
	
	int bStat = stoi(tmpLine);
	int stat = static_cast<int>(((2*bStat*lvl)/100) + 5);
	
	return stat;
}
		
//compute move damage with attacker's lvl, movePWR, attack, and opponent's defense
int Pkmn::dmg(int lvl, int PWR, int ATK, int DEF) {
	int finalDmg = static_cast<int>((2*lvl/5+2)*PWR*ATK/DEF/50+2);
	return finalDmg;
}
		
//modify pkmn stat multipliers with stat-changing moves
int Pkmn::modMult(int statMult, int moveMult) {
	statMult += moveMult;
	
	//if statMult is too high or too low
	if (statMult < -6) 		{statMult = -6;}
	else if (statMult > 6) 	{statMult = 6;}
	
	return statMult;
}

//modify pkmn effective stat with stage multipliers
int Pkmn::modStat(int bstat, int statMult) {
	//formula based on stat multipliers
	//stat multipliers table from https://bulbapedia.bulbagarden.net/wiki/Statistic#Stage_multipliers
	int finalStat = bstat * max(2, 2 + statMult)/max(2, 2-statMult);
	return finalStat;
}

void Pkmn::printPkmnMoves() {
	printCurlySep();
	cout << name << "'s Moveset:\n";
	cout << " [#]  Move\n";
	for (int i = 0; i < 4; ++i) {
		cout << " [" << i << "]  " << move[i] << "    PP: " << movePP[i] << "/" << movebasePP[i] << "\n";
	}
	printCurlySep();
}

//DEBUG TOOLS: print pkmn info
void Pkmn::debug() {
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "DEBUG INFO:" << endl;
	cout << name << endl;
	cout << lvl << endl;
	cout << HP << endl;
	cout << baseATK << endl;
	cout << baseDEF << endl;
	cout << baseSATK << endl;
	cout << baseSDEF << endl;
	cout << baseSPD << endl;
	cout << HP << endl;
	cout << ATK << endl;
	cout << DEF << endl;
	cout << SATK << endl;
	cout << SDEF << endl;
	cout << SPD << endl;
	printPkmnMoves();
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

Pkmn::Pkmn(ifstream& pkmnList, ifstream& pkmnMoves) {
	//set pkmn
	pkmnList.open("pkmn-data/pkmnList.txt");
	
	printShortSep();
	name = getValidName(pkmnList);
	printShortSep();
	lvl = getValidLvl();
	
	type1 = getNext(pkmnList);
	type2 = getNext(pkmnList);
	
	HP = setHP(pkmnList);
	ATK = setStat(pkmnList);
	baseATK = ATK;
	DEF = setStat(pkmnList);
	baseDEF = DEF;
	SATK = setStat(pkmnList);
	baseSATK = SATK;
	SDEF = setStat(pkmnList);
	baseSDEF = SDEF;
	SPD = setStat(pkmnList);
	baseSPD = SPD;
	
	pkmnList.close();
    
    //set pkmn moveset
    pkmnMoves.open("pkmn-data/pkmnMoves.txt");
    
	for (int i = 0; i < 4; ++i) {
		printShortSep();
		move[i] = getValidMove(pkmnMoves, false);
	    moveType[i] = getNext(pkmnMoves);
	    moveCat[i] = getNext(pkmnMoves);
		movebasePP[i] = stoi(getNext(pkmnMoves));
		movePP[i] = movebasePP[i];
		movePWR[i] = stoi(getNext(pkmnMoves));
		moveACC[i] = stoi(getNext(pkmnMoves));
				
		if (moveCat[i] == "PREPR" || moveCat[i] == "PREP") {
			moveMinRep[i] = stoi(getNext(pkmnMoves));
			if (moveCat[i] == "PREPR") {
					moveMaxRep[i] = stoi(getNext(pkmnMoves)) -1;
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

//prints both pokemon stats
void printBothPkmnInfo(Pkmn pkmn1, Pkmn pkmn2) {
	cout << "\n";
	printShortSep();
	cout << pkmn1.name << " / LVL " << pkmn1.lvl << "\n";
	cout << "HP: " << pkmn1.HP << " / ATK: " << pkmn1.ATK << " / DEF: " << pkmn1.DEF << " / SATK: " << pkmn1.SATK << " / SDEF: " << pkmn1.SDEF << " / SPD: " << pkmn1.SPD << "\n";
	printShortSep();
	cout << pkmn2.name << " / LVL " << pkmn2.lvl << " (WILD)\n";
	cout << "HP: " << pkmn2.HP << " / ATK: " << pkmn2.ATK << " / DEF: " << pkmn2.DEF << " / SATK: " << pkmn2.SATK << " / SDEF: " << pkmn2.SDEF << " / SPD: " << pkmn2.SPD << "\n";
	printShortSep();
	cout << "\n";
}

//changes pkmn stats based on move
//pkmn1 - attacker, pkmn2 - receiver
void moveEffect(Pkmn& pkmn1, Pkmn& pkmn2, int ind, bool& inBattle) {
	string cat = pkmn1.moveCat[ind];
	/*
	Columns for pkmnMoves.txt:

	PHYSICAL, SPECIAL, SAME Moves:
	Index, Move, Type, Category, PP, Power, Accuracy
	
	Stat-changing Moves (ATKO, DEFO, ACCO, SATKO, SDEFO, SPDO, ACCO, EVAO, ATKS, DEFS, ACCS, SATKS, SDEFS, SPDS, ACCS, EVAS):
	Index, Move, Type, Category, PP, Stage Multiplier, Accuracy
	
	Repeating moves (PREPR):
	Index, Move, Type, Category, PP, Power, Accuracy, Min Repetitions, Max Repetitions
	
	//!!!CHECK EVA STAT MODIFIERS: SHOULD BE OPPOSITE ACC
	*/
	if (cat == "PHYSICAL")		{pkmn2.HP -= pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);}
	else if (cat == "SPECIAL")	{pkmn2.HP -= pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.SATK, pkmn2.SDEF);}
	else { //Status Moves
	
		//change stat of other pokemon
		if (cat == "ATKO")		{pkmn2.ATKmult = pkmn2.modMult(pkmn2.ATKmult, pkmn1.movePWR[ind]);	pkmn2.ATK = pkmn2.modStat(pkmn2.baseATK, pkmn2.ATKmult);}
		else if (cat == "DEFO")	{pkmn2.DEFmult = pkmn2.modMult(pkmn2.DEFmult, pkmn1.movePWR[ind]);	pkmn2.DEF = pkmn2.modStat(pkmn2.baseDEF, pkmn2.DEFmult);}
		else if (cat == "SATKO"){pkmn2.SATKmult = pkmn2.modMult(pkmn2.SATKmult, pkmn1.movePWR[ind]);pkmn2.SATK = pkmn2.modStat(pkmn2.baseSATK, pkmn2.SATKmult);}
		else if (cat == "SDEFO"){pkmn2.SDEFmult = pkmn2.modMult(pkmn2.SDEFmult, pkmn1.movePWR[ind]);pkmn2.SDEF = pkmn2.modStat(pkmn2.baseSDEF, pkmn2.SDEFmult);}
		else if (cat == "SPDO")	{pkmn2.SPDmult = pkmn2.modMult(pkmn2.SPDmult, pkmn1.movePWR[ind]); 	pkmn2.SPD = pkmn2.modStat(pkmn2.baseSPD, pkmn2.SPDmult);}
		else if (cat == "ACCO")	{pkmn2.ACCmult = pkmn2.modMult(pkmn2.ACCmult, pkmn1.movePWR[ind]);	pkmn2.ACC = pkmn2.modStat(pkmn2.ACC, pkmn2.ACCmult);}
		else if (cat == "EVAO")	{pkmn2.EVAmult = pkmn2.modMult(pkmn2.EVAmult, pkmn1.movePWR[ind]);	pkmn2.EVA = pkmn2.modStat(pkmn2.EVA, pkmn2.EVAmult);}
		
		//change stat of self
		else if (cat == "ATKS")	{pkmn1.ATKmult = pkmn1.modMult(pkmn1.ATKmult, pkmn1.movePWR[ind]); 	pkmn1.ATK = pkmn1.modStat(pkmn1.baseATK, pkmn1.ATKmult);}
		else if (cat == "DEFS")	{pkmn1.DEFmult = pkmn1.modMult(pkmn1.DEFmult, pkmn1.movePWR[ind]); 	pkmn1.DEF = pkmn1.modStat(pkmn1.baseDEF, pkmn1.DEFmult);}
		else if (cat == "SATKS"){pkmn1.SATKmult = pkmn1.modMult(pkmn1.SATKmult, pkmn1.movePWR[ind]);pkmn1.SATK = pkmn1.modStat(pkmn1.baseSATK, pkmn1.SATKmult);}
		else if (cat == "SDEFS"){pkmn1.SDEFmult = pkmn1.modMult(pkmn1.SDEFmult, pkmn1.movePWR[ind]);pkmn1.SDEF = pkmn1.modStat(pkmn1.baseSDEF, pkmn1.SDEFmult);}
		else if (cat == "SPDS")	{pkmn1.SPDmult = pkmn1.modMult(pkmn1.SPDmult, pkmn1.movePWR[ind]);	pkmn1.SPD = pkmn1.modStat(pkmn1.baseSPD, pkmn1.SPDmult);}
		else if (cat == "ACCS")	{pkmn1.ACCmult = pkmn1.modMult(pkmn1.ACCmult, pkmn1.movePWR[ind]);	pkmn1.ACC = pkmn1.modStat(pkmn1.ACC, pkmn1.ACCmult);}
		else if (cat == "EVAS")	{pkmn1.EVAmult = pkmn1.modMult(pkmn1.EVAmult, pkmn1.movePWR[ind]);	pkmn1.EVA = pkmn1.modStat(pkmn1.EVA, pkmn1.EVAmult);}
		
		//instantly ends battle
		else if (cat == "OHKO") {pkmn2.HP = 0;}
		else if (cat == "FLEE")	{inBattle = false;} //add battle end message here
		
		//moves whose damage is always constant
		else if (cat == "SAME")	{pkmn2.HP -= pkmn1.movePWR[ind]; if(pkmn1.movePWR[ind] == 0) {cout << "But nothing happened!\n";}}
		
		//moves that hit multiple times
		/*
		else if (cat=="PREPR"){
			int k=(rand() % pkmn1.moveRep2[ind] + pkmn1.moveRep1[ind]);
			for(int j=0; j<k; ++j) {
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
			}
			cout << pkmn1.move[ind] << " hit " << k << " times!" << endl;
		}
		
		else if (cat == "PREP"){
			for(int j=0; j<pkmn1.moveRep1[ind]; ++j) {
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
			}
			cout << pkmn1.move[ind] << " hit " << pkmn1.moveRep1[ind] << " times!" << endl;
		}
		*/
	}
}

bool inBattleCheck(Pkmn pkmn1, Pkmn pkmn2, bool inBattle) {
	if (pkmn1.HP <= 0) { 
		cout << pkmn1.name << " fainted!\n";
		return false;
	}
	else if (pkmn2.HP <= 0) { 
		cout << "The wild " << pkmn2.name << " fainted!\n";
		return false;
	}
	return inBattle;
}

/* 
//MinGW does not currently support threading
void wait() {
    this_thread::sleep_for(std::chrono::milliseconds(5));
}
*/

bool struggleCheck(Pkmn pkmn) {
	if (all_of(pkmn.movePP,pkmn.movePP + 4, [](int currMovePP){return currMovePP == 0;})) {
		return true;
	}
	else {
		return false;
	}
}

bool Pkmn::PPCheck(int moveIndex) {
	if (movePP[moveIndex] > 0) {
		--movePP[moveIndex];
		return true;
	}
	else {
		cout << name << " tries to use " << move[moveIndex] << "...\n";
		cout << "...But " << move[moveIndex] << " is out of PP!\n";
		return false;
	}
}

void Pkmn::struggle(Pkmn& oppPkmn) {
	cout << name << " has no moves left!\n";
	cout << name << " used Struggle!\n";
	int struggleDmg = dmg(lvl, 50, ATK, oppPkmn.DEF);
	oppPkmn.HP -= struggleDmg;
	cout << name << " is hit with recoil!\n";
	HP -= struggleDmg/2;
}

void pkmn1Move(Pkmn& pkmn1, Pkmn& pkmn2, bool& inBattle, ifstream& pkmnMoves) {
	pkmn1.printPkmnMoves();
	
	//if any move still has PP, prompt for move
	if (!struggleCheck(pkmn1)) {
		cout << "What should " << pkmn1.name << " do?\n";
	
		int moveIndex = distance(pkmn1.move, find(pkmn1.move, pkmn1.move + 4, pkmn1.getValidMove(pkmnMoves, true)));
		
		if (pkmn1.PPCheck(moveIndex)) {cout << pkmn1.name << " used " << pkmn1.move[moveIndex] << "!\n";}
		moveEffect(pkmn1, pkmn2, moveIndex, inBattle);
	}
	//if no PP, struggle
	else {
		pkmn1.struggle(pkmn2);
	}
}

void pkmn2Move(Pkmn& pkmn2, Pkmn& pkmn1, bool& inBattle) {
	
	//if any move still has PP, prompt for move
	if (!struggleCheck(pkmn2)) {
		
		//get index for random move
		srand(time(0));
		int moveIndex = rand() % 4;	
		
		if (pkmn2.PPCheck(moveIndex)) {cout << "The wild " << pkmn2.name << " used " << pkmn2.move[moveIndex] << "!\n";}
		moveEffect(pkmn2, pkmn1, moveIndex, inBattle);
	}
	//if no PP, struggle
	else {
		pkmn2.struggle(pkmn1);
	}
}

bool replayCheck() {
	string s;
	cout << "Play again? Enter Y to replay and anything else to exit.\n";
	cout << "YOU: ";
	getline(cin,s);
	cout << "\n";
	if (s == "Y" || s == "y") {return true;}
	else {return false;}
}
