<<<<<<< HEAD
/*
 * conversions.h
 *
 *  Created on: Oct 8, 2016
 *      Author: NISH
 */

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#pragma once


/**
* Converts hexadecimal input to binary
*/

#include <string>
#include <iostream>
#include <string.h>
using namespace std;

char* covertHexToBinary(char *bits) {

	string sReturn = "";
	int i;
	int len = strlen(bits);
	for (i = 0; i <= len - 1; i++)
	{
		switch (*(bits + i))
		{
		case '0': sReturn.append("0000"); break;
		case '1': sReturn.append("0001"); break;
		case '2': sReturn.append("0010"); break;
		case '3': sReturn.append("0011"); break;
		case '4': sReturn.append("0100"); break;
		case '5': sReturn.append("0101"); break;
		case '6': sReturn.append("0110"); break;
		case '7': sReturn.append("0111"); break;
		case '8': sReturn.append("1000"); break;
		case '9': sReturn.append("1001"); break;
		case 'a': sReturn.append("1010"); break;
		case 'A': sReturn.append("1010"); break;
		case 'b': sReturn.append("1011"); break;
		case 'B': sReturn.append("1011"); break;
		case 'c': sReturn.append("1100"); break;
		case 'C': sReturn.append("1100"); break;
		case 'd': sReturn.append("1101"); break;
		case 'D': sReturn.append("1101"); break;
		case 'e': sReturn.append("1110"); break;
		case 'E': sReturn.append("1110"); break;
		case 'f': sReturn.append("1111"); break;
		case 'F': sReturn.append("1111"); break;
		}
	}

	strcpy(bits, sReturn.c_str());
	//strcpy_s(bits, sReturn);

	return bits;
}


/**
* Converts Binary to Decimal of input
*/
int convertBinaryTodecimal(char *bits) {

	int counter = 0;
	int decimal = 0;
	int loc = strlen(bits);

	while (counter < loc)
	{
		if (*(bits + counter) == '1') {
			decimal = (decimal + pow(2, (loc - counter) - 1));
		}
		counter++;
	}
	return decimal;
}



#endif /* CONVERSIONS_H_ */
=======
# include <string.h>

/**
 * Converts hexadecimal input to binary
 */
char* covertHexToBinary(char *bits){

	string sReturn = "";
	int i;
	int len = strlen(bits);
	for (i = 0; i <= len-1;i++)
	{
		switch (*(bits+i))
		{
			case '0': sReturn.append ("0000"); break;
			case '1': sReturn.append ("0001"); break;
			case '2': sReturn.append ("0010"); break;
			case '3': sReturn.append ("0011"); break;
			case '4': sReturn.append ("0100"); break;
			case '5': sReturn.append ("0101"); break;
			case '6': sReturn.append ("0110"); break;
			case '7': sReturn.append ("0111"); break;
			case '8': sReturn.append ("1000"); break;
			case '9': sReturn.append ("1001"); break;
			case 'a': sReturn.append ("1010"); break;
			case 'A': sReturn.append ("1010"); break;
			case 'b': sReturn.append ("1011"); break;
			case 'B': sReturn.append ("1011"); break;
			case 'c': sReturn.append ("1100"); break;
			case 'C': sReturn.append ("1100"); break;
			case 'd': sReturn.append ("1101"); break;
			case 'D': sReturn.append ("1101"); break;
			case 'e': sReturn.append ("1110"); break;
			case 'E': sReturn.append ("1110"); break;
			case 'f': sReturn.append ("1111"); break;
			case 'F': sReturn.append ("1111"); break;
		}
	}

	strcpy(bits,sReturn.c_str());

	return bits;
}


/**
 * Converts Binary to Decimal of input
 */
int convertBinaryToDecimal(char *bits){

	int counter = 0;
	int decimal = 0;
	int loc = strlen(bits);

	while (counter < loc)
	{
		if(*(bits+counter) == '1'){
			decimal = (decimal + pow(2,(loc-counter)-1));
		}
		counter++;
	}
	return decimal;
}
>>>>>>> master
