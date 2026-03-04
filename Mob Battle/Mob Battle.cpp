#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

enum Type {
	ABERRATION,
	ANIMAL,
	CONSTRUCT,
	DRAGON,
	ELEMENTAL, 
	FEY,
	GIANT,
	HUMANOID, 
	MAGICAL_BEAST,
	MONSTROUS_HUMANOID,
	OOZE,
	OUTSIDER,
	PLANT,
	UNDEAD,
	VERMIN
};

enum SubType {
	NONE,
	ANGEL,
	ARCHON,
	BAATEZU,
	COLD,
	ELADRIN,
	FIRE,
	GUARDINAL,
	SWARM,
	TANARRI
};

enum Size {
	FINE,
	DIMINUTIVE,
	TINY,
	SMALL,
	MEDIUM,
	LARGE,
	MASSIVE,
	GARGANTUAN,
	COLOSSAL
};

enum DamageType {
	GOOD,
	EVIL,
	CHAOTIC,
	LAWFUL,
	CHILL,
	BURN,
	ACID,
	ELECTRICITY,
	SONIC,
	SLASH,
	PIERCE,
	BLUDGEON,
	EMPTY
};

int RollDie(int sides);
int RollXTimes(int sides, int amount);
Type ConvertToType(string input);
SubType ConvertToSubType(string input);
Size ConvertToSize(string input);
DamageType ConvertToDamageType(string input);

struct Weapon {
	string name;
	DamageType wepDmgType;
	int attackBonus;
	int dmgDie;
	int numDD;
	int dmgBonus;
	int critMult;
	int critRange;
	DamageType additionalDmgType;
	int additionalDmgDie;
	int additionalNumDD;
};

struct Mob {
	string name;
	Type type;
	SubType subType;
	Size size;
	int dex;
	int armour;
	int hitDie = 8;
	int numHD;
	int bonusHP;
	Weapon weapon;

