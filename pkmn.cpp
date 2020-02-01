#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include "pkmn.h"

//check if name is valid
bool Pkmn::checkValidName(std::string tmpName, std::ifstream& pkmnList) {
	//if name not all alphabetical char
	if (tmpName.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) {
		std::cout << "That's one weird Pokemon. Try again?\n";
		std::cout << "\n";
		return false;
	}
	
	else {
		returnToBegin(pkmnList);
		std::string tmpLine;
		std::cout << "Finding a wild " << tmpName << "...\n";
		
		//search pkmnList for name
		while (std::getline(pkmnList, tmpLine, ',')) {	
	        if (tmpLine == tmpName) {
	        	std::cout << "Found " << tmpName << "!\n";
	        	return true;
	    	}
		}
		
		//if name not found in pkmnList.txt
		std::cout << "Looks like " << tmpName << " isn't a Pokemon. :(\n";
		std::cout << "TIPS: Check spelling or see pkmnList.txt in folder pkmn-data for valid Pokemon.\n";
		std::cout << "\n";
		return false;
	}
}

//prompt user for valid name
std::string Pkmn::getValidName(std::ifstream& pkmnList) {
	std::string tmpName;
	bool validName = false;
	
	while (!validName) {
		std::cout << "Please input a Pokemon.\n";
		std::cout << "YOU: ";
		std::getline (std::cin, tmpName);
		std::cout << "\n";
		tmpName = capitalize(tmpName);
		validName = checkValidName(tmpName, pkmnList);
	}
	return tmpName;
}

//check if lvl is valid
bool Pkmn::checkValidLvl(std::string tmpLvl) {
	//lvl must be within 1-100
	if (tmpLvl.find_first_not_of("1234567890") != std::string::npos || !(stoi(tmpLvl) > 0 && stoi(tmpLvl) < 101)) {
		std::cout << "Level '" << tmpLvl << "' doesn't seem right.\n";
		std::cout << "\n";
		return false;
	}
	
	std::cout << "Level " << stoi(tmpLvl) << " set!\n";
	return true;
}

//prompt user for valid lvl
int Pkmn::getValidLvl() {
	std::string tmpLvl;
	bool validLvl = false;
	
	std::cout << "What level will " << name << " be?\n";
	
	while (!validLvl) {
		std::cout << "Please input a level within 1-100.\n";
		std::cout << "YOU: ";
		std::getline (std::cin, tmpLvl);
		std::cout << "\n";
		validLvl = checkValidLvl(tmpLvl);
	}
	
	return stoi(tmpLvl);
}

//check if move is valid
bool Pkmn::checkValidMove(std::string tmpMove, std::ifstream& pkmnMoves, bool inBattle) {
	std::string* moveIt = find(move, move + 4, tmpMove);
	
	//if move not all alphabetical char
	if (tmpMove.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ '-") != std::string::npos) {
		std::cout << "Hey there! You probably used some weird number or punctuation. Try again?\n";
		std::cout << "\n";
		return false;
	}
	
	//if move already known
	else if (moveIt != move + 4) {
		//if choosing moves, invalid if move is already known
		if (!inBattle) {
			std::cout << "This Pokemon already knows " << tmpMove << "!\n";
			std::cout << "\n";
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
			std::string tmpLine;
				
			//search pkmnMoves for move
			while (std::getline(pkmnMoves, tmpLine, ',')) {	
			    if (tmpLine == tmpMove) {
			    	return true;
				}
			}
				
			//if move not found in pkmnMoves.txt
			std::cout << "Sorry, we couldn't find that! :(\n";
			std::cout << "TIPS: Check spelling or see pkmnMoves.txt in folder pkmn-data for valid moves.\n";
			std::cout << "\n";
			return false;
		}
		//if in battle, invalid if not known
		else if (inBattle) {
			std::cout << "Please input a known move.\n";
			return false;
		}
	}
}

