#include <algorithm>
#include <iostream>
#include <ctime>
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
	int randMoveIndex;
	
	while (inBattle) {
		printBothPkmnInfo(pkmn1, pkmn2);
		
		//set randMoveIndex
		srand(time(0));
		randMoveIndex = rand() % 4;
		
		//pkmn w higher speed goes first
		if (pkmn1.SPD > pkmn2.SPD) {
			
			pkmn1.printPkmnMoves();
			cout << "What will " << pkmn1.name << " do?\n";
			int moveIndex = distance(pkmn1.move, find(pkmn1.move, pkmn1.move + 4, getValidMove(pkmn1.name, pkmnMoves, pkmn1.move, true)));
			cout << pkmn1.name << " used " << pkmn1.move[moveIndex] << "!\n";
			moveEffect(pkmn1, pkmn2, moveIndex, inBattle);
			
			inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
			if (!inBattle) {break;}
			//wait();
			
			printBothPkmnInfo(pkmn1, pkmn2);
			
			cout << "The wild " << pkmn2.name << " used " << pkmn2.move[randMoveIndex] << "!\n";
			moveEffect(pkmn2, pkmn1, randMoveIndex, inBattle);
		}
		
		else {
			cout << "The wild " << pkmn2.name << " used " << pkmn2.move[randMoveIndex] << "!\n";
			moveEffect(pkmn2, pkmn1, randMoveIndex, inBattle);
			
			inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
			if (!inBattle) {break;}
			//wait();
			
			printBothPkmnInfo(pkmn1, pkmn2);
			
			pkmn1.printPkmnMoves();
			cout << "What will " << pkmn1.name << " do?\n";
			int moveIndex = distance(pkmn1.move, find(pkmn1.move, pkmn1.move + 4, getValidMove(pkmn1.name, pkmnMoves, pkmn1.move, true)));
			cout << pkmn1.name << " used " << pkmn1.move[moveIndex] << "!\n";
			moveEffect(pkmn1, pkmn2, moveIndex, inBattle);
		}
		printLongSep();
		
		inBattle = inBattleCheck(pkmn1, pkmn2, inBattle);
		if (!inBattle) {break;}
		//wait();
	}
	cout << "The battle has ended!\n";
}
	/*
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
			}*/
