/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel ©
// Modified by:     Brian Knight ©
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <queue>

#include "ILplates.h"

using namespace std;

struct License{
	string plate;
	int fine;
	
};

//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

  //
  // input the plates and fines:
  //
  cout << "Reading '" << infilename << "'..." << endl;

  string fine;
  string plate;

  getline(infile, fine);

  //
  // for each pair (fine, license plate), hash and store/update fine:
  //
  
  while (!infile.eof())
  {
    getline(infile, plate);

    //cout << fine << endl;
    //cout << plate << endl;

    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);

      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }

    }//valid

    getline(infile, fine);
  }
}

void ouputToFile(string filename, vector<string>& plates, vector<int>& fines){
	filename = filename + "-output.txt";
	ofstream outputFile;
	outputFile.open(filename, ios::out | ios::trunc);
	
	for(unsigned int i = 0; i < plates.size(); ++i){
		outputFile << '"' << plates[i] << '"' << " $" << fines[i] << endl;
	}
	
	
	void close();
}

void _mergeSort(vector<License>& licenses, int bottom, int top){
	int diff = (top - bottom);
	int splitPoint = (diff / 2) + bottom; //this is the middle of our mini vector
	if(diff > 1){ //recursivelly split up our vector until we our justin groups of two
		_mergeSort(licenses, bottom, splitPoint);
		_mergeSort(licenses, (splitPoint + 1), top);
	}if(diff == 0){
		return;
	}

	vector<License> temp(diff + 1);
	//fill this temp vector up in order
	queue<License> topQ;
	queue<License> bottomQ;
	for(int i = bottom; i <= splitPoint; ++i){
		bottomQ.push(licenses[i]);
	}
	for(int i = (splitPoint + 1); i <= top; ++i){
		topQ.push(licenses[i]);
	}

	//now the queues are filled up

	int tempIndex = 0;
	while(!bottomQ.empty()){
		if(bottomQ.front().plate < topQ.front().plate){
			//bottom is smaller
			temp[tempIndex++] = bottomQ.front();
			bottomQ.pop();
		}else{
			//top is smaller
			temp[tempIndex++] = topQ.front();
			topQ.pop();
		}
		if(topQ.empty()){
			//dump all the rest of bottom into the vector
			while(!bottomQ.empty()){
				temp[tempIndex++] = bottomQ.front();
				bottomQ.pop();
			}
		}
		
	}

	while(!topQ.empty()){//dump any potential left overs into the vector

		temp[tempIndex++] = topQ.front();
		topQ.pop();
	}

	for(int i = 0; (i + bottom) <= top; ++i){
		licenses[i+bottom] = temp[i];
	}

}

void mergeSort(vector<string>& plates, vector<int>& fines){
	//create a vector of the License Structs
	vector<License> licenses(plates.size());
	unsigned int i;
	for(i = 0; i < plates.size();++i){
		licenses[i].plate = plates[i];
		licenses[i].fine = fines[i];
	}


	_mergeSort(licenses, 0, (licenses.size() - 1));

	for(unsigned int j = 0; j < licenses.size(); ++j){
		plates[j] = licenses[j].plate;
		fines[j] = licenses[j].fine;
	}
}

int main()
{
  int    N;// = 1000000;
  string basename;// = "tickets9";

  cout << "Enter hashtable size> ";
  cin >> N;

  hashtable<string, int>  ht(N);
  ILplates                hashplates(ht);

  cout << "Enter base filename> ";
  cin >> basename;
  cout << endl;

  //
  // process input file of fines and license plates:
  //
  hashInput(basename, hashplates);

  //
  // debugging:
  //
  vector<string> plates = ht.Keys();
  vector<int> amounts = ht.Values();
/*
  for (size_t i = 0; i < plates.size(); ++i)
  {
    cout << plates[i] << ", " << amounts[i] << endl;
  }
	cout << "Sorting................." << endl;
	*/
	//lets try my sort here
	mergeSort(plates, amounts);
	/*
  for (size_t i = 0; i < plates.size(); ++i)
  {
    cout << plates[i] << ", " << amounts[i] << endl;
  }
	*/
	ouputToFile(basename, plates, amounts);

  return 0;
}