	bool critImmunity = false;
	vector<DamageType> alignedDmg;
	vector<DamageType> immunities;
	vector<DamageType> vulnerabilities;
	vector<DamageType> resistances;
	int damageReduction = 0;
	vector<DamageType> bypassDR;

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
		HP = RollXTimes(hitDie, numHD) + bonusHP;
	}

	void SetTypeAbilities() {
		switch (type) {
		case CONSTRUCT:
			hitDie = 10;
			critImmunity = true;
		case DRAGON:
			hitDie = 12; break;
		case FEY:
			hitDie = 6; break;
		case MAGICAL_BEAST:
			hitDie = 10; break;
		case OOZE:
			hitDie = 10;
			critImmunity = true;
		case PLANT:
			critImmunity = true;
		case UNDEAD:
			hitDie = 12;
			critImmunity = true;
		default: break;
		}
	}

	void SetSubTypeAbilities() {
		switch (subType) {
		case ANGEL:
			immunities.push_back(DamageType::ACID);
			immunities.push_back(DamageType::CHILL);
			resistances.push_back(DamageType::BURN);
			resistances.push_back(DamageType::ELECTRICITY);
			alignedDmg.push_back(DamageType::GOOD); break;
		case ARCHON:
			immunities.push_back(DamageType::ELECTRICITY);
			alignedDmg.push_back(DamageType::GOOD); break;
		case BAATEZU:
			immunities.push_back(DamageType::BURN);
			resistances.push_back(DamageType::ACID);
			resistances.push_back(DamageType::CHILL);
			alignedDmg.push_back(DamageType::EVIL);
			alignedDmg.push_back(DamageType::LAWFUL); break;
		case COLD:
			immunities.push_back(DamageType::CHILL);
			vulnerabilities.push_back(DamageType::BURN); break;
		case ELADRIN:
			immunities.push_back(DamageType::ELECTRICITY);
			resistances.push_back(DamageType::CHILL);
			resistances.push_back(DamageType::BURN); break;
		case FIRE:
			immunities.push_back(DamageType::BURN);
			vulnerabilities.push_back(DamageType::CHILL); break;
		case GUARDINAL:
			immunities.push_back(DamageType::ELECTRICITY);
			resistances.push_back(DamageType::CHILL);
			resistances.push_back(DamageType::SONIC);
			alignedDmg.push_back(DamageType::GOOD); break;
		case SWARM:
			critImmunity = true;
			immunities.push_back(DamageType::SLASH);
			immunities.push_back(DamageType::PIERCE);
		case TANARRI:
			immunities.push_back(DamageType::ELECTRICITY);
			resistances.push_back(DamageType::ACID);
			resistances.push_back(DamageType::CHILL);
			resistances.push_back(DamageType::BURN);
			alignedDmg.push_back(DamageType::EVIL);
			alignedDmg.push_back(DamageType::CHAOTIC); break;
		default: break;
		}
	}

	void Attack(Mob& enemy) {
		if (subType == SubType::SWARM) {
			int x = numHD + 4;
			x /= 5;
			int damage = RollXTimes(6, x);
			enemy.HP -= damage;
			cout << " The " << name << " dealt " << damage << " damage" << endl;
			cout << " " << enemy.name << " health remaining: " << enemy.HP << endl;
			return;
		}

		bool crit = false;
		bool hit = true;

		int attackRoll = RollDie(20);
		if (attackRoll >= weapon.critRange)
			if ((RollDie(20) + weapon.attackBonus >= enemy.AC) && (enemy.critImmunity == false))
				crit = true;

		if (attackRoll == 1) {
			cout << " The " << name << " critically missed lol" << endl;
			return;
		}
		else if (attackRoll != 20) {
			attackRoll += weapon.attackBonus;
			if (attackRoll < enemy.AC)
				hit = false;
		}

		if (hit)
			DealDamage(enemy, crit);
		else
			cout << " The " << name << " missed" << endl;
	}

	void DealDamage(Mob& enemy, bool crit) {
		int damage = 0;
		int bonusDamage = 0;
		bool damageModified = false;
		bool damageResisted = false;

		damage += (crit ? 
			RollXTimes(weapon.dmgDie, weapon.numDD * weapon.critMult) + weapon.dmgBonus * weapon.critMult : 
			RollXTimes(weapon.dmgDie, weapon.numDD) + weapon.dmgBonus);
		damage -= damageReduction;
		
		for (DamageType eachDmg : alignedDmg) {
			if (damageModified == true)
				break;
			for (DamageType each : enemy.bypassDR)
				if (eachDmg == each && !damageModified) {
					damage += enemy.damageReduction;
					damageModified = true;
				}
		}

		for (DamageType each : enemy.immunities) {
			if (each == weapon.wepDmgType) {
				if (weapon.additionalDmgType == DamageType::EMPTY) {
					cout << " The " << enemy.name << " is immune to the " << name << "'s attacks" << endl;
					return;
				}
				else
					damage = 0;
				break;
			}
		}

		if (weapon.additionalDmgType != DamageType::EMPTY) {
			bonusDamage += RollXTimes(weapon.additionalDmgDie, weapon.additionalNumDD);

			for (DamageType each : enemy.immunities) {
				if (each == weapon.additionalDmgType) {
					bonusDamage = 0;
					break;
				}
			}
			
			if (bonusDamage > 0) {
				for (DamageType each : enemy.resistances) {
					if (each == weapon.additionalDmgType) {
						bonusDamage -= 10;
						break;
					}
				}
				for (DamageType each : enemy.vulnerabilities) {
					if (each == weapon.additionalDmgType) {
						bonusDamage *= 1.5f;
						break;
					}
				}
			}

		}

		if (bonusDamage > 0)
			damage += bonusDamage;
		if (damage < 1)
			damage = 1;
		enemy.HP -= damage;

		if (crit) {
			cout << " The " << name << " landed a critical hit, dealing " << damage << " damage to the " << enemy.name << endl;
			cout << " " << enemy.name << " health remaining: " << enemy.HP << endl;
		}
		else {
			cout << " The " << name << " hit the " << enemy.name << " for " << damage << " damage" << endl;
			cout << " " << enemy.name << " health remaining: " << enemy.HP << endl;
		}
	}
};

void SetMob(Mob& mob);
int Fight(Mob& mob1, Mob& mob2);
void AddProperties(Mob mob, char property);

int main(){
	srand(time(NULL));
	Mob mob1, mob2;
	int result;
	
	cout << " =====First mob=====" << endl;
	SetMob(mob1);
	cin.ignore();
	cout << "\n =====Second mob=====" << endl;
	SetMob(mob2);
	
	result = Fight(mob1, mob2);
	switch (result) {
	case 0: cout << " The " << mob1.name << " and " << mob2.name << " both died at the same time, resulting in a draw"; break;
	case 1: cout << " The " << mob1.name << " killed the " << mob2.name << ", with " << mob1.HP << " health remaining"; break;
	case 2: cout << " The " << mob2.name << " killed the " << mob1.name << ", with " << mob2.HP << " health remaining"; break;
	case -1: cout << " huh wat happen"; break;
	}

	cout << "\n\n======END======\n";
}

