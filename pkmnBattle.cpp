#include <algorithm>
#include <iostream>
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
		printBothPkmnInfo(pkmn1, pkmn2);
		
		//pkmn w higher speed goes first
		if (pkmn1.SPD > pkmn2.SPD) {
			pkmn1Move(pkmn1, pkmn2, inBattle, pkmnMoves);
			
			inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
			if (!inBattle) {break;}
			
			//wait();
			
			printBothPkmnInfo(pkmn1, pkmn2);
			pkmn2Move(pkmn2, pkmn1, inBattle);
		}
		
		else {
			pkmn2Move(pkmn2, pkmn1, inBattle);
			
			inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
			if (!inBattle) {break;}
			
			//wait();
			
			printBothPkmnInfo(pkmn1, pkmn2);
			pkmn1Move(pkmn1, pkmn2, inBattle, pkmnMoves);
		}
		printLongSep();
		
		inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
		if (!inBattle) {break;}
		
		//wait();
	}
	printLongSep();
	cout << "\n";
	cout << "The battle has ended!\n";
	//try again function here
}
	/*			
			//accuracy check
			if (rand() % 100 + 1 > (pkmn1.moveACC[ind]*(pkmn1.ACC/pkmn2.EVA))){
				cout << pkmn1.name << " missed!" << endl;
				goto PKMN2F1;
			}*/
