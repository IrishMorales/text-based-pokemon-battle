#include <algorithm>
#include <iostream>
#include "pkmn.h"

int main() {
	std::ifstream pkmnList;
	std::ifstream pkmnMoves;

	printWelcome();
	checkData(pkmnList, pkmnMoves);
	
	bool replay = true;
	
	while (replay) {
		printLongSep();
		std::cout << "Getting your Pokemon ready...\n";
		Pkmn pkmn1(pkmnList, pkmnMoves);
	    
	    printLongSep();
	    std::cout << "Getting the opponent Pokemon ready...\n";
	    Pkmn pkmn2(pkmnList, pkmnMoves);
		
		printLongSep();
		std::cout << "A wild " << pkmn2.name << " appeared!\n";
		std::cout << "Go! " << pkmn1.name << "!\n";
		
		bool inBattle = true;
		
		while (inBattle) {
			printBothPkmnInfo(pkmn1, pkmn2);
			
			//pkmn w higher speed goes first
			if (pkmn1.compareSPD(pkmn2)) {
				pkmn1.makeMove(pkmn2, true, inBattle, pkmnMoves);
				
				inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
				if (!inBattle) {break;}
				
				printBothPkmnInfo(pkmn1, pkmn2);
				pkmn2.makeMove(pkmn1, false, inBattle, pkmnMoves);
			}
			
			else {
				pkmn2.makeMove(pkmn1, false, inBattle, pkmnMoves);
				
				inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
				if (!inBattle) {break;}
				
				printBothPkmnInfo(pkmn1, pkmn2);
				pkmn1.makeMove(pkmn2, true, inBattle, pkmnMoves);
			}
			printLongSep();
			
			inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
			if (!inBattle) {break;}
		}
		printLongSep();
		std::cout << "\n";
		std::cout << "The battle has ended!\n";
		
		replay = replayCheck();
	}
	
	std::cout << "Thank you for playing! :)\n";
}
