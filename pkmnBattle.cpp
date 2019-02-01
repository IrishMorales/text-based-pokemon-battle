#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits> 
#include <string>
#include <algorithm> 
#include <ios> 
#include <cctype>
using namespace std;

/*
To be implemented: (According to importance)
-Special moves (ALL)
-Types
-Status Effects
-Damage Modifiers
-Indentation
*/

class pkmn{
	public:
		string name;
		string moveset[4];
		string moveType[4];
		string moveCat[4];
		int movePP[4];
		int movebasePP[4];
		int movePwr[4];
		int moveAcc[4];
		int moveRep1[4];
		int moveRep2[4];
		int lvl;
		string type1;
		string type2;
		int HP;
		int ATK;
		int DEF;
		int SATK;
		int SDEF;
		int SPD;
		int ACC=1; //100/100
		int EVA=1; //100/100
		int baseATK;
		int baseDEF;
		int baseSATK;
		int baseSDEF;
		int baseSPD;
		int ATKmult=0;
		int DEFmult=0;
		int SATKmult=0;
		int SDEFmult=0;
		int SPDmult=0;
		int ACCmult=0;
		int EVAmult=0;
		//baseStat=computed stat for PKMN at mult 0
		
		pkmn();
		
		int setHP(int bHP, int lvl){
			int HP=static_cast<int>(((2*bHP*lvl)/100)+10+lvl);
			return HP;
		}
		
		int setStat(int bStat, int lvl){
			int stat=static_cast<int>(((2*bStat*lvl)/100)+5);
			return stat;
		}
		
		int dmg(int lvl, int movePwr, int A, int D){
			int t=static_cast<int>(((2*lvl / 5 + 2) * movePwr * (A/D))/50 + 2);
			return t;
		}
		
		int modMult(int statMult, int movePwr){
			statMult+=movePwr;
			if (statMult<-6) statMult=-6;
			else if (statMult>6) statMult=6;
			return statMult;
		}
		
		int modStat(int bstat, int statMult){
			int t=bstat*max(2,2+statMult)/max(2,2-statMult);
			return t;
		}
		
		int setAEStat(int mult){
			int val=1;
			switch(mult){
				case -6:
					val=static_cast<int>(33/100);
					break;
				case -5:
					val=static_cast<int>(36/100);
					break;
				case -4:
					val=static_cast<int>(43/100);
					break;
				case -3:
					val=static_cast<int>(50/100);
					break;
				case -2:
					val=static_cast<int>(60/100);
					break;
				case -1:
					val=static_cast<int>(75/100);
					break;
				case 0:
					return 1;
				case 1:
					val=static_cast<int>(133/100);
					break;
				case 2:
					val=static_cast<int>(166/100);
					break;
				case 3:
					val=static_cast<int>(200/100);
					break;
				case 4:
					val=static_cast<int>(233/100);
					break;
				case 5:
					val=static_cast<int>(266/100);
					break;
				case 6:
					val=static_cast<int>(300/100);
					break;
			}
			return val;
		}
};

string capitalize(string s){
	for (int i=0; i<s.length(); ++i) s[i]=toupper(s[i]);
	return s;
}

