#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits> 
#include <string>
#include <algorithm> 
#include <ios> 
#include <cctype>
#include "pkmn.h"
using namespace std;

int main() {
	ifstream pkmnList;
	ifstream pkmnMoves;

	//add welcome msg function here
	cout << "Welcome to the world of Pokemon!\n";
	checkData(pkmnList, pkmnMoves);
	
	printLongSep();
	cout << "Getting your Pokemon ready...\n";
	Pkmn pkmn1(pkmnList, pkmnMoves);
    
    printLongSep();
    cout << "Getting the opponent Pokemon ready...\n";
    Pkmn pkmn2(pkmnList, pkmnMoves);
	
	printLongSep();
	cout << "A wild " << pkmn2.name << " appeared!\n";
	cout << "Go! " << pkmn1.name << "!\n";
	
	bool inBattle = true;
	
	while (inBattle) {
		
		//make function
		printCurlySep();
		pkmn1.printPkmnInfo();
		printCurlySep();
		pkmn2.printPkmnInfo();
		printCurlySep();
		/*
		//pkmn w higher speed goes first
		if (pkmn1.SPD > pkmn2.SPD) {
			//pkmn1first
			cout << "What will " << pkmn1.name << " do?" << endl;
			//getValidMoveIndex()?
			
			//check pkmn2HP
			//pkmn2
		}
		else {
			//pkmn2first
			//check pkmn2HP
			//pkmn1
		}
		*/
		//check inBattle
	}
	/*
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