void SetMob(Mob& mob) {
	string input;
	
	// mob properties
	{
		cout << " -Name: ";
		getline(cin, mob.name);

		cout << " -Type: ";
		getline(cin, input);
		mob.type = ConvertToType(input);
		mob.SetTypeAbilities();
		cout << " -Subtype: ";
		cin >> input;
		mob.subType = ConvertToSubType(input);
		mob.SetSubTypeAbilities();


		if (mob.subType != SubType::SWARM) {
			cout << " -Alignment subtypes:\n :";
			AddProperties(mob, 'd');
			cout << "\n Excluding attributes already gained from the subtype:" << endl;
			cout << " -Immunities:\n :";
			AddProperties(mob, 'i');
			cout << " -Vulnerabilities:\n :";
			AddProperties(mob, 'v');
			cout << " -Resistances:\n :";
			AddProperties(mob, 'r');

			cout << " -Damage reduction (enter 0 if none): ";
			cin >> mob.damageReduction;
			cout << " -What damage types bypass DR?:\n :";
			AddProperties(mob, 'b');
		}

		cout << "\n -Size: ";
		cin >> input;
		mob.size = (ConvertToSize(input));
		cout << " -Health = How many D" << mob.hitDie << ": ";
		cin >> mob.numHD;
		cout << " -Plus how much extra: ";
		cin >> mob.bonusHP;
		mob.CalcHP();

		cout << " -Dexterity bonus: ";
		cin >> mob.dex;
		cout << " -Armour bonus (natural and worn armour): ";
		cin >> mob.armour;
		mob.CalcAC();
		
	}

	// weapon properties
	{
		cout << "\n -Weapon: ";
		cin.ignore();
		getline(cin, mob.weapon.name);

		if (mob.subType != SubType::SWARM) {
			cout << " -Damage type: ";
			cin >> input;
			mob.weapon.wepDmgType = ConvertToDamageType(input);

			cout << " -Attack bonus: ";
			cin >> mob.weapon.attackBonus;

			cout << " -Damage die: D";
			cin >> mob.weapon.dmgDie;
			cout << " -How many D" << mob.weapon.dmgDie << ": ";
			cin >> mob.weapon.numDD;
			cout << " -Plus how much extra damage: ";
			cin >> mob.weapon.dmgBonus;

			cout << " -Minimum roll for crit: ";
			cin >> mob.weapon.critRange;
			cout << " -Crit multiplier: ";
			cin >> mob.weapon.critMult;

			cout << " -Additional damage type: ";
			cin >> input;
			mob.weapon.additionalDmgType = ConvertToDamageType(input);

			if (mob.weapon.additionalDmgType != DamageType::EMPTY) {
				cout << " -Additional damage die: D";
				cin >> mob.weapon.additionalDmgDie;
				cout << " -How many D" << mob.weapon.additionalDmgDie << ": ";
				cin >> mob.weapon.additionalNumDD;
			}

		}
	}

}

int Fight(Mob& mob1, Mob& mob2) {
	mob1.initiative = RollDie(20);
	mob2.initiative = RollDie(20);
	int order;
	if (mob1.initiative > mob2.initiative)
		order = 1;
	else if (mob1.initiative < mob2.initiative)
		order = 2;
	else if (mob1.initiative == mob2.initiative)
		order = 0;

	for (DamageType each : mob1.alignedDmg)
		if (each == DamageType::EVIL) {
			if (mob2.subType == SubType::ANGEL)
				mob2.AC += 4;
			else if (mob2.subType == SubType::ARCHON)
				mob2.AC += 2;
			break;
		}
	for (DamageType each : mob2.alignedDmg)
		if (each == DamageType::EVIL) {
			if (mob1.subType == SubType::ANGEL)
				mob1.AC += 4;
			else if (mob1.subType == SubType::ARCHON)
				mob1.AC += 2;
			break;
		}

	cout << "\n +++++++++ FIGHT +++++++++";
	cout << "\n ||||| " << mob1.name << " (" << mob1.HP << "HP) VS " << mob2.name << " (" << mob2.HP << "HP) |||||";
	int counter = 0;
	while ((mob1.HP > 0) && (mob2.HP > 0)) {
		counter++;
		cout << "\n\n ROUND " << counter << endl;
		if (order == 1) {
			mob1.Attack(mob2);
			if (mob2.HP <= 0)
				break;
			mob2.Attack(mob1);
		}
		else if (order == 2) {
			mob2.Attack(mob1);
			if (mob1.HP <= 0)
				break;
			mob1.Attack(mob2);
		}
		else if (order == 3) {
			mob1.Attack(mob2);
			mob2.Attack(mob1);
		}
	}
	
	cout << "\n ==== WINNER ====" << endl;
	if ((order == 3) && ((mob1.HP) <= 0 && (mob2.HP <= 0)))
			return 0;
	else if (mob1.HP <= 0)
		return 2;
	else if (mob2.HP <= 0)
		return 1;
	else
		return -1;
}

