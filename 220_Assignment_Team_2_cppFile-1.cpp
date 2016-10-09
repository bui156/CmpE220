// Please save this as ".cpp" file

//# include <conio.h>
# include <stdio.h>
# include <string>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <iostream>
# include <fstream>

using namespace std;

# include "conversions.h"

//Defining the parts of processor
bool memory [4096][8];
bool flags [8];
bool validAddressFlag = 1;
bool accumulator [32] = {1,1,1,1,1,1,1,1,
		                 0,0,0,0,0,0,0,1,
						 1,1,1,1,1,1,1,1,
						 0,0,0,0,0,0,0,1};
bool memoryAddressRegister [32];
bool memoryDataRegister [32];
bool programCounterRegister [32] = {0,0,0,0,0,0,0,0,
		                            0,0,0,0,0,0,0,0,
									0,0,0,0,0,1,0,0,
									0,0,0,0,0,0,0,0};
int startMemLocation = 1024;

//Opcodes for Registers
char tmpExit_Opcode [8] = {'1','0','1','0','1','0','1','0'};
bool exit_Opcode [8] = {1,0,1,0,1,0,1,0}; //AAh, 170d
bool R0_Opcode [8] = {0,0,0,0,0,0,0,1};	//1
bool R1_Opcode [8] = {0,0,0,0,0,0,1,0}; //2
bool R2_Opcode [8] = {0,0,0,0,0,0,1,1}; //3
bool R3_Opcode [8] = {0,0,0,0,0,1,0,0}; //4
bool R4_Opcode [8] = {0,0,0,0,0,1,0,1}; //5
bool R5_Opcode [8] = {0,0,0,0,0,1,1,0}; //6
bool R6_Opcode [8] = {0,0,0,0,0,1,1,1}; //7
bool R7_Opcode [8] = {0,0,0,0,1,0,0,0}; //8
bool R8_Opcode [8] = {0,0,0,0,1,0,0,1}; //9
bool R9_Opcode [8] = {0,0,0,0,1,0,1,0}; //10
bool R10_Opcode [8] = {0,0,0,0,1,0,1,1}; //11
bool R11_Opcode [8] = {0,0,0,0,1,1,0,0}; //12
bool R12_Opcode [8] = {0,0,0,0,1,1,0,1}; //13
bool R13_Opcode [8] = {0,0,0,0,1,1,1,0}; //14
bool R14_Opcode [8] = {0,0,0,0,1,1,1,1}; //15
bool R15_Opcode [8] = {0,0,0,1,0,0,0,0}; //16

//Opcodes for ALU Operations
bool ADD_Opcode [8] = {1,0,0,0,0,0,0,0}; //128
bool SUB_Opcode [8] = {1,0,0,0,0,0,0,1}; //129
bool MUL_Opcode [8] = {1,0,0,0,0,0,1,0}; //130
bool DIV_Opcode [8] = {1,0,0,0,0,0,1,1}; //131
bool MOD_Opcode [8] = {1,0,0,0,0,1,0,0}; //132
bool LDA_Opcode [8] = {1,0,0,0,0,1,0,1}; //133
bool STA_Opcode [8] = {1,0,0,0,0,1,1,0}; //134
bool MOV_Opcode [8] = {1,0,0,0,0,1,1,1}; //135

