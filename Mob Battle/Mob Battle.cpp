#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

// list of types
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

// list of subtypes
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

// list of sizes
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

// list of damage types, such as alignment (good, chaotic etc), physical type (slash, pierce, bludgeon), and energy (cold, fire, electricity etc)
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

// returns a random number between 1 and the input
int RollDie(int sides);
// returns the total of multiple dice rolls
int RollXTimes(int sides, int amount);
// converts input string to corresponding type (case sensitive like 'Animal')
Type ConvertToType(string input);
// converts input string to corresponding subtype (case sensitive)
SubType ConvertToSubType(string input);
// converts input string to corresponding size (case sensitive)
Size ConvertToSize(string input);
// converts input string to corresponding damage type (case sensitive)
DamageType ConvertToDamageType(string input);

// storage for all the weapon's values
struct Weapon {
	string name;
	DamageType wepDmgType;	// slash, pierce, or bludgeon
	int attackBonus;		// added to the attack roll and compared against the enemy's AC
	int dmgDie;				// (2)d6<--
	int numDD;				// -->2(d6)
	int dmgBonus;			// (2d6)+5<--
	int critMult;			// -->2X damage on a critical hit
	int critRange;			// -->19-20 for a crit with a longsword
	// bonus damage such as 2d6 cold
	DamageType additionalDmgType;
	int additionalDmgDie;
	int additionalNumDD;
};