pkmn::pkmn(){
	ifstream pkmnList;
	pkmnList.open("pkmnList.txt");
	
	if (!pkmnList) {
	    cout << "Unable to open pkmnList.txt";
	    exit(1);
	}
	
	cout << "Please input a Pokemon." << endl;
	bool foundPKMN;
	string tmpName;
	string tmpLine;
	
	input:
	foundPKMN=false;
	cin >> tmpName;
	tmpName=capitalize(tmpName);
	cout << endl << "Looking for " << tmpName << "..." << endl;
	
	pkmnList.clear();
	pkmnList.seekg(0, ios::beg);
	
	while (getline(pkmnList, tmpLine, ',')) {
        if (tmpLine == tmpName) {
        	foundPKMN=true;
        	name=tmpLine;
        	cout << "Found it! What level will " << name << " start at?" << endl;
        	cin >> lvl;
        	getline(pkmnList, tmpLine, ',');
        	type1=tmpLine;
        	getline(pkmnList, tmpLine, ',');
        	type2=tmpLine;
        	for (int i=0; i<2; ++i) getline(pkmnList, tmpLine, ',');
			HP=setHP(stoi(tmpLine),lvl);
			getline(pkmnList, tmpLine, ',');
			ATK=setStat(stoi(tmpLine), lvl);
			baseATK=ATK;
			getline(pkmnList, tmpLine, ',');
			DEF=setStat(stoi(tmpLine), lvl);
			baseDEF=DEF;
			getline(pkmnList, tmpLine, ',');
			SATK=setStat(stoi(tmpLine), lvl);
			baseSATK=ATK;
			getline(pkmnList, tmpLine, ',');
			SDEF=setStat(stoi(tmpLine), lvl);
			baseSDEF=DEF;
			getline(pkmnList, tmpLine, ',');
			SPD=setStat(stoi(tmpLine), lvl);
			baseSPD=SPD;
            break;
        }
    }
    
    if(!foundPKMN){
    	cout << "Sorry, we couldn't find that. :<" << endl;
    	cout << "Please input a different Pokemon from Generation I." << endl;
    	goto input;
	}
	
	pkmnList.close();
	
	ifstream pkmnMoves;
	pkmnMoves.open("pkmnMoves.txt");
	
	if (!pkmnMoves) {
	    cout << "Unable to open pkmnMoves.txt";
	    exit(1);
	}
	
	for(int i=0; i<4; ++i){
		cout << endl << "Please input any valid move for " << name << "." << endl;
		bool foundMove;
		string tmpName;
		
		input2:
		foundMove=false;
		if(i==0)for (int j=0; j<2; ++j) getline(cin, tmpName);
		else getline(cin, tmpName);
		tmpName=capitalize(tmpName);
		cout << endl << "Adding move: " << tmpName << "..." << endl;
		
		pkmnMoves.clear();
		pkmnMoves.seekg(0, ios::beg);
		
		while (getline(pkmnMoves, tmpLine, ',')) {
	        if (tmpLine == tmpName) {
	        	foundMove=true;
	        	moveset[i]=tmpLine;
	        	cout << "Move added. " << endl;
	        	getline(pkmnMoves, tmpLine, ',');
	        	moveType[i]=tmpLine;
	        	cout << "Type: " << tmpLine << endl;
	        	getline(pkmnMoves, tmpLine, ',');
				moveCat[i]=tmpLine;
				cout << "Cat: " << tmpLine << endl;
				getline(pkmnMoves, tmpLine, ',');
				movebasePP[i]=stoi(tmpLine);
				movePP[i]=stoi(tmpLine);
				cout << "PP: " << tmpLine << endl;
				getline(pkmnMoves, tmpLine, ',');
				movePwr[i]=stoi(tmpLine);
				cout << "Pwr: " << tmpLine << endl;
				getline(pkmnMoves, tmpLine, ',');
				moveAcc[i]=stoi(tmpLine);
				cout << "Acc: " << tmpLine << endl;
				if(moveCat[i]=="PREPR" || moveCat[i]=="PREP"){
					getline(pkmnMoves, tmpLine, ',');
					moveRep1[i]=stoi(tmpLine);
					if(moveCat[i]=="PREPR"){
						getline(pkmnMoves, tmpLine, ',');
						moveRep2[i]=stoi(tmpLine)-1;
					}
				}
	            break;
	        }
	    }
	    
	    if(!foundMove){
	    	cout << "Sorry, that move is unavailable! :<" << endl;
	    	cout << "Please input another valid move for " << name << "." << endl << endl;
	    	goto input2;
		}
	}
	
	pkmnMoves.close();
	
	cout << name << " is ready!" << endl << endl;
}