int RollDie(int sides) {
	return (rand() % sides) + 1;
}

int RollXTimes(int sides, int amount) {
	int total = 0;
	while (amount > 0) {
		total += (rand() % sides) + 1;
		amount--;
	}
	return total;
}

Type ConvertToType(string input) {
	if (input == "Abberation")
		return Type::ABERRATION;
	else if (input == "Animal")
		return Type::ANIMAL;
	else if (input == "Construct")
		return Type::CONSTRUCT;
	else if (input == "Dragon") 
		return Type::DRAGON;
	else if (input == "Elemental")
		return Type::ELEMENTAL;
	else if (input == "Fey")
		return Type::FEY;
	else if (input == "Giant")
		return Type::GIANT;
	else if (input == "Magical Beast")
		return Type::MAGICAL_BEAST;
	else if (input == "Monstrous Humanoid")
		return Type::MONSTROUS_HUMANOID;
	else if (input == "Ooze")
		return Type::OOZE;
	else if (input == "Outsider")
		return Type::OUTSIDER;
	else if (input == "Plant")
		return Type::PLANT;
	else if (input == "Undead")
		return Type::UNDEAD;
	else if (input == "Vermin")
		return Type::VERMIN;
	else
		return Type::HUMANOID;
}

SubType ConvertToSubType(string input) {
	if (input == "Angel")
		return SubType::ANGEL;
	else if (input == "Archon")
		return SubType::ARCHON;
	else if (input == "Baatezu")
		return SubType::BAATEZU;
	else if (input == "Cold")
		return SubType::COLD;
	else if (input == "Eladrin")
		return SubType::ELADRIN;
	else if (input == "Fire")
		return SubType::FIRE;
	else if (input == "Guardinal")
		return SubType::GUARDINAL;
	else if (input == "Swarm")
		return SubType::SWARM;
	else if (input == "Tanar'ri")
		return SubType::TANARRI;
	else
		return SubType::NONE;
}

Size ConvertToSize(string input) {
	if (input == "Fine")
		return Size::FINE;
	else if (input == "Diminutive")
		return Size::DIMINUTIVE;
	else if (input == "Tiny")
		return Size::TINY;
	else if (input == "Small")
		return Size::SMALL;
	else if (input == "Large")
		return Size::LARGE;
	else if (input == "Massive")
		return Size::MASSIVE;
	else if (input == "Gargantuan")
		return Size::GARGANTUAN;
	else if (input == "Colossal")
		return Size::COLOSSAL;
	else
		return Size::MEDIUM;
}

DamageType ConvertToDamageType(string input) {
	if (input == "Good")
		return DamageType::GOOD;
	else if (input == "Evil")
		return DamageType::EVIL;
	else if (input == "Chaotic")
		return DamageType::CHAOTIC;
	else if (input == "Lawful")
		return DamageType::LAWFUL;
	else if (input == "Cold")
		return DamageType::CHILL;
	else if (input == "Fire")
		return DamageType::BURN;
	else if (input == "Acid")
		return DamageType::ACID;
	else if (input == "Electricity")
		return DamageType::ELECTRICITY;
	else if (input == "Sonic")
		return DamageType::SONIC;
	else if (input == "Slash")
		return DamageType::SLASH;
	else if (input == "Pierce")
		return DamageType::PIERCE;
	else if (input == "Bludgeon")
		return DamageType::BLUDGEON;
	else
		return DamageType::EMPTY;
}

void AddProperties(Mob mob, char property) {
	DamageType dmgTypeIn = DamageType::EMPTY;
	string input;
	do {
		cin >> input;
		dmgTypeIn = ConvertToDamageType(input);
		if (dmgTypeIn != DamageType::EMPTY) {
			if (property == 'd')
				mob.alignedDmg.push_back(dmgTypeIn);
			else if (property == 'i')
				mob.immunities.push_back(dmgTypeIn);
			else if (property == 'v')
				mob.vulnerabilities.push_back(dmgTypeIn);
			else if (property == 'r')
				mob.resistances.push_back(dmgTypeIn);
			else if (property == 'b')
				mob.bypassDR.push_back(dmgTypeIn);
			cout << " :";
		}
		else
			break;
	} while (true);
}