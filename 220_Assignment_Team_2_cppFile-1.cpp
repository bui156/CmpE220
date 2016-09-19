// Please save this as ".cpp" file

# include <conio.h>
# include <stdio.h>
# include <string>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <iostream>

using namespace std;

# include "conversions.h"

//Defining the parts of processor 
bool memory [4096][8]; 
bool flags [8];
bool validAddressFlag = 1;
bool accumulator [32] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool memoryAddressRegister [32];
bool memoryDataRegister [32];
bool programCounterRegister [32];
bool R0 [32];
bool R1 [32];
bool R2 [32];
bool R3 [32];
bool R4 [32];
bool R5 [32];
bool R6 [32];
bool R7 [32];
bool R8 [32];
bool R9 [32];
bool R10 [32];
bool R11 [32];
bool R12 [32];
bool R13 [32];
bool R14 [32];
bool R15 [32];

//Variables needed in program
char checkOperend[9];
char checkCommand[5];
char inputString[100];

//Fuction prototype for different user difined functions 
void LDA(int); //This function will copy the contents of 4 consecutive bytes starting from the specified byte address into accumulator  
void STA(int); //This function will copy the contents of accumulator into 4 consecutive bytes starting from the specified byte address
int convertHexToDecimal(char checkOperend []);
void validDataMemoryAddressCheck(int memoryLocation); 
void memoryDump();

int main(){

	int j;
	int c = 0;

	while(1)
	{

	// Taking command as input from user
	if(c == 0)
		cout<<"Enter command or 'exit' to exit out of the code: ";
	gets(inputString); 
	
	if(strcmp(inputString,"exit") == 0){
		printf("\n ---------------------- Thanks for using out code !!! ------------------------ \n");
		break;
	}

	
	//Seperating the command part from input string
	for (int i = 0; i<=5; i++) {
		if(inputString[i] == ' ' || inputString[i] == '\0'){
			j = i;
			break;
		}
		checkCommand[i] = inputString[i];
	}
	checkCommand[j] = '\0';
	string command = checkCommand;
	
	//Seperating the address part from input string
	for(int i =j+1,k=0; inputString[i] != '\0'; i++,k++) {
		checkOperend[k] = inputString[i];
		if (inputString[i+1] == '\0')
			checkOperend[k+1] = '\0';
	}
	
	//convert Hex memory to decimal
	int memoryLocation = convertBinaryTodecimal(covertHexToBinary(checkOperend));	
	
	//Validation of memory 
	validDataMemoryAddressCheck(memoryLocation);

	
	//If else lader to check for opperations to call 
	if(strcmp(command.c_str(),"LDA") == 0 && validAddressFlag == 1) { 
		//call the function to load
		LDA(memoryLocation); 
	}
	else if(strcmp(command.c_str(),"STA") == 0 && validAddressFlag == 1){
		//call the function to store 
		STA(memoryLocation);
	}
	else{
		printf("Inside main -- Invalid command !!! \n");
		continue;
	}
	
	/*//Memory dump 
	printf("Memory: \n");
	char loc[8];
	for(int i = 2040; i<=2055; i++) {
		printf("Memory Location: %X: ",i);
		char vOut[9];
		for(j=0;j<8;j++){
			vOut[j] = memory[i][j] ? '1' : '0';
		}
		vOut[8] = '\0';
		printf("%X",convertBinaryTodecimal(vOut));
		
		printf("\n");
	}
	*/
	memoryDump();
	
	for(int i=0; i<=99;i++){
		inputString[i]= '\0';
	}
	
	//Asking to loop 

	cout<<"\n\nEnter next command or 'exit' to exit out of the code: ";

c++;
					
} // do-while loop ends here
}

void LDA (int finalMemoryLocation) {
	
	for (int i = 0; i < 32; i++){
		if (i >= 0 && i <= 7)
			accumulator[i] = memory[finalMemoryLocation][i];
		if (i >= 8 && i <= 15)
			accumulator[i] = memory[finalMemoryLocation+1][i-8];
		if (i >= 16 && i <= 23)
			accumulator[i] = memory[finalMemoryLocation+2][i-16];
		if (i >= 24 && i <= 31)
			accumulator[i] = memory[finalMemoryLocation+3][i-24];
	}
	
	return ;
}

void STA(int finalMemoryLocation) {
	
	for (int i = 0; i < 32; i++){
		if (i >= 0 && i <= 7)
			memory[finalMemoryLocation][i] = accumulator[i];
		if (i >= 8 && i <= 15)
			memory[finalMemoryLocation+1][i-8] = accumulator[i];
		if (i >= 16 && i <= 23)
			memory[finalMemoryLocation+2][i-16] = accumulator[i];
		if (i >= 24 && i <= 31)
			memory[finalMemoryLocation+3][i-24] = accumulator[i];
	}
	
	return ;
}

void validDataMemoryAddressCheck(int memoryLocation){
	
	if (memoryLocation <= 1024 || memoryLocation > 4092) {
		validAddressFlag = 0;
	} 
	
}

void memoryDump() {
	/*
	int memory[4096][8];
		int flags[32];
	int validAddressFlag = 1;
		int accumulator[32] = { 1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1 };
		int memoryAddressRegister[32];
		int memoryDataRegister[32];
		int programCounterRegister[32];
		int R0[32];
		int R1[32];
		...
		int R15[32];
	*/

	int i = 0; 
	char binaryString[8];

	printf("\n Flag Register: ");
	
	for(int j=0;j<8;j++){
		printf("%d",flags[j]);
	}

	printf("\n\nSpecial Purpose registers: ");
	printf("\n Accumulator Register: "); 
	char vOut[33];
	for(int i = 0; i < 32; i++){
		vOut[i] = accumulator[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\n Memory Address Register: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = memoryAddressRegister[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\n Memory Data Register: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = memoryDataRegister[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\n Program Counter Register: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = programCounterRegister[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\n\nGeneral Purpose registers: ");
	printf("\n R0: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R0[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R1: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R1[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R2: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R2[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R3: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R3[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R4: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R4[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R5: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R5[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R6: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R6[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R7: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R7[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\n R8: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R8[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R9: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R9[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R10: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R10[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));
	
	printf("\t R11: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R11[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R12: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R12[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R13: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R13[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R14: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R14[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));

	printf("\t R15: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R15[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryTodecimal(vOut));
	
	//Memory dump 
	cout<<endl<<endl;
	printf("Memory: \n");
	char loc[8];
	for(int i = 2040; i<=2055; i++) {
		printf(" Memory Location: %X: ",i);
		char vOut[9];
		for(int j=0;j<8;j++){
			vOut[j] = memory[i][j] ? '1' : '0';
		}
		vOut[8] = '\0';
		printf("%X\t\t\t",convertBinaryTodecimal(vOut));
		
		if (i%2 != 0)
			printf("\n");
		
	}

/*
	printf("\n Memory values 2048 - 4096:");
	for (i = 2048; i < 2096; i++) {
		//Get Binary string from memory
		for (int j = 0; j < 7; j++)
			binaryString[j] = memory[i][j];
		//Call Binary to Hex Converter
		//binaryToHex(binaryString[]);
		//Print Memory Address Location and its hexValue
		printf("Memory Address %d: ", i);
		for (int k = 0; k < 2; k++)
			printf("%c", hexValue[k]);
		printf("\n");
	}
*/

	return;
}