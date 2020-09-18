/***
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

std::vector<process_stats> vec;

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

int loadData(const char* filename, bool ignoreFirstRow) {

	vec.clear();
	string line;
	std::vector<string> tokens;
	bool cont = !ignoreFirstRow;

	ifstream infile;
	infile.open(filename);

	if (infile.is_open()) {
		while (getline (infile, line)) {
			tokens.clear();
			stringstream tokenize(line);
			string newline;
			while(getline (tokenize, newline, ',')) {
				tokens.push_back(newline);
			}

			for (int j = 0; j < tokens.size(); j++) {
				if (tokens[j] == "") {
					tokens.erase(tokens.begin() + j);
				}
			}

			if ((tokens.size() == 4) && tokens[0] != "process_number" && cont) {
				process_stats stat;
				int p, s, c, i;

				p = stoi(tokens[0]);
				s = stoi(tokens[1]);
				c = stoi(tokens[2]);
				i = stoi(tokens[3]);

				stat.process_number = p;
				stat.start_time = s;
				stat.cpu_time = c;
				stat.io_time = i;
				vec.push_back(stat);

			} else if (!cont) {
				cont = true;
			}
		}
	} else {
		return COULD_NOT_OPEN_FILE;
	}
	return SUCCESS;
}


//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {
	switch (mySortOrder) {
	case 0:
		sort(vec.begin(), vec.end(), [](const auto& lhs, const auto& rhs){
			return lhs.process_number < rhs.process_number;
		});
		break;
	case 1:
		sort(vec.begin(), vec.end(), [](const auto& lhs, const auto& rhs){
			return lhs.start_time < rhs.start_time;
		});
		break;
	case 2:
		sort(vec.begin(), vec.end(), [](const auto& lhs, const auto& rhs){
			return lhs.cpu_time < rhs.cpu_time;
		});
		break;
	case 3:
		sort(vec.begin(), vec.end(), [](const auto& lhs, const auto& rhs){
			return lhs.io_time < rhs.io_time;
		});
		break;
	}
}

process_stats getNext() {
	process_stats myFirst;
	myFirst = vec[0];
	vec.erase(vec.begin());
	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return vec.size();
}