// majority of the main combat logic, stores a mob's values
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
	// vectors for the various strengths/weaknesses
	vector<DamageType> alignedDmg;
	vector<DamageType> immunities;
	vector<DamageType> vulnerabilities;
	vector<DamageType> resistances;
	int damageReduction = 0;
	vector<DamageType> bypassDR;

	int AC;
	int HP;
	int initiative;

	// set the hit die (the dice that are rolled to determine health) and crit immunity based on type
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

	// set the strengths/weaknesses based on subtype
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

	// calculate health
	void CalcHP() {
		HP = RollXTimes(hitDie, numHD) + bonusHP;
	}

	// calculate the AC (armour class, compared against the enemy's attack roll to determine if they hit), larger size increases the AC
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

	// main logic, called twice in each iteration of the main loop, one for each mob
	void Attack(Mob& enemy) {
		// if its a swarm, it deals ((number of hit dice/ 5) rounded up) d6 damage each turn
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
		bool hit = true;	// default to true

		// roll a d20, and check if its a crit
		int attackRoll = RollDie(20);
		if (attackRoll >= weapon.critRange)															// if its within the crit range
			if ((RollDie(20) + weapon.attackBonus >= enemy.AC) && (enemy.critImmunity == false))	// then if another roll hits, its a crit
				crit = true;																		// if the second roll doesn't hit, its normal dmg

		// rolled nat 1
		if (attackRoll == 1) {
			cout << " The " << name << " critically missed!" << endl;
			return;
		}
		// if its not a nat 20, check if it hits
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

	// called if a mob makes a hit
	void DealDamage(Mob& enemy, bool crit) {
		int damage = 0;			// total damage
		int bonusDamage = 0;	// bonus energy damage
		bool damageModified = false;

		// if its a crit include multiplier
		damage += (crit ? 
			RollXTimes(weapon.dmgDie, weapon.numDD * weapon.critMult) + weapon.dmgBonus * weapon.critMult : 
			RollXTimes(weapon.dmgDie, weapon.numDD) + weapon.dmgBonus);
		// damage reduction, negated later if weapon type bypasses
		damage -= damageReduction;
		
		// check all damage alignemts in case the second one etc is the one that can bypass DR
		for (DamageType eachDmg : alignedDmg) {
			// break out of this outer loop
			if (damageModified == true)
				break;
			for (DamageType each : enemy.bypassDR)
				if (eachDmg == each) {
					damage += enemy.damageReduction;	// negate DR
					// break out of loops
					damageModified = true;
					break;
				}
		}

		// check for immunities
		for (DamageType each : enemy.immunities) {
			if (each == weapon.wepDmgType) {
				if (weapon.additionalDmgType == DamageType::EMPTY) {
					// if its immune leave the DealDamage() function
					cout << " The " << enemy.name << " is immune to the " << name << "'s attacks" << endl;
					return;
				}
				else
					damage = 0;
				break;
			}
		}

		// if there is bonus energy damage, check for weaknesses/resistances
		if (weapon.additionalDmgType != DamageType::EMPTY) {
			bonusDamage += RollXTimes(weapon.additionalDmgDie, weapon.additionalNumDD);

			for (DamageType each : enemy.immunities) {
				if (each == weapon.additionalDmgType) {
					bonusDamage = 0;
					break;
				}
			}
			
			// can't be immune AND vulnerable/resistant duh but if user inputs both then skips if immmunity is already triggered
			if (bonusDamage > 0) {
				for (DamageType each : enemy.resistances) {
					if (each == weapon.additionalDmgType) {
						// resist damage
						bonusDamage -= 10;
						break;
					}
				}
				for (DamageType each : enemy.vulnerabilities) {
					if (each == weapon.additionalDmgType) {
						// vulnerable makes damage 150%
						bonusDamage *= 1.5f;
						break;
					}
				}
			}

		}

		// adds bonus damage if there is any
		if (bonusDamage > 0)
			damage += bonusDamage;
		// always deals a minimum of 1 damage
		if (damage < 1)
			damage = 1;
		enemy.HP -= damage;

		// output message
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

// runs the sequence for entering all the values for the mobs and weapons
void SetMob(Mob& mob);
// main combat round/turn loop, as well as prep
int Fight(Mob& mob1, Mob& mob2);
// loop that runs until the user enters something that isn't a damage type, puts the values into the immunites etc vectors
void AddProperties(Mob mob, char property);

int main(){
	// random seed
	srand(time(NULL));
	Mob mob1, mob2;
	int result;
	
	// initialise the first mob
	cout << " =====First mob=====" << endl;
	SetMob(mob1);
	cin.ignore();	// for the next mob's name which uses getline()
	// initialise the second mob
	cout << "\n =====Second mob=====" << endl;
	SetMob(mob2);
	
	// main loop, gets an output for the result of the fight
	result = Fight(mob1, mob2);
	switch (result) {
	case 0: cout << " The " << mob1.name << " and " << mob2.name << " both died at the same time, resulting in a draw"; break;
	case 1: cout << " The " << mob1.name << " killed the " << mob2.name << ", with " << mob1.HP << " health remaining"; break;
	case 2: cout << " The " << mob2.name << " killed the " << mob1.name << ", with " << mob2.HP << " health remaining"; break;
	case -1: cout << " huh wat happen"; break;
	}

	cout << "\n\n======END======\n";
}

int RollDie(int sides) {
	return (rand() % sides) + 1;	// random between 1 and sides
}

int RollXTimes(int sides, int amount) {
	int total = 0;
	// for loop but its while instead cause yeah
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
		return Type::HUMANOID;	// default case
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
		return SubType::NONE;	// default case
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
		return Size::MEDIUM;	// default case
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
		return DamageType::EMPTY;	// default case, also works for breaking out of AddProperties() loop
}

void SetMob(Mob& mob) {
	string input;
	
	// mob initialisation
	//==========================================================================
	// get name
	cout << " -Name: ";
	getline(cin, mob.name);

	// get type
	cout << " -Type: ";
	getline(cin, input);
	mob.type = ConvertToType(input);
	mob.SetTypeAbilities();
	// get subtype
	cout << " -Subtype: ";
	cin >> input;
	mob.subType = ConvertToSubType(input);
	mob.SetSubTypeAbilities();

	// if subtype is swarm none of this is needed
	if (mob.subType != SubType::SWARM) {
		// get alignment subtypes that aren't from stuff like devil/demon(baatezu/tanar'ri)
		cout << " -Alignment subtypes:\n :";
		AddProperties(mob, 'd');
		cout << "\n Excluding attributes already gained from the subtype:" << endl;
		// get other immunites...
		cout << " -Immunities:\n :";
		AddProperties(mob, 'i');
		// ...vulneratibites...
		cout << " -Vulnerabilities:\n :";
		AddProperties(mob, 'v');
		// ...resistances
		cout << " -Resistances:\n :";
		AddProperties(mob, 'r');

		// get damage reduction
		cout << " -Damage reduction (enter 0 if none): ";
		cin >> mob.damageReduction;
		cout << " -What damage types bypass DR?:\n :";
		AddProperties(mob, 'b');
	}

	// get size
	cout << "\n -Size: ";
	cin >> input;
	mob.size = (ConvertToSize(input));
	// get health
	cout << " -Health = How many d" << mob.hitDie << ": ";
	cin >> mob.numHD;
	cout << " -Plus how much extra: ";
	cin >> mob.bonusHP;
	mob.CalcHP();

	// get dex bonus (used for initiative and AC)
	cout << " -Dexterity bonus: ";
	cin >> mob.dex;
	// get armour
	cout << " -Armour bonus (natural and worn armour): ";
	cin >> mob.armour;
	mob.CalcAC();
	//==========================================================================

	// weapon initialisation
	//==========================================================================
	// get name
	cout << "\n -Weapon: ";
	cin.ignore();
	getline(cin, mob.weapon.name);

	// if its swarm the weapon is always the same
	if (mob.subType != SubType::SWARM) {
		// get type (slash etc)
		cout << " -Damage type: ";
		cin >> input;
		mob.weapon.wepDmgType = ConvertToDamageType(input);

		// get attack bonus (added to attack roll)
		cout << " -Attack bonus: ";
		cin >> mob.weapon.attackBonus;

		// get damage
		cout << " -Damage die: d";
		cin >> mob.weapon.dmgDie;
		cout << " -How many d" << mob.weapon.dmgDie << ": ";
		cin >> mob.weapon.numDD;
		cout << " -Plus how much extra damage: ";
		cin >> mob.weapon.dmgBonus;

		// get crit variables
		cout << " -Minimum roll for crit: ";
		cin >> mob.weapon.critRange;
		cout << " -Crit multiplier: ";
		cin >> mob.weapon.critMult;

		// get bonus energy type damage
		cout << " -Additional damage type: ";
		cin >> input;
		mob.weapon.additionalDmgType = ConvertToDamageType(input);

		if (mob.weapon.additionalDmgType != DamageType::EMPTY) {
			cout << " -Additional damage die: d";
			cin >> mob.weapon.additionalDmgDie;
			cout << " -How many d" << mob.weapon.additionalDmgDie << ": ";
			cin >> mob.weapon.additionalNumDD;
		}
	}
	//==========================================================================
}

int Fight(Mob& mob1, Mob& mob2) {
	// set the initiative of each mob (higher initiative attacks first)
	mob1.initiative = RollDie(20);
	mob2.initiative = RollDie(20);
	int order;	// used in a switch
	if (mob1.initiative > mob2.initiative)
		order = 1;
	else if (mob1.initiative < mob2.initiative)
		order = 2;
	else if (mob1.initiative == mob2.initiative)
		order = 0;

	// angel subtype has +4 AC against evil enemies, archon subtype has +2 AC
	for (DamageType each : mob1.alignedDmg)
		if (each == DamageType::EVIL) {
			if (mob2.subType == SubType::ANGEL)
				mob2.AC += 4;
			else if (mob2.subType == SubType::ARCHON)
				mob2.AC += 2;
			break;
		}
	for (DamageType each : mob2.alignedDmg)				// checks through the mob's innate damage types
		if (each == DamageType::EVIL) {					// if evil is one of them
			if (mob1.subType == SubType::ANGEL)			// and the other mob is an angel
				mob1.AC += 4;
			else if (mob1.subType == SubType::ARCHON)	// or an archon
				mob1.AC += 2;							// increase the AC
			break;
		}

	cout << "\n +++++++++ FIGHT +++++++++";
	cout << "\n ||||| " << mob1.name << " (" << mob1.HP << "HP) VS " << mob2.name << " (" << mob2.HP << "HP) |||||";
	// round counter
	int counter = 0;
	// main loop, breaks when one or both mobs die
	while ((mob1.HP > 0) && (mob2.HP > 0)) {
		counter++;
		cout << "\n\n ROUND " << counter << endl;
		switch (order) {
		case 1:
			mob1.Attack(mob2);
			if (mob2.HP <= 0)
				break;	// break loop
			mob2.Attack(mob1);
			break;
		case 2:
			mob2.Attack(mob1);
			if (mob1.HP <= 0)
				break;	// break loop
			mob1.Attack(mob2);
			break;
		case 3:
			mob1.Attack(mob2);
			mob2.Attack(mob1);
			break;
		}
	}

	cout << "\n ==== WINNER ====" << endl;
	// return the result, 0 is a draw, -1 is an error
	if ((mob1.HP) <= 0 && (mob2.HP <= 0))
		return 0;
	else if (mob1.HP <= 0)
		return 2;
	else if (mob2.HP <= 0)
		return 1;
	else
		return -1;
}

void AddProperties(Mob mob, char property) {
	DamageType dmgTypeIn;	// stores input
	string input;			// stores input
	// loop until the break
	while (true) {
		cin >> input;							// get input and
		dmgTypeIn = ConvertToDamageType(input);	// turn it into something usable
		// run if something legible is entered
		if (dmgTypeIn != DamageType::EMPTY) {
			switch (property) {
			case 'd': mob.alignedDmg.push_back(dmgTypeIn); break;
			case 'i': mob.immunities.push_back(dmgTypeIn); break;
			case 'v': mob.vulnerabilities.push_back(dmgTypeIn); break;
			case 'r': mob.resistances.push_back(dmgTypeIn); break;
			case 'b': mob.bypassDR.push_back(dmgTypeIn); break;
			}
			cout << " :";
		}
		// otherwise exit
		else
			break;
	}
}