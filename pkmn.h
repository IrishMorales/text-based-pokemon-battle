#include <fstream>

class Pkmn {
	public:
		Pkmn (std::ifstream& pkmnList, std::ifstream& pkmnMoves);
		
		//pkmn info
		std::string name;
		
		//get pkmn names
		bool checkValidName(std::string tmpName, std::ifstream& pkmnList);
		std::string getValidName(std::ifstream& pkmnList);
		
		//get pkmn lvls
		bool checkValidLvl(std::string tmpLvl);
		int getValidLvl();
		
		//get pkmn moves
		bool checkValidMove(std::string tmpMove, std::ifstream& pkmnMoves, bool inBattle);
		std::string getValidMove(std::ifstream& pkmnMoves, bool inBattle);
		
		//pkmn functions
		int setHP(std::ifstream& pkmnList);
		int setStat(std::ifstream& pkmnList);
		
		bool compareSPD(Pkmn& pkmnB);
		
		//in-battle functions
		int dmg(int lvl, int movePWR, int ATK1, int DEF2);
		int modMult(int statMult, int moveMult);
		int modStat(int bstat, int statMult);
		
		//print functions
		friend void printBothPkmnInfo(Pkmn pkmn1, Pkmn pkmn2);
		void printPkmnMoves();
		void debug();
		
		bool missCheck(Pkmn pkmnB, int moveIndex);

		//PP-related functions
		bool PPCheck(int moveIndex);
		bool struggleCheck();
		void struggle(Pkmn& oppPkmn);
		
		//move functions
		void moveEffect(Pkmn& pkmnB, int ind, bool& inBattle);
		void makeMove(Pkmn& pkmnB, bool isPlayer, bool& inBattle, std::ifstream& pkmnMoves);
		
		//check functions
		friend bool inBattleCheck(Pkmn pkmn1, Pkmn pkmn2, bool inBattle);
		
	private:
		
		//pkmn stats - baseStat affected by multiplier
		int lvl;
		int HP;
		int ATK;
		int DEF;
		int SATK;
		int SDEF;
		int SPD;
		
		//pkmn moveset and info
		std::string move[4];
		std::string moveCat[4];
		int movePP[4];
		
		//pkmn type
		std::string type1;
		std::string type2;
		
		//pkmn moveset and info
		std::string moveType[4];
		int movebasePP[4];
		int movePWR[4];
		int moveACC[4];
		int moveMinRep[4];
		int moveMaxRep[4];
		
		//pkmn hidden stats
		int ACC = 100;
		int EVA = 100;
		
		//pkmn base stats - stat when multiplier is 0
		int baseHP;
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

std::string format(std::string s);

//print separators
void printLongSep();
void printShortSep();
void printCurlySep();

void printWelcome();

//read txt files
void errorCannotOpen(std::string filename);
void checkData(std::ifstream& pkmnList, std::ifstream& pkmnMoves);

//change txt file pointera
std::string getNext(std::ifstream& file);
void returnToBegin(std::ifstream& file);

bool replayCheck();
