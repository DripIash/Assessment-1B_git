#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

enum Size {
	FINE = 1,
	DIMINUTIVE,
	TINY,
	SMALL,
	MEDIUM,
	LARGE,
	MASSIVE,
	GARGANTUAN,
	COLOSSAL
};

enum Weapon {
	SWORD,
	BATTLEAXE,
	CLAWS
};

vector<Weapon> weaponList = {

};

int RollDie(int sides);
int RollXTimes(int sides, int amount);

struct Mob {
	string name;
	Size size;
	int str;
	int dex;
	int armour;
	int hitDie;
	int numHD;
	Weapon weapon;

	int AC;
	int HP;
	int initiative;

	void CalcAC() {
		AC = 10 + armour + dex;
		switch (size) {
		case FINE: AC += 8; break;
		case DIMINUTIVE: AC += 4; break;
		case TINY: AC += 2; break;
		case SMALL: AC += 1; break;
		case MEDIUM: break;
		case LARGE: AC -= 1; break;
		case MASSIVE: AC -= 2; break;
		case GARGANTUAN: AC -= 4; break;
		case COLOSSAL: AC -= 8; break;
		}
	}

	void CalcHP() {
		HP = RollXTimes(hitDie, numHD);
	}

	void SetSize(string input) {
		if (input == "Fine")
			size = Size::FINE;
		else if (input == "Diminutive")
			size = Size::DIMINUTIVE;
		else if (input == "Tiny")
			size = Size::TINY;
		else if (input == "Small")
			size = Size::SMALL;
		else if (input == "Large")
			size = Size::LARGE;
		else if (input == "Massive")
			size = Size::MASSIVE;
		else if (input == "Gargantuan")
			size = Size::GARGANTUAN;
		else if (input == "Colossal")
			size = Size::COLOSSAL;
		else
			size = Size::MEDIUM;
	}
};

void SetMob(Mob& mob);
int FIGHT(Mob& mob1, Mob& mob2);

int main(){
	srand(time(NULL));
	Mob mob1, mob2;
	int result;
	
	cout << "===First mob===" << endl;
	SetMob(mob1);
	cout << "===Second mob===" << endl;
	SetMob(mob2);
	
	result = FIGHT(mob1, mob2);
	switch (result) {
	case 0: cout << "The " << mob1.name << " and " << mob2.name << " both died at the same time, resulting in a draw"; break;
	case 1: cout << "The " << mob1.name << " and " << mob2.name << " both died at the same time, resulting in a draw"; break;
	case 2: cout << "The " << mob1.name << " and " << mob2.name << " both died at the same time, resulting in a draw"; break;
	case -1: cout << "huh wat happen"; break;
	}

	cout << "\n\n======END======\n";
}


void SetMob(Mob& mob) {
	cout << "=Mob stats=\n-Name: ";
	cin >> mob.name;

	string input;
	cout << "- Size: ";
	cin >> input;
	mob.SetSize(input);

	cout << "- Strength bonus: ";
	cin >> mob.str;

	cout << "- Dexterity bonus: ";
	cin >> mob.dex;

	cout << "- Armour bonus (natural and worn armour): ";
	cin >> mob.armour;
	mob.CalcAC();

	cout << "- Hit die: D";
	cin >> mob.hitDie;
	cout << "- How many D" << mob.hitDie << ": ";
	cin >> mob.numHD;
	mob.CalcHP();

	cout << "- Weapon: ";
	cin >> input;

}

int FIGHT(Mob& mob1, Mob& mob2) {
	return -1;
}

int RollDie(int sides) {
	return (rand() % sides) + 1;
}

int RollXTimes(int sides, int amount) {
	int total = 0;
	while (amount > 0) {
		total += (rand() % sides) + 1;
	}
	return total;
}