//prompt user for valid move
std::string Pkmn::getValidMove(std::ifstream& pkmnMoves, bool inBattle) {
	std::string tmpMove;
	bool validMove = false;
	
	while (!validMove) {
		if (!inBattle) std::cout << "Please input any valid move for " << name << ".\n";
		std::cout << "YOU: ";
		std::getline (std::cin, tmpMove);
		std::cout << "\n";
		tmpMove = capitalize(tmpMove);
		validMove = checkValidMove(tmpMove, pkmnMoves, inBattle);
	}
	
	if (!inBattle) std::cout << name << " learned " << tmpMove << "!\n";
	return tmpMove;
}

//compute HP with base HP and lvl
int Pkmn::setHP(std::ifstream& pkmnList) {
	std::string tmpLine;
	std::getline (pkmnList, tmpLine, ',');
	
	int bHP = stoi(tmpLine);
	int HP = static_cast<int>(((2*bHP*lvl)/100) + 10 + lvl);
	
	return HP;
}
		
//compute stat with base stat and lvl
int Pkmn::setStat(std::ifstream& pkmnList) {
	std::string tmpLine;
	std::getline (pkmnList, tmpLine, ',');
	
	int bStat = stoi(tmpLine);
	int stat = static_cast<int>(((2*bStat*lvl)/100) + 5);
	
	return stat;
}

bool Pkmn::compareSPD(Pkmn& pkmnB) {
	if (SPD > pkmnB.SPD) {return true;}
	else {return false;}
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
	int finalStat = bstat * std::max(2, 2 + statMult)/std::max(2, 2-statMult);
	return finalStat;
}

void Pkmn::printPkmnMoves() {
	printCurlySep();
	std::cout << name << "'s Moveset:\n";
	std::cout << " [#]  Move\n";
	for (int i = 0; i < 4; ++i) {
		std::cout << " [" << i << "]  " << move[i] << "    PP: " << movePP[i] << "/" << movebasePP[i] << "\n";
	}
	printCurlySep();
}

