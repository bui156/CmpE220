
# include <conio.h>
# include <stdio.h>
# include <string>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <iostream>

using namespace std;

//# include "conversions.h"

bool A[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R1[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R2[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1};
bool R3[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1};
bool R4[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1};
bool R5[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1};
bool R6[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R7[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1};
bool R8[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R9[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1};
bool R10[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R11[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1};
bool R12[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R13[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1};
bool R14[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1};
bool R15[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1};

void findSourceRegister(int x, bool *tempR1);
void findDestinationRegister(int x, bool *tempR2);
int convertBinaryTodecimal(bool *bits, int);
void convertDecimalToBinary(int dec, bool *result);
void add(int z, int x, int y);
void sub(int z, int x, int y);
void mul(int z, int x, int y);
void div(int z, int x, int y);

int main(){
	//add(1,4,3);
	//sub(1,10,11);
	//mul(1,4,3);
	div(1,13,14);
	
	return 0;
}

void div(int z, int x, int y){
	
	int i, rim, q=0, divisor, dividend;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA[32], tempA1[32], tempA2[32];
	
//	function to decide which register is deffined by x	
	findSourceRegister(x,tempR1);
//	funtion to decide which register is deffined by y
	findSourceRegister(y,tempR2);
	
	dividend = convertBinaryTodecimal(tempR1,32);
	divisor = convertBinaryTodecimal(tempR2,32);
	bool tempRX[32], tempRY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	
	while(dividend >= divisor){
		
		printf("\nIn divide function. ");
		for(int i=0; i<32; i++){
			tempRX[i] = !tempR2[i];
		}
		
		while(i>=0){
			sum = (tempRX[i]^tempRY[i]) ^ carry;
			temp1 = tempRX[i] & tempRY[i];
			temp2 = (tempRX[i] ^ tempRY[i]) & carry;
			carry = temp1 | temp2;
			tempA1[i] = sum;
			i--;
		}	
		
		i = 31;
		while(i>=0){
			sum = (tempR1[i] ^ tempA1[i]) ^ carry;
			temp1 = tempR1[i] & tempA1[i];
			temp2 = (tempR1[i] ^ tempA1[i]) & carry;
			carry = temp1 | temp2;
			tempA2[i] = sum;
			i--;
		}

		for(int i=0; i<32; i++){
			tempA2[i] = !tempA2[i];
		}
		
		i = 31;
		while(i>=0){
			sum = (tempA2[i]^tempRY[i]) ^ carry;
			temp1 = tempA2[i] & tempRY[i];
			temp2 = (tempA2[i] ^ tempRY[i]) & carry;
			carry = temp1 | temp2;
			tempA1[i] = sum;
			i--;
		}		
		
		for(int i=0; i<32; i++) {
			tempR1[i] = tempA2[i]; 
		}
		
		q++;
		dividend = 	convertBinaryTodecimal(tempA2,32);
		convertDecimalToBinary(dividend, tempR1);
		printf("\nAt the end of this loop, dividend = %d", dividend);	
	}
	
	//debugging
	printf("\n%d / %d = %d\n", dividend,divisor,q);
	
}

void mul(int z, int x, int y){
	
	int i;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempR11[64], tempR22[64], tempA[32], tempAA[64];

//	function to decide which register is deffined by x	
	findSourceRegister(x,tempR1);
//	funtion to decide which register is deffined by y
	findSourceRegister(y,tempR2);
	
	int n = convertBinaryTodecimal(tempR2,32);

//	Debugging
	int n1 = convertBinaryTodecimal(tempR1, 32);
	int n2 = convertBinaryTodecimal(tempR2, 32);
	
//	Assigning 32 array to 64 bit array by padding 0 at the 32 MSBs for multiplicxation 
	for(int i=0; i<64; i++){
		
		if(i<=31){
			tempR11[i] = 0;
			tempR22[i] = 0;
		}
		else{
			tempR11[i] = tempR1[i-32];
			tempR22[i] = tempR2[i-32];
		}
	}
	
//	Initialinzing tempAA with tempR11
	for(int i=0; i<64; i++){
		tempAA[i]=tempR11[i];
	}	
//	Multiplication logic
	for(int j=1; j<n; j++){
		i = 64;
		while(i>=0){
			sum = (tempR11[i]^tempAA[i]) ^ carry;
			temp1 = tempR11[i] & tempAA[i];
			temp2 = (tempR11[i]^tempAA[i]) & carry;
			carry = temp1 | temp2;
			tempAA[i] = sum;
			i--;
		}
	}
	
// Dibugging	
	unsigned int n3 = convertBinaryTodecimal(tempAA, 64);
	unsigned int n4 = convertBinaryTodecimal(tempR11, 64);
	unsigned int n5 = convertBinaryTodecimal(tempR22, 64);
	printf("\n%d * %d = %d\n", n1,n,n3);
	printf("\n%d * %d = %d\n", n4,n5,n3);
	
}

void add(int z, int x, int y) {
	
	int i = 31;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA[32]; 

//	function to decide which register is deffined by x	
	findSourceRegister(x,tempR1);
		
//	funtion to decide which register is deffined by y
	findSourceRegister(y,tempR2);
	
	while(i>=0){
		sum = (tempR1[i]^tempR2[i]) ^ carry;
		temp1 = tempR1[i] & tempR2[i];
		temp2 = (tempR1[i]^tempR2[i]) & carry;
		carry = temp1 | temp2;
		tempA[i] = sum;
		i--;
	}
	
// function to decide which register is deffined by z	
	findDestinationRegister(z, tempA);
/*
	printf("Addition: \n");
	for(int i=0; i<32; i++){
		printf("%d", R4[i]);
	}
	printf("\n");
	
	for(int i=0; i<32; i++){
		printf("%d", R3[i]);
	}
	printf("  +\n--------------------------------\n");
	
	for(int i=0; i<32; i++){
		printf("%d", R1[i]);
	}
	printf("\n");
*/
	
}

void sub(int z, int x, int y){
	
	int i = 31;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA1[32], tempA2[32]; 
		
//	function to decide which register is deffined by x	
	findSourceRegister(x,tempR1);	
//	funtion to decide which register is deffined by y
	findSourceRegister(y,tempR2);
	
	bool tempRX[32], tempRY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	for(int i=0; i<32; i++){
		tempRX[i] = !tempR2[i];
	}
	
	while(i>=0){
		sum = (tempRX[i]^tempRY[i]) ^ carry;
		temp1 = tempRX[i] & tempRY[i];
		temp2 = (tempRX[i] ^ tempRY[i]) & carry;
		carry = temp1 | temp2;
		tempA1[i] = sum;
		i--;
	}	
	
	i = 31;
	while(i>=0){
		sum = (tempA1[i] ^ tempR1[i]) ^ carry;
		temp1 = tempA1[i] & tempR1[i];
		temp2 = (tempA1[i] ^ tempR1[i]) & carry;
		carry = temp1 | temp2;
		tempA2[i] = sum;
		i--;
	}
	
// function to decide which register is deffined by z	
	findDestinationRegister(z, tempA2);			
	
}

/**
 * Converts hexadecimal input to binary
 */
void convertDecimalToBinary(int dec, bool *result){
	
	bool bit;
	for(int i=0; i<32; i++){
		result[i] = 0;
	}
	
	int x=dec, count=0;
	while(x != 0) {
		bit = bool(x%2);
		result[count]=bit;
		count++;
		x=x/2;
	}
}


/**
 * Converts dinary input to decimal
 */
int convertBinaryTodecimal(bool *bits, int loc){

	int counter = 0;
	int decimal = 0;
	//int loc = strlen(bits);

	while (counter < loc)
	{
		if(*(bits+counter) == 1){
			decimal = (decimal + pow(2,(loc-counter)-1));
		}
		counter++;
	}
	
	return decimal;
}


void findSourceRegister(int x, bool *tempR1){

//	Switch case to deside which register is deffined by x	
	switch(x){
		
		case 0: 
			for(int i=0; i<32; i++){
				tempR1[i] = A[i];
			}
			break;
			
		case 1: 
			for(int i=0; i<32; i++){
				tempR1[i] = R1[i];
			}
			break;

		case 2: 
			for(int i=0; i<32; i++){
				tempR1[i] = R2[i];
			}
			break;

		case 3: 
			for(int i=0; i<32; i++){
				tempR1[i] = R3[i];
			}
			break;
			
		case 4: 
			for(int i=0; i<32; i++){
				tempR1[i] = R4[i];
			}
			break;
			
		case 5: 
			for(int i=0; i<32; i++){
				tempR1[i] = R5[i];
			}
			break;
			
		case 6: 
			for(int i=0; i<32; i++){
				tempR1[i] = R6[i];
			}
			break;
			
		case 7: 
			for(int i=0; i<32; i++){
				tempR1[i] = R7[i];
			}
			break;
			
		case 8: 
			for(int i=0; i<32; i++){
				tempR1[i] = R8[i];
			}
			break;
			
		case 9: 
			for(int i=0; i<32; i++){
				tempR1[i] = R9[i];
			}
			break;

		case 10: 
			for(int i=0; i<32; i++){
				tempR1[i] = R10[i];
			}
			break;

		case 11: 
			for(int i=0; i<32; i++){
				tempR1[i] = R11[i];
			}
			break;
			
		case 12: 
			for(int i=0; i<32; i++){
				tempR1[i] = R12[i];
			}
			break;
			
		case 13: 
			for(int i=0; i<32; i++){
				tempR1[i] = R13[i];
			}
			break;
			
		case 14: 
			for(int i=0; i<32; i++){
				tempR1[i] = R14[i];
			}
			break;
			
		case 15: 
			for(int i=0; i<32; i++){
				tempR1[i] = R15[i];
			}
			break;
			
		default: 
			printf("\nWrong register input !!! \n");
			break;		
	}
}

void findDestinationRegister(int z, bool *tempA){

	switch(z){
		
		case 0: 
			for(int i=0; i<32; i++){
				A[i] = tempA[i];
			}
			break;
			
		case 1: 
			for(int i=0; i<32; i++){
				R1[i] = tempA[i];
			}
			break;

		case 2: 
			for(int i=0; i<32; i++){
				R2[i] = tempA[i];
			}
			break;

		case 3: 
			for(int i=0; i<32; i++){
				R3[i] = tempA[i];
			}
			break;
			
		case 4: 
			for(int i=0; i<32; i++){
				R4[i] = tempA[i];
			}
			break;
			
		case 5: 
			for(int i=0; i<32; i++){
				R5[i] = tempA[i];
			}
			break;
			
		case 6: 
			for(int i=0; i<32; i++){
				R6[i] = tempA[i];
			}
			break;
			
		case 7: 
			for(int i=0; i<32; i++){
				R7[i] = tempA[i];
			}
			break;
			
		case 8: 
			for(int i=0; i<32; i++){
				R8[i] = tempA[i];
			}
			break;
			
		case 9: 
			for(int i=0; i<32; i++){
				R9[i] = tempA[i];
			}
			break;

		case 10: 
			for(int i=0; i<32; i++){
				R10[i] = tempA[i];
			}
			break;

		case 11: 
			for(int i=0; i<32; i++){
				R11[i] = tempA[i];
			}
			break;
			
		case 12: 
			for(int i=0; i<32; i++){
				R12[i] = tempA[i];
			}
			break;
			
		case 13: 
			for(int i=0; i<32; i++){
				R13[i] = tempA[i];
			}
			break;
			
		case 14: 
			for(int i=0; i<32; i++){
				R14[i] = tempA[i];
			}
			break;
			
		case 15: 
			for(int i=0; i<32; i++){
				R15[i] = tempA[i];
			}
			break;
			
		default: 
			printf("\nWrong register input !!! \n");
			break;		
	}	
}