bool A[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R0 [32];
bool R1 [32];
bool R2 [32] = {0,0,0,0,0,0,0,0,
		        0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,1};
bool R3 [32] = {0,0,0,0,0,0,0,0,
			    0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,1,0};
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
char currentInstruction[33]; //will be string?
char operation[8];
char operand1[8];
char operand2[8];
char operand3[8];
int instructionOperation = 0;
int instructionOperand1 = 0;
int instructionOperand2 = 0;
int instructionOperand3 = 0;
int exitCodeCount = 0;

//Fuction prototype for different user difined functions
void LDA(int); //This function will copy the contents of 4 consecutive bytes starting from the specified byte address into accumulator
void STA(int); //This function will copy the contents of accumulator into 4 consecutive bytes starting from the specified byte address
void validDataMemoryAddressCheck(int memoryLocation);
void memoryDump();
void readFromFile(string fileName);
void decodeInstructionFromFile(string textLine);
void instructionIntoMemory(char* token,int memLocation);
void parseInstructionFromMemory();
void getCurrentInstruction();
void decodeInstructionOperationOperand();
void callAppropriateFunction();
void add(int z, int x, int y);
//void sub(int z, int x, int y);
//void mul(int z, int x, int y);
//void div(int z, int x, int y);
void findSourceRegister(int x, bool *tempR1);
void findDestinationRegister(int x, bool *tempR2);

int main(){

	string fileName;

	cout<<"Enter the fileName"<<endl;
	//cin>>fileName;

	fileName = "inputFile.txt";

	readFromFile(fileName);

	memoryDump();

	//Start Program
	cout<<"Starting Program"<<endl;
	startMemLocation=1024;
	while (exitCodeCount != 8){
		getCurrentInstruction();
		parseInstructionFromMemory();
		if (exitCodeCount != 8){
			decodeInstructionOperationOperand();
			callAppropriateFunction();
		}
	}

	for (int i = 0; i < 32; i++){
		printf("%c", currentInstruction[i]);
	}
	cout<<"Program Finished"<<endl;

	memoryDump();
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

	int i = 0;

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
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\n Memory Address Register: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = memoryAddressRegister[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\n Memory Data Register: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = memoryDataRegister[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\n Program Counter Register: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = programCounterRegister[i] ? '1' : '0';
	}
	vOut[33] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\n\nGeneral Purpose registers: ");
	printf("\n R0: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R0[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R1: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R1[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R2: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R2[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R3: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R3[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R4: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R4[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R5: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R5[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R6: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R6[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R7: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R7[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\n R8: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R8[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R9: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R9[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R10: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R10[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R11: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R11[i] ? '1': '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R12: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R12[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R13: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R13[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R14: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R14[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	printf("\t R15: ");
	for (i = 0; i < 32; i++) {
		vOut[i] = R15[i] ? '1' : '0';
	}
	vOut[32] = '\0';
	printf("%X",convertBinaryToDecimal(vOut));

	//Memory dump
	cout<<endl<<endl;
	printf("Memory: HEX: DEC\n");

	for(int i = 1024; i<=2055; i++) {
		printf(" Memory Location: %X: ",i);
		char vOut[9];
		for(int j=0;j<8;j++){
			vOut[j] = memory[i][j] ? '1' : '0';
		}
		vOut[8] = '\0';
		printf("%d\t\t\t",convertBinaryToDecimal(vOut));

		if (i%2 != 0)
			printf("\n");

	}

	return;
}

void readFromFile(string fileName){

	ifstream textFile;
	string textLine;

	textFile.open(fileName.c_str());

	if(access(fileName.c_str(),F_OK) != -1){

		while(true){

			getline(textFile,textLine);

			decodeInstructionFromFile(textLine);

			if(textFile.eof()){
				break;
			}

		}
		textFile.close();

	}else{
		cout<<"FILE DOES NOT EXIST"<<endl;
		cerr<<"The requested file"<<fileName<<" does not exist"<<endl;
	}

}

//Reads in Instruction from Text File
//"Decodes" Instruction into each opcode/operand.
void decodeInstructionFromFile(string textLine){

	char* pch;

	pch = strtok((char*)textLine.c_str()," ,\r");

	while (pch != NULL)
	{
		instructionIntoMemory(pch,startMemLocation);
		startMemLocation++;
		pch = strtok (NULL, " ,\r");
	}

}

//Converts operand into bits and stores into memory.
void instructionIntoMemory(char* token,int memLocation) {

	cout<<"TOKEN: "<<token<<" in memory location "<<memLocation<<endl;

	if(strcmp(token,"ADD")==0){
		cout<<"IN ADD"<<endl;
		for (int i = 0; i < 8; i++){
			memory[memLocation][i] = ADD_Opcode[i];
		}
	}else
		if(strcmp(token,"SUB")==0){
			cout<<"IN SUB"<<endl;
			for (int i = 0; i < 8; i++){
				memory[memLocation][i] = SUB_Opcode[i];
			}
		}else
			if(strcmp(token,"MUL")==0){
				cout<<"IN MUL"<<endl;
				for (int i = 0; i < 8; i++){
					memory[memLocation][i] = MUL_Opcode[i];
				}
			}else
				if(strcmp(token,"DIV")==0){
					cout<<"IN DIV"<<endl;
					for (int i = 0; i < 8; i++){
						memory[memLocation][i] = DIV_Opcode[i];
					}
				}else
					if(strcmp(token,"R0")==0){
						cout<<"IN R0"<<endl;
						for (int i = 0; i < 8; i++){
							memory[memLocation][i] = R0_Opcode[i];
						}
					}else
						if(strcmp(token,"R1")==0){
							cout<<"IN R1"<<endl;
							for (int i = 0; i < 8; i++){
								memory[memLocation][i] = R1_Opcode[i];
							}
						}else
							if(strcmp(token,"R2")==0){
								cout<<"IN R2"<<endl;
								for (int i = 0; i < 8; i++){
									memory[memLocation][i] = R2_Opcode[i];
								}
							}else
								//if((strcmp(token,"R3")==0)||(strcmp(token,"R3\r")==0)){
								if(strcmp(token,"R3")==0){
									cout<<"IN R3"<<endl;
									for (int i = 0; i < 8; i++){
										memory[memLocation][i] = R3_Opcode[i];
									}
								}else
									if(strcmp(token,"R4")==0){
										cout<<"IN R4"<<endl;
										for (int i = 0; i < 8; i++){
											memory[memLocation][i] = R4_Opcode[i];
										}
									}else
										if(strcmp(token,"R5")==0){
											cout<<"IN R5"<<endl;
											for (int i = 0; i < 8; i++){
												memory[memLocation][i] = R5_Opcode[i];
											}
										}else
											//if ((strcmp(token,"R6")==0) || (strcmp(token,"R6\r")==0)){
											if(strcmp(token,"R6")==0){
												cout<<"IN R6"<<endl;
												for (int i = 0; i < 8; i++){
													memory[memLocation][i] = R6_Opcode[i];
												}
											}else
												if(strcmp(token,"R7")==0){
													cout<<"IN R7"<<endl;
													for (int i = 0; i < 8; i++){
														memory[memLocation][i] = R7_Opcode[i];
													}
												}else
													if(strcmp(token,"R8")==0){
														cout<<"IN R8"<<endl;
														for (int i = 0; i < 8; i++){
															memory[memLocation][i] = R8_Opcode[i];
														}
													}else
														if(strcmp(token,"R9")==0){
															cout<<"IN R9"<<endl;
															for (int i = 0; i < 8; i++){
																memory[memLocation][i] = R9_Opcode[i];
															}
														}else
															if(strcmp(token,"R10")==0){
																cout<<"IN R10"<<endl;
																for (int i = 0; i < 8; i++){
																	memory[memLocation][i] = R10_Opcode[i];
																}
															}else
																if(strcmp(token,"R11")==0){
																	cout<<"IN R11"<<endl;
																	for (int i = 0; i < 8; i++){
																		memory[memLocation][i] = R11_Opcode[i];
																	}
																}else
																	if(strcmp(token,"R12")==0){
																		cout<<"IN R12"<<endl;
																		for (int i = 0; i < 8; i++){
																			memory[memLocation][i] = R12_Opcode[i];
																		}
																	}else
																		if(strcmp(token,"R13")==0){
																			cout<<"IN R13"<<endl;
																			for (int i = 0; i < 8; i++){
																				memory[memLocation][i] = R13_Opcode[i];
																			}
																		}else
																			if(strcmp(token,"R14")==0){
																				cout<<"IN R14"<<endl;
																				for (int i = 0; i < 8; i++){
																					memory[memLocation][i] = R14_Opcode[i];
																				}
																			}else
																				if(strcmp(token,"R15")==0){
																					cout<<"IN R15"<<endl;
																					for (int i = 0; i < 8; i++){
																						memory[memLocation][i] = R15_Opcode[i];
																					}
																				}else
																					if(strcmp(token,"EXIT")==0){
																						cout<<"EXIT CODE"<<endl;
																						for (int i = 0; i < 8; i++){
																							memory[memLocation][i] = exit_Opcode[i];
																						}
																					}
}
/*************************************************************/
// This function reads an byte of memory and decodes it into
// its appropriate instruction/operand.
//
// Input: 32 bit value from Memory
// Output: Breaks the instruction into its operations and three
// operands.
/*************************************************************/
void parseInstructionFromMemory(){
	//Start of Instruction Memory is 1024 --> startMemLocation

	//first 8 bits are always opcode
	//next 24 bits are always registers
	for (int i = 0; i < 32; i++){
		if (i >= 0 && i <= 7)
			operation[i] = currentInstruction[i];
		if (i >= 8 && i <= 15)
			operand1[i-8] = currentInstruction[i];
		if (i >= 16 && i <= 23)
			operand2[i-16] = currentInstruction[i];
		if (i >= 24 && i <= 31)
			operand3[i-24] = currentInstruction[i];
	}

	exitCodeCount = 0; //Reset Exit Code Count in case it was incremented by other codes matching "part" of the exit code. 

	for (int i = 0; i < 8; i++){
		if (operation[i] == tmpExit_Opcode[i])
			exitCodeCount++;
	}

	/*
	operation[9] = '\0';
	operand1[9] = '\0';
	operand2[9] = '\0';
	operand3[9] = '\0';
	 */
	return;
}

/*************************************************************/
// This function reads in 4 bytes of memory and decodes it
// into its appropriate opcode/operands.
//
// Input: 4 x 8 bit value from Memory
// Output: Returns Current Memory Instruction stored in
//         Global Variable currentInstruction[32]
/*************************************************************/
void getCurrentInstruction(){
	/*
	//Read Value from Program Counter Instruction
	char tempMemoryLocation[32]; //Will convert bool programCounterRegster to char programCounterRegister

	for (int i = 0; i < 32; i++){
		//Write to locations 0-31
		tempMemoryLocation[i] = programCounterRegister[i] ? '1': '0';
	}
	tempMemoryLocation[32] = '\0'; //Add null to end string.

	//convertBinaryToDecimal accepts a 32 bit Char string.
	int currentMemoryLocation = convertBinaryToDecimal(tempMemoryLocation);

	//Code is similar to LDA command
	//Store in currentInstruction instead of accumulator
	for (int i = 0; i < 32; i++){
		if (i >= 0 && i <= 7)
			currentInstruction[i] = memory[currentMemoryLocation][i] ? '1': '0';
		if (i >= 8 && i <= 15)
			currentInstruction[i] = memory[currentMemoryLocation+1][i-8] ? '1': '0';
		if (i >= 16 && i <= 23)
			currentInstruction[i] = memory[currentMemoryLocation+2][i-16] ? '1': '0';
		if (i >= 24 && i <= 31)
			currentInstruction[i] = memory[currentMemoryLocation+3][i-24] ? '1': '0';
	}
	currentInstruction[33] = '\0';

	//parseInstructionFromMemory();

	//decodeInstructionOperationOperand();

	//callAppropriateFunction();
	//Increment Program Counter + 4
	//programCounterRegister + 4
	return;
	*/

	for (int i = 0; i < 32; i++){
		if (i >= 0 && i <= 7)
			currentInstruction[i] = memory[startMemLocation][i] ? '1' : '0';
		if (i >= 8 && i <= 15)
			currentInstruction[i] = memory[startMemLocation+1][i-8] ? '1' : '0';
		if (i >= 16 && i <= 23)
			currentInstruction[i] = memory[startMemLocation+2][i-16] ? '1' : '0';
		if (i >= 24 && i <= 31)
			currentInstruction[i] = memory[startMemLocation+3][i-24] ? '1' : '0';
	}
	currentInstruction[33] = '\0';
	if (startMemLocation == 1032);

	startMemLocation += 4;

}
/*******************************************
 * This function changes the operation/opcode
 * into an integer value.
 *******************************************/
void decodeInstructionOperationOperand(){
	int addCount = 0, subCount = 0, mulCount = 0, divCount = 0,
		modCount = 0, ldaCount = 0, staCount = 0, movCount = 0;

	int operand1R0Count = 0, operand1R1Count = 0, operand1R2Count = 0, operand1R3Count = 0,
		operand1R4Count = 0, operand1R5Count = 0, operand1R6Count = 0, operand1R7Count = 0,
		operand1R8Count = 0, operand1R9Count = 0, operand1R10Count = 0, operand1R11Count = 0,
		operand1R12Count = 0, operand1R13Count = 0, operand1R14Count = 0, operand1R15Count = 0;

	int operand2R0Count = 0, operand2R1Count = 0, operand2R2Count = 0, operand2R3Count = 0,
		operand2R4Count = 0, operand2R5Count = 0, operand2R6Count = 0, operand2R7Count = 0,
		operand2R8Count = 0, operand2R9Count = 0, operand2R10Count = 0, operand2R11Count = 0,
		operand2R12Count = 0, operand2R13Count = 0, operand2R14Count = 0, operand2R15Count = 0;

	int operand3R0Count = 0, operand3R1Count = 0, operand3R2Count = 0, operand3R3Count = 0,
		operand3R4Count = 0, operand3R5Count = 0, operand3R6Count = 0, operand3R7Count = 0,
		operand3R8Count = 0, operand3R9Count = 0, operand3R10Count = 0, operand3R11Count = 0,
		operand3R12Count = 0, operand3R13Count = 0, operand3R14Count = 0, operand3R15Count = 0;

	char tmpADD_Opcode[8], tmpSUB_Opcode[8], tmpMUL_Opcode[8], tmpDIV_Opcode[8],
		 tmpMOD_Opcode[8], tmpLDA_Opcode[8], tmpSTA_Opcode[8], tmpMOV_Opcode[8],
		 tmpR0_Opcode[8], tmpR1_Opcode[8], tmpR2_Opcode[8], tmpR3_Opcode[8],
		 tmpR4_Opcode[8], tmpR5_Opcode[8], tmpR6_Opcode[8], tmpR7_Opcode[8],
		 tmpR8_Opcode[8], tmpR9_Opcode[8], tmpR10_Opcode[8], tmpR11_Opcode[8],
		 tmpR12_Opcode[8], tmpR13_Opcode[8], tmpR14_Opcode[8], tmpR15_Opcode[8];

	for (int i = 0; i< 8; i++){
		tmpADD_Opcode[i] = ADD_Opcode[i] ? '1' : '0';
		tmpSUB_Opcode[i] = SUB_Opcode[i] ? '1' : '0';
		tmpMUL_Opcode[i] = MUL_Opcode[i] ? '1' : '0';
		tmpDIV_Opcode[i] = DIV_Opcode[i] ? '1' : '0';
		tmpMOD_Opcode[i] = MOD_Opcode[i] ? '1' : '0';
		tmpLDA_Opcode[i] = LDA_Opcode[i] ? '1' : '0';
		tmpSTA_Opcode[i] = STA_Opcode[i] ? '1' : '0';
		tmpMOV_Opcode[i] = MOV_Opcode[i] ? '1' : '0';
		tmpR0_Opcode[i] = R0_Opcode[i] ? '1' : '0';
		tmpR1_Opcode[i] = R1_Opcode[i] ? '1' : '0';
		tmpR2_Opcode[i] = R2_Opcode[i] ? '1' : '0';
		tmpR3_Opcode[i] = R3_Opcode[i] ? '1' : '0';
		tmpR4_Opcode[i] = R4_Opcode[i] ? '1' : '0';
		tmpR5_Opcode[i] = R5_Opcode[i] ? '1' : '0';
		tmpR6_Opcode[i] = R6_Opcode[i] ? '1' : '0';
		tmpR7_Opcode[i] = R7_Opcode[i] ? '1' : '0';
		tmpR8_Opcode[i] = R8_Opcode[i] ? '1' : '0';
		tmpR9_Opcode[i] = R9_Opcode[i] ? '1' : '0';
		tmpR10_Opcode[i] = R10_Opcode[i] ? '1' : '0';
		tmpR11_Opcode[i] = R11_Opcode[i] ? '1' : '0';
		tmpR12_Opcode[i] = R12_Opcode[i] ? '1' : '0';
		tmpR13_Opcode[i] = R13_Opcode[i] ? '1' : '0';
		tmpR14_Opcode[i] = R14_Opcode[i] ? '1' : '0';
		tmpR15_Opcode[i] = R15_Opcode[i] ? '1' : '0';
	}

	for (int i = 0; i < 8; i++){
		if (operation[i]==tmpADD_Opcode[i])
			addCount++;
		if (operation[i]==tmpSUB_Opcode[i])
			subCount++;
		if (operation[i]==tmpMUL_Opcode[i])
			mulCount++;
		if (operation[i]==tmpDIV_Opcode[i])
			divCount++;
		if (operation[i]==tmpMOD_Opcode[i])
			modCount++;
		if (operation[i]==tmpLDA_Opcode[i])
			ldaCount++;
		if (operation[i]==tmpSTA_Opcode[i])
			staCount++;
		if (operation[i]==tmpMOV_Opcode[i])
			movCount++;

		if (operand1[i]==tmpR0_Opcode[i])
			operand1R0Count++;
		if (operand1[i]==tmpR1_Opcode[i])
			operand1R1Count++;
		if (operand1[i]==tmpR2_Opcode[i])
			operand1R2Count++;
		if (operand1[i]==tmpR3_Opcode[i])
			operand1R3Count++;
		if (operand1[i]==tmpR4_Opcode[i])
			operand1R4Count++;
		if (operand1[i]==tmpR5_Opcode[i])
			operand1R5Count++;
		if (operand1[i]==tmpR6_Opcode[i])
			operand1R6Count++;
		if (operand1[i]==tmpR7_Opcode[i])
			operand1R7Count++;
		if (operand1[i]==tmpR8_Opcode[i])
			operand1R8Count++;
		if (operand1[i]==tmpR9_Opcode[i])
			operand1R9Count++;
		if (operand1[i]==tmpR10_Opcode[i])
			operand1R10Count++;
		if (operand1[i]==tmpR11_Opcode[i])
			operand1R11Count++;
		if (operand1[i]==tmpR12_Opcode[i])
			operand1R12Count++;
		if (operand1[i]==tmpR13_Opcode[i])
			operand1R13Count++;
		if (operand1[i]==tmpR14_Opcode[i])
			operand1R14Count++;
		if (operand1[i]==tmpR15_Opcode[i])
			operand1R15Count++;

		if (operand2[i]==tmpR0_Opcode[i])
			operand2R0Count++;
		if (operand2[i]==tmpR1_Opcode[i])
			operand2R1Count++;
		if (operand2[i]==tmpR2_Opcode[i])
			operand2R2Count++;
		if (operand2[i]==tmpR3_Opcode[i])
			operand2R3Count++;
		if (operand2[i]==tmpR4_Opcode[i])
			operand2R4Count++;
		if (operand2[i]==tmpR5_Opcode[i])
			operand2R5Count++;
		if (operand2[i]==tmpR6_Opcode[i])
			operand2R6Count++;
		if (operand2[i]==tmpR7_Opcode[i])
			operand2R7Count++;
		if (operand2[i]==tmpR8_Opcode[i])
			operand2R8Count++;
		if (operand2[i]==tmpR9_Opcode[i])
			operand2R9Count++;
		if (operand2[i]==tmpR10_Opcode[i])
			operand2R10Count++;
		if (operand2[i]==tmpR11_Opcode[i])
			operand2R11Count++;
		if (operand2[i]==tmpR12_Opcode[i])
			operand2R12Count++;
		if (operand2[i]==tmpR13_Opcode[i])
			operand2R13Count++;
		if (operand2[i]==tmpR14_Opcode[i])
			operand2R14Count++;
		if (operand2[i]==tmpR15_Opcode[i])
			operand2R15Count++;

		if (operand3[i]==tmpR0_Opcode[i])
			operand3R0Count++;
		if (operand3[i]==tmpR1_Opcode[i])
			operand3R1Count++;
		if (operand3[i]==tmpR2_Opcode[i])
			operand3R2Count++;
		if (operand3[i]==tmpR3_Opcode[i])
			operand3R3Count++;
		if (operand3[i]==tmpR4_Opcode[i])
			operand3R4Count++;
		if (operand3[i]==tmpR5_Opcode[i])
			operand3R5Count++;
		if (operand3[i]==tmpR6_Opcode[i])
			operand3R6Count++;
		if (operand3[i]==tmpR7_Opcode[i])
			operand3R7Count++;
		if (operand3[i]==tmpR8_Opcode[i])
			operand3R8Count++;
		if (operand3[i]==tmpR9_Opcode[i])
			operand3R9Count++;
		if (operand3[i]==tmpR10_Opcode[i])
			operand3R10Count++;
		if (operand3[i]==tmpR11_Opcode[i])
			operand3R11Count++;
		if (operand3[i]==tmpR12_Opcode[i])
			operand3R12Count++;
		if (operand3[i]==tmpR13_Opcode[i])
			operand3R13Count++;
		if (operand3[i]==tmpR14_Opcode[i])
			operand3R14Count++;
		if (operand3[i]==tmpR15_Opcode[i])
			operand3R15Count++;
	}

	if (addCount==8)
		instructionOperation=128;
	else if (subCount==8)
		instructionOperation=129;
	else if (mulCount==8)
		instructionOperation=130;
	else if (divCount==8)
		instructionOperation=131;
	else if (modCount==8)
		instructionOperation=132;
	else if (ldaCount==8)
		instructionOperation=133;
	else if (staCount==8)
		instructionOperation=134;
	else if (movCount==8)
		instructionOperation=135;

	if (operand1R0Count==8)
		instructionOperand1=0;
	else if (operand1R1Count==8)
		instructionOperand1=1;
	else if (operand1R2Count==8)
		instructionOperand1=2;
	else if (operand1R3Count==8)
		instructionOperand1=3;
	else if (operand1R4Count==8)
		instructionOperand1=4;
	else if (operand1R5Count==8)
		instructionOperand1=5;
	else if (operand1R6Count==8)
		instructionOperand1=6;
	else if (operand1R7Count==8)
		instructionOperand1=7;
	else if (operand1R8Count==8)
		instructionOperand1=8;
	else if (operand1R9Count==8)
		instructionOperand1=9;
	else if (operand1R10Count==8)
		instructionOperand1=10;
	else if (operand1R11Count==8)
		instructionOperand1=11;
	else if (operand1R12Count==8)
		instructionOperand1=12;
	else if (operand1R13Count==8)
		instructionOperand1=13;
	else if (operand1R14Count==8)
		instructionOperand1=14;
	else if (operand1R15Count==8)
		instructionOperand1=15;

	if (operand2R0Count==8)
		instructionOperand2=0;
	else if (operand2R1Count==8)
		instructionOperand2=1;
	else if (operand2R2Count==8)
		instructionOperand2=2;
	else if (operand2R3Count==8)
		instructionOperand2=3;
	else if (operand2R4Count==8)
		instructionOperand2=4;
	else if (operand2R5Count==8)
		instructionOperand2=5;
	else if (operand2R6Count==8)
		instructionOperand2=6;
	else if (operand2R7Count==8)
		instructionOperand2=7;
	else if (operand2R8Count==8)
		instructionOperand2=8;
	else if (operand2R9Count==8)
		instructionOperand2=9;
	else if (operand2R10Count==8)
		instructionOperand2=10;
	else if (operand2R11Count==8)
		instructionOperand2=11;
	else if (operand2R12Count==8)
		instructionOperand2=12;
	else if (operand2R13Count==8)
		instructionOperand2=13;
	else if (operand2R14Count==8)
		instructionOperand2=14;
	else if (operand2R15Count==8)
		instructionOperand2=15;

	if (operand3R0Count==8)
		instructionOperand3=0;
	else if (operand3R1Count==8)
		instructionOperand3=1;
	else if (operand3R2Count==8)
		instructionOperand3=2;
	else if (operand3R3Count==8)
		instructionOperand3=3;
	else if (operand3R4Count==8)
		instructionOperand3=4;
	else if (operand3R5Count==8)
		instructionOperand3=5;
	else if (operand3R6Count==8)
		instructionOperand3=6;
	else if (operand3R7Count==8)
		instructionOperand3=7;
	else if (operand3R8Count==8)
		instructionOperand3=8;
	else if (operand3R9Count==8)
		instructionOperand3=9;
	else if (operand3R10Count==8)
		instructionOperand3=10;
	else if (operand3R11Count==8)
		instructionOperand3=11;
	else if (operand3R12Count==8)
		instructionOperand3=12;
	else if (operand3R13Count==8)
		instructionOperand3=13;
	else if (operand3R14Count==8)
		instructionOperand3=14;
	else if (operand3R15Count==8)
		instructionOperand3=15;

	return;
}
/*******************************************
 This function decodes operations and  calls
 the appropriate function.
 *******************************************/
void callAppropriateFunction(){
	if (instructionOperation==128){
		add(instructionOperand1, instructionOperand2, instructionOperand3);
		cout<<"add"<<endl;
	}
/*
	if (instructionOperation==128)
		add(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==129)
		sub(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==130)
		mul(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==131)
		div(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==132)
		mod(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==133)
		lda();
	else if (instructionOperation==134)
		sta();
	else if (instructionOperation==135)
		mov();
*/
	return;
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
