# Assessment 1B_git
Part 2 of assessment 1, contains 4 projects. #8 (Binary search) is combined with the #7 (Bubble sort) project.  
6, 7, 8, and 9 all have assertions to check the operations have been performed correctly.  
The array used in 6, 7, and 8 is:  
[ 67, 13, 3, 89, 43, 2, 19, 71, 5, 61, 97, 7, 37, 31, 17, 11, 83, 53, 23, 29 ]
## 6. Linear Search
The linear search searches a hard-coded array to find an input integer, returns the index that it is in the array.  
*Example input:*  
```
7
```
*Example output*  
```
Value 7 found at index 11 in the array

======END======
```
## 7. Bubble Sort
The bubble sort sorts an array from smallest to largest by iterating through it (size-1) times, swapping values if they are the wrong way around.
*Example input and ouput:*  
Refer to #8
## 8. Binary Search
The binary search finds a value in a sorted array by halving it each time and checking if the value is higher or lower.
*Example input:*  
```
7
```
*Example output*  
```
Value 7 found at index 3 in the array

======END======
```
## 9. Pointers
The pointers swaps 2 integers using pointers.
## 10. Mob Battle
### Background info
For the mob battle, I chose to do a simulation of Dungeons & Dragons combat, using version 3.5's ruleset.
I've included initiative, health, armour class (AC), single weapon attacks with additional damage (1d8 cold for example), immunities, vulnerabilities, resistances, and damage reduction.
The types and subtypes that I've included are in [types](/Types.txt) and [subtypes](/Subtypes.txt).
### How to use
To use the simulation, we have to enter all the details of each monster one by one.  
Everything is case sensitive, so for the size, type, subtype, immunities etc, enter for example 'Huge' or 'Magical Beast'.  
An example of what to enter is in [this file](/test.txt), you simply have to copy and paste it into the program, then hit enter to run the simulation. 
For most cases, we only need the table. A list of monsters can be found at [this link](https://www.d20srd.org/indexes/monsters.htm).

#### **EXAMPLE**
![Monster info page on d20srd.org](/Images/frost_worm.png)
The rows we need to pull information from are:
- Size/Type
- Hit Dice
- Armor Class
- Attack
- Special Qualities
- Abilities

![Relevant info highlighted](/Images/frost_worm_annotat.png)

We enter(inputs are in ''): 
```
-Name: 'Frost Worm'
-Type: 'Magical Beast'
-Subtype: 'Cold'
-Alignment subtypes:
:'none' //enter anything thats not a subtype to go to next section, same for all other ones like this

Excluding attributes already gained from the subtype: //refer to subtypes file to see what attributes are given automatically
-Immunities:
:'Cold' //not necessary due to subtype
:'none'
-Vulnerabilities:
:'none'
-Resistances:
:'none'
-Damage reduction (enter 0 if none): '0'

-Size: 'Huge'
-Health = How many d10: '14'
-Plus how much extra: '70'
-Dexterity bonus: '0'
-Armour bonus (natural and worn armour): '10'

-Weapon: 'Bite'
-Damage type: 'Bludgeon'   //take a guess based on the weapon name/what the monster is, if it seems like it might be multiple,
-Attack bonus: '21'        //pick the one that is more likely to bypass DR
-Damage die: d'8'
-How many d8: '1'
-Plus how much extra damage: '12'
-Minimum roll for crit: '20' //enter 20 if not in table
-Crit multiplier: '2'        //enter 2 if not in table
-Additional damage type: 'Cold'
-Additional damage die: 'd8'
-How many d8: '1'