int main(){
	cout << "Setting up your Pokemon..." << endl;
	pkmn pkmn1;
    
    cout << "------------------------------------" << endl;
    cout << "Setting up the opponent Pokemon..." << endl;
    pkmn pkmn2;
	
	/*DEBUG:
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << pkmn1.name << endl;
	cout << pkmn1.lvl << endl;
	cout << pkmn1.baseHP << endl;
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
	cout << pkmn2.baseHP << endl;
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
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;*/
	
	cout << "~-~-~-~-~-~-~-~-~-~-~-~-~" << endl;
	cout << "A wild " << pkmn2.name << " appeared!" << endl;
	cout << "Go! " << pkmn1.name << "!" << endl;
	
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
			cout << pkmn1.name << "'s Moveset: " << endl;
			cout << "[Index]  Move"<< endl;
			for (int i=0; i<4; ++i) cout << " [" << i << "]  " << pkmn1.moveset[i] << "    PP: " << pkmn1.movePP[i] << "/" << pkmn1.movebasePP[i] << endl;
			cout << "~-~-~-~-~-~-~-~-~-~-~-~-~" << endl;
			
			ind=0;
			do { 
				if (ind > 3 || ind < 0) cout << "Input invalid, please enter the index of a valid move.\n";
				cin >> ind;
			} while(ind > 3 || ind < 0);
			
			if(pkmn1.movePP[ind]>0){ //checks if move still has PP
				cout << endl << pkmn1.name << " used " << pkmn1.moveset[ind] << "!" << endl;
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
				cout << pkmn1.name << " tries to use " << pkmn1.moveset[ind] << "..." << endl;
				cout << "...But " << pkmn1.moveset[ind] << "is out of PP!" << endl;
				goto PKMN2F1;
			}
			
			//accuracy check
			if (rand() % 100 + 1 > (pkmn1.moveAcc[ind]*(pkmn1.ACC/pkmn2.EVA))){
				cout << pkmn1.name << " missed!" << endl;
				goto PKMN2F1;
			}
			
			//checks moveCat
			string cat=pkmn1.moveCat[ind];
			
			if(cat=="PHYSICAL"){
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePwr[ind], pkmn1.ATK, pkmn2.DEF);
			}
			
			else if(cat=="SPECIAL"){
				pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePwr[ind], pkmn1.SATK, pkmn2.SDEF);
			}
			
			else{ //Status
				if (cat=="ATKO"){ pkmn2.ATKmult=pkmn2.modMult(pkmn2.ATKmult, pkmn1.movePwr[ind]); pkmn2.ATK=pkmn2.modStat(pkmn2.baseATK, pkmn2.ATKmult);}
				else if (cat=="DEFO"){ pkmn2.DEFmult=pkmn2.modMult(pkmn2.DEFmult, pkmn1.movePwr[ind]); pkmn2.DEF=pkmn2.modStat(pkmn2.baseDEF, pkmn2.DEFmult);}
				else if (cat=="SATKO"){ pkmn2.SATKmult=pkmn2.modMult(pkmn2.SATKmult, pkmn1.movePwr[ind]); pkmn2.SATK=pkmn2.modStat(pkmn2.baseSATK, pkmn2.SATKmult);}
				else if (cat=="SDEFO"){ pkmn2.SDEFmult=pkmn2.modMult(pkmn2.SDEFmult, pkmn1.movePwr[ind]); pkmn2.SDEF=pkmn2.modStat(pkmn2.baseSDEF, pkmn2.SDEFmult);}
				else if (cat=="SPDO"){ pkmn2.SPDmult=pkmn2.modMult(pkmn2.SPDmult, pkmn1.movePwr[ind]); pkmn2.SPD=pkmn2.modStat(pkmn2.baseSPD, pkmn2.SPDmult);}
				else if (cat=="ATKS"){ pkmn1.ATKmult=pkmn1.modMult(pkmn1.ATKmult, pkmn1.movePwr[ind]); pkmn1.ATK=pkmn1.modStat(pkmn1.baseATK, pkmn1.ATKmult);}
				else if (cat=="DEFS"){ pkmn1.DEFmult=pkmn1.modMult(pkmn1.DEFmult, pkmn1.movePwr[ind]); pkmn1.DEF=pkmn1.modStat(pkmn1.baseDEF, pkmn1.DEFmult);}
				else if (cat=="SATKS"){ pkmn1.SATKmult=pkmn1.modMult(pkmn1.SATKmult, pkmn1.movePwr[ind]); pkmn1.SATK=pkmn1.modStat(pkmn1.baseSATK, pkmn1.SATKmult);}
				else if (cat=="SDEFS"){ pkmn1.SDEFmult=pkmn1.modMult(pkmn1.SDEFmult, pkmn1.movePwr[ind]); pkmn1.SDEF=pkmn1.modStat(pkmn1.baseSDEF, pkmn1.SDEFmult);}
				else if (cat=="SPDS"){ pkmn1.SPDmult=pkmn1.modMult(pkmn1.SPDmult, pkmn1.movePwr[ind]); pkmn1.SPD=pkmn1.modStat(pkmn1.baseSPD, pkmn1.SPDmult);}
				else if (cat=="OHKO") pkmn2.HP=0;
				else if (cat=="FLEE"){ inBattle=false; goto flee; }
				else if (cat=="SAME"){ pkmn2.HP-=pkmn1.movePwr[ind]; }
				else if (cat=="ACCS"){
					pkmn1.ACCmult+=pkmn1.movePwr[ind];
					pkmn1.ACC=pkmn1.setAEStat(pkmn1.ACCmult);
				}
				else if (cat=="EVAS"){
					pkmn1.EVAmult+=pkmn1.movePwr[ind];
					pkmn1.EVA=pkmn1.setAEStat(pkmn1.EVAmult);
				}
				else if (cat=="ACCO"){
					pkmn2.ACCmult+=pkmn1.movePwr[ind];
					pkmn2.ACC=pkmn2.setAEStat(pkmn2.ACCmult);
				}
				else if (cat=="EVAO"){
					pkmn2.EVAmult+=pkmn1.movePwr[ind];
					pkmn2.EVA=pkmn2.setAEStat(pkmn2.EVAmult);
				}
				else if (cat=="PREPR"){
					int k=(rand() % pkmn1.moveRep2[ind] + pkmn1.moveRep1[ind]);
					for(int j=0; j<k; ++j) {
						pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePwr[ind], pkmn1.ATK, pkmn2.DEF);
					}
					cout << pkmn1.moveset[ind] << " hit " << k << " times!" << endl;
				}
				else if (cat=="PREP"){
					for(int j=0; j<pkmn1.moveRep1[ind]; ++j) {
						pkmn2.HP-=pkmn1.dmg(pkmn1.lvl, pkmn1.movePwr[ind], pkmn1.ATK, pkmn2.DEF);
					}
					cout << pkmn1.moveset[ind] << " hit " << pkmn1.moveRep1[ind] << " times!" << endl;
				}
			}
		}
		
		if (PKMN2FIRST) goto PKMN2F3;
		PKMN2F1:
		
		if(pkmn2.HP>0) {
			
			do {
				ind=rand() % 4;
			} while (pkmn2.moveset[ind] == "-");
			
			cout << pkmn2.name << " used " << pkmn2.moveset[ind] << "!" << endl;
			string cat=pkmn2.moveCat[ind];
			
			if(cat=="PHYSICAL"){
				pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePwr[ind], pkmn2.ATK, pkmn1.DEF);
			}
			
			else if(cat=="SPECIAL"){
				pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePwr[ind], pkmn2.SATK, pkmn1.SDEF);
			}
			
			else{ //Status
				if (cat=="ATKO"){ pkmn1.ATKmult=pkmn1.modMult(pkmn1.ATKmult, pkmn2.movePwr[ind]); pkmn1.ATK=pkmn1.modStat(pkmn1.baseATK, pkmn1.ATKmult);}
				else if (cat=="DEFO"){ pkmn1.DEFmult=pkmn1.modMult(pkmn1.DEFmult, pkmn2.movePwr[ind]); pkmn1.DEF=pkmn1.modStat(pkmn1.baseDEF, pkmn1.DEFmult);}
				else if (cat=="SATKO"){ pkmn1.SATKmult=pkmn1.modMult(pkmn1.SATKmult, pkmn2.movePwr[ind]); pkmn1.SATK=pkmn1.modStat(pkmn1.baseSATK, pkmn1.SATKmult);}
				else if (cat=="SDEFO"){ pkmn1.SDEFmult=pkmn1.modMult(pkmn1.SDEFmult, pkmn2.movePwr[ind]); pkmn1.SDEF=pkmn1.modStat(pkmn1.baseSDEF, pkmn1.SDEFmult);}
				else if (cat=="SPDO"){ pkmn1.SPDmult=pkmn1.modMult(pkmn1.SPDmult, pkmn2.movePwr[ind]); pkmn1.SPD=pkmn1.modStat(pkmn1.baseSPD, pkmn1.SPDmult);}
				else if (cat=="ATKS"){ pkmn2.ATKmult=pkmn2.modMult(pkmn2.ATKmult, pkmn2.movePwr[ind]); pkmn2.ATK=pkmn2.modStat(pkmn2.baseATK, pkmn2.ATKmult);}
				else if (cat=="DEFS"){ pkmn2.DEFmult=pkmn2.modMult(pkmn2.DEFmult, pkmn2.movePwr[ind]); pkmn2.DEF=pkmn2.modStat(pkmn2.baseDEF, pkmn2.DEFmult);}
				else if (cat=="SATKS"){ pkmn2.SATKmult=pkmn2.modMult(pkmn2.SATKmult, pkmn2.movePwr[ind]); pkmn2.SATK=pkmn2.modStat(pkmn2.baseSATK, pkmn2.SATKmult);}
				else if (cat=="SDEFS"){ pkmn2.SDEFmult=pkmn2.modMult(pkmn2.SDEFmult, pkmn2.movePwr[ind]); pkmn2.SDEF=pkmn2.modStat(pkmn2.baseSDEF, pkmn2.SDEFmult);}
				else if (cat=="SPDS"){ pkmn2.SPDmult=pkmn2.modMult(pkmn2.SPDmult, pkmn2.movePwr[ind]); pkmn2.SPD=pkmn2.modStat(pkmn2.baseSPD, pkmn2.SPDmult);}
				else if (cat=="OHKO") pkmn1.HP=0;
				else if (cat=="FLEE"){ inBattle=false; goto flee; }
				else if (cat=="SAME"){ pkmn1.HP-=pkmn2.movePwr[ind]; }
				else if (cat=="ACCS"){
					pkmn2.ACCmult+=pkmn2.movePwr[ind];
					pkmn2.ACC=pkmn2.setAEStat(pkmn2.ACCmult);
				}
				else if (cat=="EVAS"){
					pkmn2.EVAmult+=pkmn2.movePwr[ind];
					pkmn2.EVA=pkmn2.setAEStat(pkmn2.EVAmult);
				}
				else if (cat=="ACCO"){
					pkmn1.ACCmult+=pkmn2.movePwr[ind];
					pkmn1.ACC=pkmn1.setAEStat(pkmn1.ACCmult);
				}
				else if (cat=="EVAO"){
					pkmn1.EVAmult+=pkmn2.movePwr[ind];
					pkmn1.EVA=pkmn1.setAEStat(pkmn1.EVAmult);
				}
				else if (cat=="PREPR"){
					int k=(rand() % pkmn2.moveRep2[ind] + pkmn2.moveRep1[ind]);
					for(int j=0; j<k; ++j) {
						pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePwr[ind], pkmn2.ATK, pkmn1.DEF);
					}
					cout << pkmn2.moveset[ind] << " hit " << k << " times!" << endl;
				}
				else if (cat=="PREP"){
					for(int j=0; j<pkmn2.moveRep1[ind]; ++j) {
						pkmn1.HP-=pkmn2.dmg(pkmn2.lvl, pkmn2.movePwr[ind], pkmn2.ATK, pkmn1.DEF);
					}
					cout << pkmn2.moveset[ind] << " hit " << pkmn2.moveRep1[ind] << " times!" << endl;
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
}
