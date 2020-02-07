/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// Brian Knight ©
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>

#include "ILplates.h"

using namespace std;


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//

bool vanityCheck(string plate){
	//check if is 3 numbers
	bool noLetter = true;
	bool allLetter = true;
	for(char c : plate){
		if(!isdigit(c)){
			noLetter = false;
		}
		if(!isalpha(c)){
			allLetter = false;
		}
		
	}
	if(plate.size() <= 3 && noLetter){
		//it is a vanity plate // type digits
		return true;
	}
	if(plate.size() <= 7 && allLetter){
		//it is a vanity plate // type lettes
		return true;
	}
	//if we get here. then it is not a vanity plate
	return false;
	
}

bool personalizedCheck(string plate){
	int numOfLetters = 0;
	int numOfNum = 0;
	int spaceCount = 0;
	for(char c : plate){
		if(isdigit(c)){
			++numOfNum;
		}else if(isalpha(c)){
			++numOfLetters;
		}else if(c == ' '){
			++spaceCount;
		}
	}
	if(spaceCount != 1){ // only 1 space allowed
		return false;
	}
	
	if(numOfLetters > 0 && numOfLetters < 6){//it could be type 1
		if(numOfNum == 2){
			//this next check is used 3 times
			//it is making sure the last characters are indeed digits... and
			//that the character before the digits is a space
			if(plate[plate.size() - 2] != '0' && isdigit(plate[plate.size() - 2]) && isdigit(plate[plate.size() - 1]) && plate[plate.size() - 3] == ' '){
			//this number can't be 0
			return true;
		}	
		}else if(numOfNum == 1){
			if(plate[plate.size() - 1] != '0' && isdigit(plate[plate.size() - 1]) && plate[plate.size() - 2] == ' '){
				//this number can't be 0
				return true;
			}
		}
		
	}else if(numOfLetters == 6){ //it could be type 2
		if(numOfNum == 1){
			if(plate[plate.size() - 1] != '0' && isdigit(plate[plate.size() - 1]) && plate[plate.size() - 2] == ' '){
				//this number can't be 0
				return true;
			}
		}
		
	}
	
	//if we make it here there was no match
	return false;
}
bool digitsAndCaps(string plate){ 
	//This function will loop through every char and only allowed
	//capital lettes, spaces and digits.  Nothing else
	for(char c : plate){
		int val = c;
		if(!(val == 32 || (val > 47 && val < 58) || (val > 64 && val < 91))){
			return false; 
		}
	}
	
	//if we can make it here the plate has valid characters
	return true;
}

bool validPlate(string plate){
	return (digitsAndCaps(plate) && (personalizedCheck(plate) || vanityCheck(plate)));
}

int quickHash(string plate, int hashSize){// this will do the hashing without checking for a valid plate

	long long index = 0;
	long long multiplier = 1;
	for(char c : plate){
		if( c != ' '){//skip blanks
			int b = c;
			if(b > 64){
				index += (b - 54) * multiplier; //let letters be starting where numbers end
				multiplier *= 36; //multiply by the number of letters + digits... kinda like a hex system
			}else if(b > 47){
				index += (b - 47) * multiplier;
				multiplier *= 36;
			}
		}
	}
	

	//cout << "Index: " << index << endl;
	//cout << "Real Index:  " << (index % hashSize) << endl;
	return index % hashSize;
}


int ILplates::Hash(string plate)
{

	if(!validPlate(plate)){
		//The plate was not valid!!!!!!!!
		return -1;
	}
	
	return quickHash(plate, HT.Size());
}


//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
	if(!validPlate(plate)){
		//The plate was not valid!!!!!!!!
		return -1;
	}
	

	bool empty = false;
	string key;
	int fineSum;
	int index = quickHash(plate, HT.Size());
	int count = 0;
	
	while(empty == false && count < HT.Size()){
		//Get(int index, bool& empty, TKey& key, TValue& value)
		HT.Get(index, empty, key, fineSum);
		if(key == plate){
			return fineSum;
		}
		++count;//keep track of this to avoid an infinite looping of the table
		++index;
		if(index >=  HT.Size()){//head back to the start of hash table
			index = 0;
		}
	}

	//we searched until there was an empty slot or
	//untill we looped the entire hash table
	//since nothing was found return
  return -1;
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
	if(!validPlate(plate)){
		//The plate was not valid!!!!!!!!
		return ;
	}

	
	//Set(int index, TKey key, TValue value)
	int index = quickHash(plate, HT.Size());
	bool empty = false;
	string key;
	int fineSum;
	int count = 0;

	while(empty == false && count < HT.Size()){
		//call get, to check if the spot
		//is empty or not... and to see what plate is currently there
		
		HT.Get(index, empty, key, fineSum);
		
		if(key == plate || empty == true){
			HT.Set(index, plate, newValue);
			return;
		}
		
		++count;//keep track of this to avoid an infinite looping of the table
		++index;
		if(index >=  HT.Size()){//head back to the start of hash table
			index = 0;
		}
	}

}










