#include <fstream>

class Pkmn {
	
	public:
		Pkmn (std::ifstream& pkmnList, std::ifstream& pkmnMoves);
		
		//pkmn info
		std::string name;
		
		//pkmn functions
		int setHP (int lvl, std::ifstream& pkmnList);
		int setStat (int lvl, std::ifstream& pkmnList);
		int dmg (int lvl, int movePWR, int ATK1, int DEF2);
		int modMult (int statMult, int movePWR);
		int modStat (int bstat, int statMult);
		int setAEStat (int mult);
		void printPkmnInfo ();
		void printPkmnMoves (Pkmn pkmn);
		void debug (Pkmn pkmn1, Pkmn pkmn2);
		
	private:
		//pkmn type
		std::string type1;
		std::string type2;
		
		//pkmn moveset and info
		std::string move[4];
		std::string moveType[4];
		std::string moveCat[4]; //Physical/Special
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

std::string capitalize(std::string s);

void printLongSep();
void printShortSep();
void printCurlySep();

void errorCannotOpen(std::string filename);
void checkData(std::ifstream& pkmnList, std::ifstream& pkmnMoves);

std::string getNext(std::ifstream& file);
void returnToBegin(std::ifstream& file);

bool checkValidName(std::string tmpName, std::ifstream& pkmnList);
std::string getValidName(std::ifstream& pkmnList);

bool checkValidLvl(std::string tmpLvl);
int getValidLvl(std::string name);

bool checkValidMove(std::string tmpMove, std::ifstream& pkmnMoves, std::string move[4], bool inBattle);
std::string getValidMove(std::string name, std::ifstream& pkmnMoves, std::string move[4], bool inBattle);
