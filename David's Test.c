/***********************************
Name: David Bui
Description: Homework 1
Class: CmpE 220
Professor Ishie
***********************************/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <unistd.h>

//#define sixteen0 1;
//#define sixteen1 16;
//#define sixteen2 256;
//#define sixteen3 4096;

//Used to convert Hex value to Dec. 
static const int sixteen0 = 1;
static const int sixteen1 = 16;
static const int sixteen2 = 256;
static const int sixteen3 = 4096;
static const int sixteen4 = 65536;
static const int sixteen5 = 1048576;
static const int sixteen6 = 16777216;
static const int sixteen7 = 268453456;

static const int asciiDigit = 48; //Used to convert char to int
static const int asciiAlpha = 55; //Used to convert Uppercase Char to int. 


int memory[20000000][8] = { 0 };
int accumulator[32] = { 0 };
int memoryAddressRegister[32] = { 0 };
int memoryDataRegister[32] = { 0 };
int programCounter[32] = { 0 };
int flags[32] = { 0 }; 

/*Function Declarations*/
void lda(char address[], int sizeOfAddress);
int hexToDec(char position1, char position0);
int power(int base, int exp);

/*Main*/
int main(void) {
	printf("Hello World!!\n");

	char userInput[32];
	char opCode[3];
	char LDAMemoryAddress[8];

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 32; j++) {
			printf("%d", memory[i][j]);
		}
	}

	//initialize accumulator
	for (int i = 0; i < 32; i++)
		accumulator[i] = 1;

	printf("\n");

	//print contents of accumulator
	printf("Accumulator before load:\n");
	for (int i = 0; i < 32; i++)
		printf("%d", accumulator[i]);

	printf("\n");

	printf("Please enter a command:");
	scanf_s("%[^\n]", userInput, (unsigned)_countof(userInput));
	printf("break time: %s\n", userInput);

	for (int i = 0; i < 32; i++) {
		if (userInput[i] != '\0')
			printf("%c\n", userInput[i]);
		int j = strlen (userInput);
		printf("%d", j);
		if (userInput[i] == '\0')
			i = 31;
	}
	printf("newline\n");

	int userInputLength = strlen(userInput);
	printf("%d", userInputLength);

	printf("newline\n");

	//Parse userInputLength to opCode and memoryAddress
	for (int i = 0; i < userInputLength - 1; i++) {
		if (i < 3)
			opCode[i] = toupper(userInput[i]);
		if (i > 2)
			LDAMemoryAddress[i-3] = toupper(userInput[i+1]);
	}

	/*
	char y = LDAMemoryAddress[0];
	int z = y;
	printf("char %c, int %d", y, z);
	*/

	/*
	for (int i = 0; i < 16; i++)
		memoryAddress[i] = 'a';
	*/
	int LDAMemoryAddressLength = sizeof(LDAMemoryAddress);

	for (int i = 0; i < sizeof(LDAMemoryAddress); i++)
		printf("%c", LDAMemoryAddress[i]);

	lda(LDAMemoryAddress, LDAMemoryAddressLength);

	//Print contents of accumulator
	printf("Accumulator after load:\n");
	for (int i = 0; i < 32; i++)
		printf("%d", accumulator[i]);

	return 0;
}