//DEBUG TOOLS: print pkmn info
void Pkmn::debug() {
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "DEBUG INFO:" << std::endl;
	std::cout << name << std::endl;
	std::cout << lvl << std::endl;
	std::cout << HP << std::endl;
	std::cout << baseATK << std::endl;
	std::cout << baseDEF << std::endl;
	std::cout << baseSATK << std::endl;
	std::cout << baseSDEF << std::endl;
	std::cout << baseSPD << std::endl;
	std::cout << HP << std::endl;
	std::cout << ATK << std::endl;
	std::cout << DEF << std::endl;
	std::cout << SATK << std::endl;
	std::cout << SDEF << std::endl;
	std::cout << SPD << std::endl;
	printPkmnMoves();
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

Pkmn::Pkmn(std::ifstream& pkmnList, std::ifstream& pkmnMoves) {
	//set pkmn
	pkmnList.open("pkmn-data/pkmnList.txt");
	
	printShortSep();
	name = getValidName(pkmnList);
	printShortSep();
	lvl = getValidLvl();
	
	type1 = getNext(pkmnList);
	type2 = getNext(pkmnList);
	
	HP = setHP(pkmnList);
	baseHP = HP;
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
	
	std::cout << name << " is ready!\n";
}

//capitalize entire std::string
std::string capitalize(std::string s) {
	for (unsigned int i = 0; i < s.length(); ++i) s[i]=toupper(s[i]);
	return s;
}

//print separators
void printLongSep() {
	std::cout << "=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=-~-=\n";
}

void printShortSep() {
	std::cout << "-----------------------------------\n";
}

void printCurlySep() {
	std::cout << "~-~-~-~-~-~-~-~-~-~-~-~-~\n";
}

void printWelcome() {
	std::cout << "________      ______                                    ________       ________________\n"
		"___  __ \\________  /_____________ ________________      ___  __ )_____ __  /__  /___  /____\n"
		"__  /_/ /  __ \\_  //_/  _ \\_  __ `__ \\  __ \\_  __ \\     __  __  |  __ `/  __/  __/_  /_  _ \\\n"
		"_  ____// /_/ /  ,<  /  __/  / / / / / /_/ /  / / /     _  /_/ // /_/ // /_ / /_ _  / /  __/\n"
		"/_/     \\____//_/|_| \\___//_/ /_/ /_/\\____//_/ /_/      /_____/ \\__,_/ \\__/ \\__/ /_/  \\___/\n"
		"\n"
		"	   ____________                  ______      _____\n"
		"	   __  ___/__(_)______ _______  ____  /_____ __  /______________\n"
		"	   _____ \\__  /__  __ `__ \\  / / /_  /_  __ `/  __/  __ \\_  ___/\n"
		"	   ____/ /_  / _  / / / / / /_/ /_  / / /_/ // /_ / /_/ /  /\n"
		"	   /____/ /_/  /_/ /_/ /_/\\__,_/ /_/  \\__,_/ \\__/ \\____//_/\n"
		"\n"
		"\n"
		"			   Welcome to the world of Pokemon!\n"
		"\n";
}


//error message if a file fails to open
void errorCannotOpen(std::string filename) {
	printLongSep();
	std::cout << "ERROR: Unable to open " << filename << ".\n";
	std::cout << "\n";
	std::cout << "TIPS:\n";
	std::cout << "Please check if " << filename << " is in the 'pkmn-data' folder.\n";
	std::cout << "Please check if " << filename << " can be opened.\n";
	std::cout << "\n";
	std::cout << "If " << filename << " is missing or damaged, please re-install from https://github.com/IrishMorales/text-based-pokemon-battle";
	printLongSep();
	exit(1);
}

//checks if data files are complete
void checkData(std::ifstream& pkmnList, std::ifstream& pkmnMoves) {
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
std::string getNext(std::ifstream& file) {
	std::string tmpLine;
	std::getline (file, tmpLine, ',');
	return tmpLine;
}

//return search pointer to beginning of ifstream file
void returnToBegin(std::ifstream& file) {
	file.clear();
	file.seekg(0, std::ios::beg);
}

//prints both pokemon stats
void printBothPkmnInfo(Pkmn pkmn1, Pkmn pkmn2) {
	std::cout << "\n";
	printShortSep();
	std::cout << pkmn1.name << " LVL " << pkmn1.lvl << " ACC: " << pkmn1.ACC << " ACCmult: " << pkmn1.ACCmult << "\n";
	std::cout << "HP: " << pkmn1.HP << "/" << pkmn1.baseHP << "\nATK: " << pkmn1.ATK << " DEF: " << pkmn1.DEF << " SATK: " << pkmn1.SATK << " SDEF: " << pkmn1.SDEF << " SPD: " << pkmn1.SPD << "\n";
	printShortSep();
	std::cout << pkmn2.name << " LVL " << pkmn2.lvl << " ACC: " << pkmn2.ACC << " ACCmult: " << pkmn2.ACCmult << " (WILD)\n";
	std::cout << "HP: " << pkmn2.HP << "/" << pkmn2.baseHP << "\nATK: " << pkmn2.ATK << " DEF: " << pkmn2.DEF << " SATK: " << pkmn2.SATK << " SDEF: " << pkmn2.SDEF << " SPD: " << pkmn2.SPD << "\n";
	printShortSep();
	std::cout << "\n";
}

//changes pkmn stats based on move
//pkmn1 - attacker, pkmn2 - receiver
void Pkmn::moveEffect(Pkmn& pkmnB, int ind, bool& inBattle) {
	std::string cat = moveCat[ind];
	/*
	Columns for pkmnMoves.txt:

	PHYSICAL, SPECIAL, SAME Moves:
	Index, Move, Type, Category, PP, Power, Accuracy
	
	Stat-changing Moves (ATKO, DEFO, ACCO, SATKO, SDEFO, SPDO, ACCO, EVAO, ATKS, DEFS, ACCS, SATKS, SDEFS, SPDS, ACCS, EVAS):
	Index, Move, Type, Category, PP, Stage Multiplier, Accuracy
	
	Repeating moves (PREPR):
	Index, Move, Type, Category, PP, Power, Accuracy, Min Repetitions, Max Repetitions
	
	For this program, EVA stages are not opposite ACC stages (e.g. EVA higher by 1 stage is stored as -1 instead of +1)
	*/
	if (cat == "PHYSICAL")		{pkmnB.HP -= dmg(lvl, movePWR[ind], ATK, pkmnB.DEF);}
	else if (cat == "SPECIAL")	{pkmnB.HP -= dmg(lvl, movePWR[ind], SATK, pkmnB.SDEF);}
	else { //Status Moves
	
		//change stat of other pokemon
		if (cat == "ATKO")		{pkmnB.ATKmult = pkmnB.modMult(pkmnB.ATKmult, movePWR[ind]);	pkmnB.ATK = pkmnB.modStat(pkmnB.baseATK, pkmnB.ATKmult);}
		else if (cat == "DEFO")	{pkmnB.DEFmult = pkmnB.modMult(pkmnB.DEFmult, movePWR[ind]);	pkmnB.DEF = pkmnB.modStat(pkmnB.baseDEF, pkmnB.DEFmult);}
		else if (cat == "SATKO"){pkmnB.SATKmult= pkmnB.modMult(pkmnB.SATKmult,movePWR[ind]);	pkmnB.SATK= pkmnB.modStat(pkmnB.baseSATK, pkmnB.SATKmult);}
		else if (cat == "SDEFO"){pkmnB.SDEFmult= pkmnB.modMult(pkmnB.SDEFmult,movePWR[ind]);	pkmnB.SDEF= pkmnB.modStat(pkmnB.baseSDEF, pkmnB.SDEFmult);}
		else if (cat == "SPDO")	{pkmnB.SPDmult = pkmnB.modMult(pkmnB.SPDmult, movePWR[ind]); 	pkmnB.SPD = pkmnB.modStat(pkmnB.baseSPD, pkmnB.SPDmult);}
		else if (cat == "ACCO")	{pkmnB.ACCmult = pkmnB.modMult(pkmnB.ACCmult, movePWR[ind]);	pkmnB.ACC = pkmnB.modStat(100, pkmnB.ACCmult);}
		else if (cat == "EVAO")	{pkmnB.EVAmult = pkmnB.modMult(pkmnB.EVAmult, movePWR[ind]);	pkmnB.EVA = pkmnB.modStat(100, pkmnB.EVAmult);}
		
		//change stat of self
		else if (cat == "ATKS")	{ATKmult = modMult(ATKmult, movePWR[ind]); 	ATK = modStat(baseATK, ATKmult);}
		else if (cat == "DEFS")	{DEFmult = modMult(DEFmult, movePWR[ind]); 	DEF = modStat(baseDEF, DEFmult);}
		else if (cat == "SATKS"){SATKmult= modMult(SATKmult,movePWR[ind]);	SATK= modStat(baseSATK, SATKmult);}
		else if (cat == "SDEFS"){SDEFmult= modMult(SDEFmult,movePWR[ind]);	SDEF= modStat(baseSDEF, SDEFmult);}
		else if (cat == "SPDS")	{SPDmult = modMult(SPDmult, movePWR[ind]);	SPD = modStat(baseSPD, SPDmult);}
		else if (cat == "ACCS")	{ACCmult = modMult(ACCmult, movePWR[ind]);	ACC = modStat(ACC, ACCmult);}
		else if (cat == "EVAS")	{EVAmult = modMult(EVAmult, movePWR[ind]);	EVA = modStat(EVA, EVAmult);}
		
		//instantly ends battle
		else if (cat == "OHKO") {pkmnB.HP = 0;}
		else if (cat == "FLEE")	{inBattle = false;} //add battle end message here
		
		//moves whose damage is always constant
		else if (cat == "SAME")	{pkmnB.HP -= movePWR[ind]; if(movePWR[ind] == 0) {std::cout << "But nothing happened!\n";}}
		
		//moves that hit multiple times
		/*
		else if (cat=="PREPR"){
			int k=(rand() % pkmn1.moveRep2[ind] + pkmn1.moveRep1[ind]);
			for(int j=0; j<k; ++j) {
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
			}
			std::cout << pkmn1.move[ind] << " hit " << k << " times!" << std::endl;
		}
		
		else if (cat == "PREP"){
			for(int j=0; j<pkmn1.moveRep1[ind]; ++j) {
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePWR[ind], pkmn1.ATK, pkmn2.DEF);
			}
			std::cout << pkmn1.move[ind] << " hit " << pkmn1.moveRep1[ind] << " times!" << std::endl;
		}
		*/
	}
}

bool inBattleCheck(Pkmn pkmn1, Pkmn pkmn2, bool inBattle) {
	if (pkmn1.HP <= 0) { 
		std::cout << pkmn1.name << " fainted!\n";
		return false;
	}
	else if (pkmn2.HP <= 0) { 
		std::cout << "The wild " << pkmn2.name << " fainted!\n";
		return false;
	}
	return inBattle;
}

bool Pkmn::struggleCheck() {
	if (std::all_of(movePP,movePP + 4, [](int currMovePP){return currMovePP == 0;})) {
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
		std::cout << name << " tries to use " << move[moveIndex] << "...\n";
		std::cout << "...But " << move[moveIndex] << " is out of PP!\n";
		return false;
	}
}

bool Pkmn::missCheck(Pkmn pkmnB, int moveIndex) {
	int threshold = static_cast<int>(moveACC[moveIndex] * ACC * pkmnB.EVA / 10000);
	int randNum = rand() % 101;
	srand(time(0));
	
	std:: cout << moveACC[moveIndex] << " " << ACC << " " << pkmnB.EVA << std::endl;
	
	if (randNum > threshold){
		std::cout << name << " missed! THRES:" << threshold << " RAND: " << randNum << std::endl;
		return true;
	}
	return false;
}

void Pkmn::struggle(Pkmn& oppPkmn) {
	std::cout << name << " has no moves left!\n";
	std::cout << name << " used Struggle!\n";
	int struggleDmg = dmg(lvl, 50, ATK, oppPkmn.DEF);
	oppPkmn.HP -= struggleDmg;
	std::cout << name << " is hit with recoil!\n";
	HP -= struggleDmg/2;
}

void Pkmn::makeMove(Pkmn& pkmnB, bool isPlayer, bool& inBattle, std::ifstream& pkmnMoves) {
	printPkmnMoves();
	
	//if any move still has PP, move
	if (!struggleCheck()) {
		int moveIndex;
		//if player is moving
		if (isPlayer) {
			std::cout << "What should " << name << " do?\n";
		
			moveIndex = distance(move, find(move, move + 4, getValidMove(pkmnMoves, true)));
			
			if (PPCheck(moveIndex)) {std::cout << name << " used " << move[moveIndex] << "!\n";}
		}
		//if opponent is moving
		else {
			//get index for random move
			srand(time(0));
			moveIndex = rand() % 4;	
			
			while (movePP[moveIndex] == 0) {
				srand(time((long int*)moveIndex));
				int moveIndex = rand() % 4;	
			}
			
			if (PPCheck(moveIndex)) {std::cout << "The wild " << name << " used " << move[moveIndex] << "!\n";}
		}
		
		if (!missCheck(pkmnB, moveIndex)) {moveEffect(pkmnB, moveIndex, inBattle);}
	}
	//if no PP, struggle
	else {
		struggle(pkmnB);
	}
}

bool replayCheck() {
	std::string s;
	std::cout << "Play again? Enter Y to replay and anything else to exit.\n";
	std::cout << "YOU: ";
	std::getline(std::cin,s);
	std::cout << "\n";
	if (s == "Y" || s == "y") {return true;}
	else {return false;}
}
