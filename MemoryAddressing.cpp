//============================================================================
// Name        : MemoryAddressing.cpp
// Author      : Nishant Patil
// Version     :
// Copyright   : Your copyright notice
// Description : Complete Memory Addressing Modes
//============================================================================

#include <iostream>
#include <string.h>
#include <cstdlib>

using namespace std;

int MemoryAddressing(int imm=0, int rdx=0, int rcx=0, int S=1){
	int address;
	address  = imm + rdx + rcx*S;
	return address;
}

int StringMemoryAddressing(string str){
	unsigned first = 0;
	unsigned last = str.find('(');
	string strNew = str.substr (first,last-first);
	long int int_value1 = std::strtol(strNew.c_str(), 0, 16);

	first = str.find('(');
	last = str.find(',');
	strNew = str.substr (first+1,last-first-1);
	long int int_value2 = std::strtol(strNew.c_str(), 0, 16);

	first = str.find(',',last);
	last = str.find(',',last+1);
	strNew = str.substr (first+1,last-first-1);
	long int int_value3 = std::strtol(strNew.c_str(), 0, 16);

	first = str.find(',',last);
	last = str.find(')',last+1);
	strNew = str.substr (first+1,last-first-1);
	long int int_value4 = std::strtol(strNew.c_str(), 0, 10);

	int addr = MemoryAddressing(int_value1,int_value2,int_value3,int_value4);
	return addr;
}

int main() {

	int memadd;
	string str1 = "0xff(0x20,0x30,2)";

	memadd = StringMemoryAddressing(str1);
	cout << memadd << endl;

	return 0;
}