/*Function Definitions*/
//lda will load contents from memory address to accumulator
void lda(char address[], int sizeOfAddress) {
	/*
	int memoryLocation1;
	int memoryLocation2;
	int memoryLocation3;
	int memoryLocation4;
	int memoryLocation5;
	int memoryLocation6;
	int memoryLocation7;
	int memoryLocation8;
	*/

	int memoryLocation[8];

	//Convert string of characters to array of ints
	for (int i = 0; i < sizeOfAddress; i++) {
		memoryLocation[i] = address[i];
		printf("memorylocation %d, address %c", memoryLocation[i], address[i]);

		//Convert array of ints (with their ascii int value) to decimal value
		if (memoryLocation[i] >= 48 && memoryLocation[i] <= 57) //isdigit
			memoryLocation[i] = memoryLocation[i] - asciiDigit;
		else //isalpha
			memoryLocation[i] = memoryLocation[i] - asciiAlpha;
	}

	//Convert array of ints to decimal value
	int finalMemoryLocation = 0;
	for (int i = 7; i > -1; i--) {
		finalMemoryLocation += (memoryLocation[i] * power(16, abs(i - 7)));
	}

	//Calculate final memory location. EX: LDA 1234abcd = 305441741
	for (int i = 0; i < 32; i++){
		if (0 <= i && i <= 7)
			accumulator[i] = memory[finalMemoryLocation][i];
		if (8 <= i && i <= 15)
			accumulator[i] = memory[finalMemoryLocation][i-8];
		if (16 <= i && i <= 23)
			accumulator[i] = memory[finalMemoryLocation][i-16];
		if (24 <= i && i <= 31)
			accumulator[i] = memory[finalMemoryLocation][i-24];
	}

	/*
	for (int i = 0; i < 8; i + 2) {
		if (((int)address[i] <= 57) || ((int)address[i] >= 48)) //isdigit
			if (((int)address[i + 1] <= 57) || ((int)address[i + 1] >= 48))
				;//isdigit
			else // is alpha
				;
		else //is alpha
			if (((int)address[i + 1] <= 57) || ((int)address[i + 1] >= 48)) //isdigit
				;
			else //is alpha
				;
	}
	*/

	/*
	int u = atoi(address[0]);
	printf("%d", u);

	memoryLocation1 = hexToDec(atoi(address[0]), atoi(address[1]));
	memoryLocation2 = hexToDec(atoi(address[2]), atoi(address[3]));
	memoryLocation3 = hexToDec(atoi(address[4]), atoi(address[5]));
	memoryLocation4 = hexToDec(atoi(address[6]), atoi(address[7]));
	*/
	/*
	memoryLocation1 = hexToDec(address[0], address[1]);
	memoryLocation2 = hexToDec(address[2], address[3]);
	memoryLocation3 = hexToDec(address[4], address[5]);
	memoryLocation4 = hexToDec(address[6], address[7]);
	*/

	//finalMemoryLocation = (address[0] * sixteen0);


	//LDA can send array to hex to dec. 
	//8 character long address? or 4 sets of 2 char long?

	int length = sizeof(address);
	printf("Length of passed in array... %d\n", length);

	for (int i = 0; i < sizeOfAddress; i++) {
		printf("%c", address[i]);
	}
	//convert memory address (hex value) to decimal
	//write contents of memory address to accumulator
	for (int j = 0; j < 32; j++);
		//accumulator[j] = memory[memoryAddressInt][j];
	return;
}
int hexToDec(char position1, char position0){
	int hexValue;
	int hexPosition0;
	int hexPosition1;

	if (position1 = '0')
		hexPosition1 = 0;
	else if (position1 = '1')
		hexPosition1 = 1;
	else if (position1 = '2')
		hexPosition1 = 2;
	else if (position1 = '3')
		hexPosition1 = 3;
	else if (position1 = '4')
		hexPosition1 = 4;
	else if (position1 = '5')
		hexPosition1 = 5;
	else if (position1 = '6')
		hexPosition1 = 6;
	else if (position1 = '7')
		hexPosition1 = 7;
	else if (position1 = '8')
		hexPosition1 = 8;
	else if (position1 = '9')
		hexPosition1 = 9;
	else if (position1 = 'A')
		hexPosition1 = 10;
	else if (position1 = 'B')
		hexPosition1 = 11;
	else if (position1 = 'C')
		hexPosition1 = 12;
	else if (position1 = 'D')
		hexPosition1 = 13;
	else if (position1 = 'E')
		hexPosition1 = 14;
	else if (position1 = 'F')
		hexPosition1 = 15;
	else
		;

	if (position0 = '0')
		hexPosition0 = 0;
	else if (position0 = '1')
		hexPosition0 = 1;
	else if (position0 = '2')
		hexPosition0 = 2;
	else if (position0 = '3')
		hexPosition0 = 3;
	else if (position0 = '4')
		hexPosition0 = 4;
	else if (position0 = '5')
		hexPosition0 = 5;
	else if (position0 = '6')
		hexPosition0 = 6;
	else if (position0 = '7')
		hexPosition0 = 7;
	else if (position0 = '8')
		hexPosition0 = 8;
	else if (position0 = '9')
		hexPosition0 = 9;
	else if (position0 = 'A')
		hexPosition0 = 10;
	else if (position0 = 'B')
		hexPosition0 = 11;
	else if (position0 = 'C')
		hexPosition0 = 12;
	else if (position0 = 'D')
		hexPosition0 = 13;
	else if (position0 = 'E')
		hexPosition0 = 14;
	else if (position0 = 'F')
		hexPosition0 = 15;
	else
		;

	printf("Char %c", hexPosition0);
	printf("Int %d", hexPosition0);

	hexValue = 1;

	//does not work
	//hexValue = (((atoi(hexPosition0)) * sixteen0) + ((atoi(hexPosition1)) * sixteen1));
	
	hexValue = ((hexPosition1 * sixteen1) + (hexPosition0 * sixteen0));

	//repeat above for hexPosition 0, 1, 2, 3, 4, 5, 6, 7, 8...

	return hexValue;
}

int power(int base, int exp) {
	if (exp == 0)
		return 1;
	else if (exp % 2)
		return base * power(base, exp - 1);
	else {
		int temp = power(base, exp / 2);
		return temp * temp;
	}
}
