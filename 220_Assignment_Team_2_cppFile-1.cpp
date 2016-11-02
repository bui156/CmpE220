// Please save this as ".cpp" file

//# include <conio.h>
# include <stdio.h>
# include <string>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <iostream>
# include <fstream>
# include <cstdlib>
# include <string.h>

using namespace std;

//# include "conversions.h"

//Defining the parts of processor
bool memory [4096][8];
bool flags [8];
bool signFlag=0, overflowFlag=0, zeroFlag=0, carryFlag=0;
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
//Program Counter is startMemLocation
int startMemLocation = 1024;
//L1, L2, and L3 are the address locations in instruction memory for the Labels.
int L1 = 0;
int L2 = 0;
int L3 = 0;
int jal = 0;
//operationType will be the "opcode" of the operation to be taken.
int operationType = 0;

//Stack Pointer = 1536 because Stack starts from there.
int basePointer = 1536;
int stackPointer = 1536;

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
bool L1_Opcode [8] = {0,0,0,1,0,0,0,1};	//17
bool L2_Opcode [8] = {0,0,0,1,0,0,1,0};	//18
bool L3_Opcode [8] = {0,0,0,1,0,0,1,1};	//19

//Opcodes for ALU Operations
bool ADD_Opcode [8] = {1,0,0,0,0,0,0,0}; //128
bool SUB_Opcode [8] = {1,0,0,0,0,0,0,1}; //129
bool MUL_Opcode [8] = {1,0,0,0,0,0,1,0}; //130
bool DIV_Opcode [8] = {1,0,0,0,0,0,1,1}; //131
bool MOD_Opcode [8] = {1,0,0,0,0,1,0,0}; //132
bool LDA_Opcode [8] = {1,0,0,0,0,1,0,1}; //133
bool STA_Opcode [8] = {1,0,0,0,0,1,1,0}; //134
bool MOV_Opcode [8] = {1,0,0,0,0,1,1,1}; //135
bool MVI_Opcode [8] = {1,0,0,0,1,0,0,0}; //136
bool JMP_Opcode [8] = {1,0,0,0,1,0,0,1}; //137
bool JGT_Opcode [8] = {1,0,0,0,1,0,1,0}; //138
bool JLT_Opcode [8] = {1,0,0,0,1,0,1,1}; //139
bool JNE_Opcode [8] = {1,0,0,0,1,1,0,0}; //140
bool JEQ_Opcode [8] = {1,0,0,0,1,1,0,1}; //141
bool DMP_Opcode [8] = {1,0,0,0,1,1,1,0}; //142
bool RET_Opcode [8] = {1,0,0,0,1,1,1,1}; //143
bool JAL_Opcode [8] = {1,0,0,1,0,0,0,0}; //144

bool A[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool R0 [32];
bool R1 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,1}; //1
bool R2 [32] = {0,0,0,0,0,0,0,0,
		        0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,1,0}; //2
bool R3 [32] = {0,0,0,0,0,0,0,0,
			    0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,1,1}; //3
bool R4 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,1,0,0}; //4
bool R5 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,1,0,1}; //5
bool R6 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,1,1,0}; //6
bool R7 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,1,1,1}; //7
bool R8 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,1,0,0,0}; //8
bool R9 [32] = {0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,1,0,0,1}; //9
bool R10 [32] = {0,0,0,0,0,0,0,0,
				 0,0,0,0,0,0,0,0,
			  	 0,0,0,0,0,0,0,0,
				 0,0,0,0,1,0,1,0}; //10
bool R11 [32];
bool R12 [32];
bool R13 [32];
bool R14 [32];
bool R15 [32];

//flags for mvi and jmp, ret, and jal instructions
bool MVI_flag = false;
bool JMP_flag = false;
bool RET_flag = false;
bool JAL_flag = false;

//Variables needed in program
char checkOperend[9];
char checkCommand[5];
char inputString[100];
//Instruction Fetch and Decode
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

//Function prototype for different user defined functions
void LDA(int); //This function will copy the contents of 4 consecutive bytes starting from the specified byte address into accumulator
void STA(int); //This function will copy the contents of accumulator into 4 consecutive bytes starting from the specified byte address
void validDataMemoryAddressCheck(int memoryLocation);
void memoryDump();
//Reading input from file and storing into instruction memory.
void readFromFile(string fileName);
void decodeInstructionFromFile(string textLine);
void instructionIntoMemory(char* token,int memLocation);
int convertBinaryToDecimal(char* bits);
string convertDecimalToBinary(int n);
////////////////////////////////////////////////
//Instruction Fetch and Decode
void parseInstructionFromMemory();
void getCurrentInstruction();
void decodeALUInstructionOperands();
void decodeMVIInstructionOperand();
void decodeJMPInstructionOperand();
void decodeDMPInstructionOperand();
void decodeRETInstructionOperand();
void decodeJALInstructionOperand();
void callAppropriateFunction();
//void add(int z, int x, int y);
//void sub(int z, int x, int y);
//void mul(int z, int x, int y);
//void div(int z, int x, int y);
//void findSourceRegister(int x, bool *tempR1);
//void findDestinationRegister(int x, bool *tempR2);
//int convertBinaryTodecimal(bool *bits, int);
//void convertDecimalToBinary(int dec, bool *result);
int MemoryAddressing(int imm, int rdx, int rcx, int S);
int StringMemoryAddressing(string str);
int StringRegistersToInt(string strNew);

// Function declaration for ALU operation
void findSourceRegister(int x, bool *tempR1);
void findDestinationRegister(int x, bool *tempR2);
int convertBinaryToDecimal_N(bool *bits, int);
void convertDecimalToBinary_N(int dec, bool *result);
void add(int z, int x, int y); //add: x + y = z
void sub(int z, int x, int y); //sub: x - y = z
void mul(int z, int x, int y); //mul: x * y = z
void div(int z, int x, int y); //div: x / y = z
void mod(int z, int x, int y); //mod: x % y = z
void mov(int z, int x, int y);
void JEQ(int x, int y, int L); //Jump if equal
void JLT(int x, int y, int L); //Jump if less than
void JGT(int x, int y, int L); //Jump if greater than
void JNE(int x, int y, int L); //Jump if not equal
void mvi(int RX, char* operand2);
void RET();
void JAL(int L);

void JMP(int L); //Jump to Label

int main(){

	string fileName;

	cout<<"Enter the fileName"<<endl;
	//cin>>fileName;

	fileName = "inputFile.txt";

	readFromFile(fileName);

	memoryDump();
	cout<< endl << "L1:: "<<L1<<", L2:: "<<L2<<", L3:: "<<L3 <<", JAL:: "<<jal << endl;
	//Start Program
	cout << "Starting Program" << endl;
	startMemLocation=1024;
	while (exitCodeCount != 8){
		operationType = 0;
		getCurrentInstruction();
		parseInstructionFromMemory();

		for (int i = 0; i < 8; i++) {
			//printf("%c", operation[i]);
			cout << operation[i];
		}

		cout << "Hello" << endl;

		if (exitCodeCount != 8){
			if (operationType == 1) { //ALU Type Instruction: Operation, Operand1, Operand2, Operand3
				decodeALUInstructionOperands();
				startMemLocation += 4;
			}
			else if (operationType == 2) { //MVI Instruction
				decodeMVIInstructionOperand();
				startMemLocation += 4;
			}
			else if (operationType == 3) { //JMP Instruction
				decodeJMPInstructionOperand();
			}
			else if (operationType == 4) { //DMP Instruction
				decodeDMPInstructionOperand();
				startMemLocation += 4;
			}
			else if (operationType == 5) { //RET Instruction
				decodeRETInstructionOperand();
				startMemLocation +=4;
			}
			else if (operationType == 6) { //JAL Instruction
				decodeJALInstructionOperand();
			}
			callAppropriateFunction();
		}
	}

	for (int i = 0; i < 32; i++){
		printf("%c", currentInstruction[i]);
	}
	cout<<"Program Finished"<<endl;

	memoryDump();

	cout<<"L1:: "<<L1<<", L2:: "<<L2<<", L3:: "<<L3 << endl;

	cout << "FINISHED" << endl;
}


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
int convertBinaryToDecimal(char *bits) {

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

string convertDecimalToBinary(int number){

    string bin;
    char holder=' ';
    while(number!=0)
    {
        holder=number%2+'0';
        bin=holder+bin;
        number/=2;
    }

    int len = bin.length();

    if(len!=8){
        while(len!=8){
            bin = "0" + bin;
            len++;
        }
    }

    return bin;
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
	if (memoryLocation <= 1024 || memoryLocation > 4092)
		validAddressFlag = 0;
}

void memoryDump() {
	int i = 0;
	/*
	printf("\n Flag Register: ");

	for(int j=0;j<8;j++){
		printf("%d",flags[j]);
	}
	*/

	printf("\n Flags: \n overflowFlag = %d, signFlag = %d, zeroFlag = %d, carryFlag = %d \n",
			overflowFlag, signFlag, zeroFlag, carryFlag);

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

	//for(int i = 1024; i<=2055; i++) {
	//for (int i = 1024; i <= 4096; i++) { //Memory Range of 1024 - 4096
	for (int i = 1024; i <=1104; i++) {
		printf(" Memory Location: %d: ",i);
		char vOut[9];
		for(int j=0;j<8;j++){
			vOut[j] = memory[i][j] ? '1' : '0';
		}
		vOut[8] = '\0';
		printf("%d\t\t\t",convertBinaryToDecimal(vOut));

		if (i%2 != 0)
			printf("\n");
	}

	cout<<endl << "L1:: "<<L1<<", L2:: "<<L2<<", L3:: "<<L3 <<", JAL:: "<<jal << endl;

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

			if(textFile.eof())
				break;
		}
		textFile.close();

	}
	else{
		cout<<"FILE DOES NOT EXIST"<<endl;
		cerr<<"The requested file"<<fileName<<" does not exist"<<endl;
	}
}

//Reads in Instruction from Text File
//"Decodes" Instruction into each opcode/operand.
//Converts operand into bits and stores into memory.
void decodeInstructionFromFile(string textLine){


    char* pch;

    pch = strtok((char*)textLine.c_str()," ,\r");
    while (pch != NULL)
    {
        if(strcmp(pch,"MFA")==0){
            cout<<"IN MFA"<<endl;
            pch = strtok (NULL, " ");
            cout<<"THE PCH TO PASS: "<<pch<<endl;
            pch = strtok (NULL, " ,\r");
        }else
            if(strcmp(pch,"MTA")==0){
                cout<<"IN MTA"<<endl;
                pch = strtok (NULL, " ");
                cout<<"THE PCH TO PASS: "<<pch<<endl;
                pch = strtok (NULL, " ,\r");
            }else
                if(strcmp(pch,"JMP")==0){
                    cout<<"IN JUMP"<<endl;
                    instructionIntoMemory(pch,startMemLocation);
                    startMemLocation++;
                    JMP_flag = true;
                    pch = strtok (NULL, " ,\r");
                }else
                    if(strcmp(pch,"MVI")==0){
                        cout<<"IN MOVE INS"<<endl;
                        instructionIntoMemory(pch,startMemLocation);
                        startMemLocation++;
                        MVI_flag = true;
                        pch = strtok (NULL, " ,\r");
                    }else
                        if(strcmp(pch,"JAL")==0){
                            cout<<"IN JAL INS"<<endl;
                            instructionIntoMemory(pch,startMemLocation);
                            startMemLocation++;
                            JAL_flag = true;
                            pch = strtok (NULL, " ,\r");
                        }else
                            if(strcmp(pch,"RET")==0){
                                cout<<"IN RET INS"<<endl;
                                instructionIntoMemory(pch,startMemLocation);
                                startMemLocation++;
                                RET_flag = true;
                                pch = strtok (NULL, " ,\r");
                            }else
                                if(strcmp(pch,"L1:")==0){
                                    cout<<"IN L1 TOKEN"<<endl;
                                    pch = strtok (NULL, " ,\r");
                                    L1=startMemLocation;
                                }else
                                    if(strcmp(pch,"L2:")==0){
                                        cout<<"IN L2 TOKEN"<<endl;
                                        pch = strtok (NULL, " ,\r");
                                        L2=startMemLocation;
                                    }else
                                        if(strcmp(pch,"L3:")==0){
                                            cout<<"IN L3 TOKEN"<<endl;
                                            pch = strtok (NULL, " ,\r");
                                            L3=startMemLocation;
                                        }else
                                            {
                                                cout<<"IN ELSE"<<endl;
                                                instructionIntoMemory(pch,startMemLocation);
                                                startMemLocation++;
                                                pch = strtok (NULL, " ,\r");
                                            }
    }

}

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
                    if(strcmp(token,"MVI")==0){
                        cout<<"IN MVI"<<endl;
                        for (int i = 0; i < 8; i++){
                            memory[memLocation][i] = MVI_Opcode[i];
                        }
                    }else
                        if(strcmp(token,"JMP")==0){
                            cout<<"IN JMP"<<endl;
                            for (int i = 0; i < 8; i++){
                                memory[memLocation][i] = JMP_Opcode[i];
                            }
                        }else
                            if(strcmp(token,"JGT")==0){
                                cout<<"IN JGT"<<endl;
                                for (int i = 0; i < 8; i++){
                                    memory[memLocation][i] = JGT_Opcode[i];
                                }
                            }else
                                if(strcmp(token,"JLT")==0){
                                    cout<<"IN JLT"<<endl;
                                    for (int i = 0; i < 8; i++){
                                        memory[memLocation][i] = JLT_Opcode[i];
                                    }
                                }else
                                    if(strcmp(token,"JEQ")==0){
                                        cout<<"IN JEQ"<<endl;
                                        for (int i = 0; i < 8; i++){
                                            memory[memLocation][i] = JEQ_Opcode[i];
                                        }
                                    }else
                                        if(strcmp(token,"JNE")==0){
                                            cout<<"IN JNE"<<endl;
                                            for (int i = 0; i < 8; i++){
                                                memory[memLocation][i] = JNE_Opcode[i];
                                            }
                                        }else
                                            if(strcmp(token,"JAL")==0){
                                                cout<<"IN JAL"<<endl;
                                                for (int i = 0; i < 8; i++){
                                                    memory[memLocation][i] = JAL_Opcode[i];
                                                }
                                            }else
                                                if(strcmp(token,"RET")==0){
                                                    cout<<"IN RET"<<endl;
                                                    for (int i = 0; i < 8; i++){
                                                        memory[memLocation][i] = RET_Opcode[i];
                                                    }
                                                    startMemLocation+=3;
                                                }else
                                                    if(strcmp(token,"L1")==0){
                                                        cout<<"IN L1"<<endl;
                                                        for (int i = 0; i < 8; i++){
                                                            memory[memLocation][i] = L1_Opcode[i];
                                                        }
                                                        if(JMP_flag || JAL_flag){
                                                            startMemLocation+=2;
                                                            JMP_flag = false;
                                                            if(JAL_flag){
                                                                jal = startMemLocation+1;
                                                                JAL_flag = false;
                                                            }
                                                        }
                                                    }else
                                                        if(strcmp(token,"L2")==0){
                                                            cout<<"IN L2"<<endl;
                                                            for (int i = 0; i < 8; i++){
                                                                memory[memLocation][i] = L2_Opcode[i];
                                                            }
                                                            if(JMP_flag || JAL_flag){
                                                                startMemLocation+=2;
                                                                JMP_flag = false;
                                                                if(JAL_flag){
                                                                    jal = startMemLocation+1;
                                                                    JAL_flag = false;
                                                                }
                                                            }
                                                        }else
                                                            if(strcmp(token,"L3")==0){
                                                                cout<<"IN L3"<<endl;
                                                                for (int i = 0; i < 8; i++){
                                                                    memory[memLocation][i] = L3_Opcode[i];
                                                                }
                                                                if(JMP_flag || JAL_flag){
                                                                    startMemLocation+=2;
                                                                    JMP_flag = false;
                                                                    if(JAL_flag){
                                                                        jal = startMemLocation+1;
                                                                        JAL_flag = false;
                                                                    }
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
                                                                                                                                cout<<"EXIT"<<endl;
                                                                                                                                for (int i = 0; i < 8; i++){
                                                                                                                                    memory[memLocation][i] = exit_Opcode[i];
                                                                                                                                }
                                                                                                                                startMemLocation+=3;
                                                                                                                            }else
                                                                                                                                if(MVI_flag){
                                                                                                                                    cout<<token<<endl;
                                                                                                                                    string num = convertDecimalToBinary(atoi(token));
                                                                                                                                    cout<<"STR:: "<<num<<endl;
                                                                                                                                    for (int i = 0; i < 8; i++){
                                                                                                                                        memory[memLocation][i] = num[i]=='1' ? 1 : 0;
                                                                                                                                    }
                                                                                                                                    startMemLocation++;
                                                                                                                                    MVI_flag = false;
                                                                                                                                }

}

/****************************************
 * MemoryAddressing
 */
int MemoryAddressing(int imm=0, int rdx=0, int rcx=0, int S=1){
	int address;
	address  = imm + rdx + rcx*S;
	return address;
}
/*************************************************************
 *
 *************************************************************/
int StringMemoryAddressing(string str){
	unsigned first = 0;
	unsigned last = str.find('(');
	string strNew = str.substr (first,last-first);
	long int int_value1 = std::strtol(strNew.c_str(), 0, 16);

	first = str.find('(');
	last = str.find(',');
	strNew = str.substr (first+1,last-first-1);
	//long int int_value2 = std::strtol(strNew.c_str(), 0, 16);
	//cout << strNew << endl;
	int int_value2 = StringRegistersToInt(strNew);
	//cout << "int_value2=" << int_value2 << endl;

	first = str.find(',',last);
	last = str.find(',',last+1);
	strNew = str.substr (first+1,last-first-1);
	//long int int_value3 = std::strtol(strNew.c_str(), 0, 16);
	//cout << strNew << endl;
	int int_value3 = StringRegistersToInt(strNew);
	//cout << "int_value3=" << int_value3 << endl;

	first = str.find(',',last);
	last = str.find(')',last+1);
	strNew = str.substr (first+1,last-first-1);
	long int int_value4 = std::strtol(strNew.c_str(), 0, 10);

	int addr = MemoryAddressing(int_value1,int_value2,int_value3,int_value4);
	return addr;
	//return 0;
}
/*************************************************************
 *
 *************************************************************/
int StringRegistersToInt(string strNew){
	char vOut[33];
	if(strcmp(strNew.c_str(),"R0")==0){
		for(int i = 0; i < 32; i++){
			vOut[i] = R0[i] ? '1' : '0';
		}
	}
	if(strcmp(strNew.c_str(),"R1")==0){
		for(int i = 0; i < 32; i++){
			vOut[i] = R1[i] ? '1' : '0';
		}
	}
	if(strcmp(strNew.c_str(),"R2")==0){
		for(int i = 0; i < 32; i++){
			vOut[i] = R2[i] ? '1' : '0';
		}
	}
	if(strcmp(strNew.c_str(),"R3")==0){
		for(int i = 0; i < 32; i++){
			vOut[i] = R3[i] ? '1' : '0';
		}
	}
	if(strcmp(strNew.c_str(),"R4")==0){
		for(int i = 0; i < 32; i++){
			vOut[i] = R4[i] ? '1' : '0';
		}
	}
	if(strcmp(strNew.c_str(),"R5")==0){
		for(int i = 0; i < 32; i++){
			vOut[i] = R5[i] ? '1' : '0';
		}
	}
	if(strcmp(strNew.c_str(),"R6")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R6[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R7")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R7[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R8")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R8[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R9")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R9[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R10")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R10[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R11")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R11[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R12")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R12[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R13")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R13[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R14")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R14[i] ? '1' : '0';
			}
		}
	if(strcmp(strNew.c_str(),"R15")==0){
			for(int i = 0; i < 32; i++){
				vOut[i] = R15[i] ? '1' : '0';
			}
		}

	int ret = strtol(vOut, NULL, 2);
	return ret;
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
	int addCount = 0, subCount = 0, mulCount = 0, divCount = 0,
		modCount = 0, ldaCount = 0, staCount = 0, movCount = 0,
		mviCount = 0, jmpCount = 0, jgtCount = 0, jltCount = 0,
		jneCount = 0, jeqCount = 0, dmpCount = 0, retCount = 0,
		jalCount = 0;

	char tmpADD_Opcode[8], tmpSUB_Opcode[8], tmpMUL_Opcode[8], tmpDIV_Opcode[8],
		 tmpMOD_Opcode[8], tmpLDA_Opcode[8], tmpSTA_Opcode[8], tmpMOV_Opcode[8],
		 tmpMVI_Opcode[8], tmpJMP_Opcode[8], tmpJGT_Opcode[8], tmpJLT_Opcode[8],
		 tmpJNE_Opcode[8], tmpJEQ_Opcode[8], tmpDMP_Opcode[8], tmpRET_Opcode[8],
		 tmpJAL_Opcode[8];

	//Convert Opcodes stored as Boolean array into Array of Chars.
	for (int i = 0; i < 8; i++){
		tmpADD_Opcode[i] = ADD_Opcode[i] ? '1' : '0';
		tmpSUB_Opcode[i] = SUB_Opcode[i] ? '1' : '0';
		tmpMUL_Opcode[i] = MUL_Opcode[i] ? '1' : '0';
		tmpDIV_Opcode[i] = DIV_Opcode[i] ? '1' : '0';
		tmpMOD_Opcode[i] = MOD_Opcode[i] ? '1' : '0';
		tmpLDA_Opcode[i] = LDA_Opcode[i] ? '1' : '0';
		tmpSTA_Opcode[i] = STA_Opcode[i] ? '1' : '0';
		tmpMOV_Opcode[i] = MOV_Opcode[i] ? '1' : '0';
		tmpMVI_Opcode[i] = MVI_Opcode[i] ? '1' : '0';
		tmpJMP_Opcode[i] = JMP_Opcode[i] ? '1' : '0';
		tmpJGT_Opcode[i] = JGT_Opcode[i] ? '1' : '0';
		tmpJLT_Opcode[i] = JLT_Opcode[i] ? '1' : '0';
		tmpJNE_Opcode[i] = JNE_Opcode[i] ? '1' : '0';
		tmpJEQ_Opcode[i] = JEQ_Opcode[i] ? '1' : '0';
		tmpDMP_Opcode[i] = DMP_Opcode[i] ? '1' : '0';
		tmpRET_Opcode[i] = RET_Opcode[i] ? '1' : '0';
		tmpJAL_Opcode[i] = JAL_Opcode[i] ? '1' : '0';
	}

	//read in opcode
	for (int i = 0; i < 8; i++)
		operation[i] = currentInstruction[i];

	//"decode" opcode. Compare with function opcodes.
	for (int i = 0; i < 8; i++) {
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
		if (operation[i]==tmpMVI_Opcode[i])
			mviCount++;
		if (operation[i]==tmpJMP_Opcode[i])
			jmpCount++;
		if (operation[i]==tmpJGT_Opcode[i])
			jgtCount++;
		if (operation[i]==tmpJLT_Opcode[i])
			jltCount++;
		if (operation[i]==tmpJNE_Opcode[i])
			jneCount++;
		if (operation[i]==tmpJEQ_Opcode[i])
			jeqCount++;
		if (operation[i]==tmpDMP_Opcode[i])
			dmpCount++;
		if (operation[i]==tmpRET_Opcode[i])
			retCount++;
		if (operation[i]==tmpJAL_Opcode[i])
			jalCount++;
	}

	//if ALU instruction, Format = Operation, Operand1, Operand2, Operand3
	if (addCount==8 || subCount==8 || mulCount==8 || divCount==8 ||
		modCount==8 || ldaCount==8 || staCount==8 || movCount==8 ||
		jgtCount==8 || jltCount==8 || jneCount==8 || jeqCount==8) {

		operationType = 1; //Set to ALU Type Instruction

		for (int i = 8; i < 32; i++) {
			if (i >= 8 && i <= 15)
				operand1[i-8] = currentInstruction[i];
			if (i >= 16 && i <= 23)
				operand2[i-16] = currentInstruction[i];
			if (i >= 24 && i <= 31)
				operand3[i-24] = currentInstruction[i];
		}
	}
	//MVI Instruction = Operation, Operand1 (8-bit)
	else if (mviCount==8){

		operationType = 2; //Set to MVI Type Instruction

		for (int i = 8; i < 24; i++) {
			if (i >= 8 && i <=15)
				operand1[i-8] = currentInstruction[i]; //Operand1 will have destination Register
			if (i >= 16 && i <= 23)
				operand2[i-16] = currentInstruction[i]; //Operand2 will have 8-bit immediate value. This will be an array of chars.
		}
	}
	//JMP Instruction = Operation, Operand1 (L1,L2,L3)
	else if (jmpCount==8){

		operationType = 3; //Set to JMP Instruction

		for (int i = 8; i < 16; i++) {
			operand1[i-8] = currentInstruction[i]; //Operand1 will have L1, L2, L3
		}
	}
	//DMP Instruction = Operation
	else if (dmpCount==8){

		operationType = 4; //Set to DMP Instruction

	}
	//RET Instruction
	else if (retCount==8){

		operationType = 5; //Set to RET Instruction

	}
	//JAL Instruction
	else if (jalCount==8){

		operationType = 6; //Set to JAL Instruction

		for (int i = 8; i < 16; i++) {
			operand1[i-8] = currentInstruction[i];
		}
	}

	exitCodeCount = 0; //Reset Exit Code Count in case it was incremented by other codes matching "part" of the exit code.

	for (int i = 0; i < 8; i++){
		if (operation[i] == tmpExit_Opcode[i])
			exitCodeCount++;
	}

	if (exitCodeCount==8)
		operationType=10101010;

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

	//startMemLocation += 4;

}
/*******************************************
 * This function changes the operation/opcode
 * into an integer value.
 *******************************************/
void decodeALUInstructionOperands(){
	int addCount = 0, subCount = 0, mulCount = 0, divCount = 0,
		modCount = 0, ldaCount = 0, staCount = 0, movCount = 0,
	    mviCount = 0, jmpCount = 0, jgtCount = 0, jltCount = 0,
		jneCount = 0, jeqCount = 0, dmpCount = 0;

	int operand1R0Count = 0, operand1R1Count = 0, operand1R2Count = 0, operand1R3Count = 0,
		operand1R4Count = 0, operand1R5Count = 0, operand1R6Count = 0, operand1R7Count = 0,
		operand1R8Count = 0, operand1R9Count = 0, operand1R10Count = 0, operand1R11Count = 0,
		operand1R12Count = 0, operand1R13Count = 0, operand1R14Count = 0, operand1R15Count = 0,
		operand1L1Count = 0, operand1L2Count = 0, operand1L3Count = 0;

	int operand2R0Count = 0, operand2R1Count = 0, operand2R2Count = 0, operand2R3Count = 0,
		operand2R4Count = 0, operand2R5Count = 0, operand2R6Count = 0, operand2R7Count = 0,
		operand2R8Count = 0, operand2R9Count = 0, operand2R10Count = 0, operand2R11Count = 0,
		operand2R12Count = 0, operand2R13Count = 0, operand2R14Count = 0, operand2R15Count = 0;

	int operand3R0Count = 0, operand3R1Count = 0, operand3R2Count = 0, operand3R3Count = 0,
		operand3R4Count = 0, operand3R5Count = 0, operand3R6Count = 0, operand3R7Count = 0,
		operand3R8Count = 0, operand3R9Count = 0, operand3R10Count = 0, operand3R11Count = 0,
		operand3R12Count = 0, operand3R13Count = 0, operand3R14Count = 0, operand3R15Count = 0,
		operand3L1Count = 0, operand3L2Count = 0, operand3L3Count = 0;

	char tmpADD_Opcode[8], tmpSUB_Opcode[8], tmpMUL_Opcode[8], tmpDIV_Opcode[8],
		 tmpMOD_Opcode[8], tmpLDA_Opcode[8], tmpSTA_Opcode[8], tmpMOV_Opcode[8],
		 tmpR0_Opcode[8], tmpR1_Opcode[8], tmpR2_Opcode[8], tmpR3_Opcode[8],
		 tmpR4_Opcode[8], tmpR5_Opcode[8], tmpR6_Opcode[8], tmpR7_Opcode[8],
		 tmpR8_Opcode[8], tmpR9_Opcode[8], tmpR10_Opcode[8], tmpR11_Opcode[8],
		 tmpR12_Opcode[8], tmpR13_Opcode[8], tmpR14_Opcode[8], tmpR15_Opcode[8],
	     tmpMVI_Opcode[8], tmpJMP_Opcode[8], tmpJGT_Opcode[8], tmpJLT_Opcode[8],
		 tmpJNE_Opcode[8], tmpJEQ_Opcode[8], tmpDMP_Opcode[8],
		 tmpL1_Opcode[8], tmpL2_Opcode[8], tmpL3_Opcode[8];

	for (int i = 0; i< 8; i++){
		tmpADD_Opcode[i] = ADD_Opcode[i] ? '1' : '0';
		tmpSUB_Opcode[i] = SUB_Opcode[i] ? '1' : '0';
		tmpMUL_Opcode[i] = MUL_Opcode[i] ? '1' : '0';
		tmpDIV_Opcode[i] = DIV_Opcode[i] ? '1' : '0';
		tmpMOD_Opcode[i] = MOD_Opcode[i] ? '1' : '0';
		tmpLDA_Opcode[i] = LDA_Opcode[i] ? '1' : '0';
		tmpSTA_Opcode[i] = STA_Opcode[i] ? '1' : '0';
		tmpMOV_Opcode[i] = MOV_Opcode[i] ? '1' : '0';
		tmpMVI_Opcode[i] = MVI_Opcode[i] ? '1' : '0';
		tmpJMP_Opcode[i] = JMP_Opcode[i] ? '1' : '0';
		tmpJGT_Opcode[i] = JGT_Opcode[i] ? '1' : '0';
		tmpJLT_Opcode[i] = JLT_Opcode[i] ? '1' : '0';
		tmpJNE_Opcode[i] = JNE_Opcode[i] ? '1' : '0';
		tmpJEQ_Opcode[i] = JEQ_Opcode[i] ? '1' : '0';
		tmpDMP_Opcode[i] = DMP_Opcode[i] ? '1' : '0';

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
		tmpL1_Opcode[i] = L1_Opcode[i] ? '1' : '0';
		tmpL2_Opcode[i] = L2_Opcode[i] ? '1' : '0';
		tmpL3_Opcode[i] = L3_Opcode[i] ? '1' : '0';
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
	    if (operation[i]==tmpMVI_Opcode[i])
			mviCount++;
		if (operation[i]==tmpJMP_Opcode[i])
			jmpCount++;
		if (operation[i]==tmpJGT_Opcode[i])
			jgtCount++;
		if (operation[i]==tmpJLT_Opcode[i])
			jltCount++;
		if (operation[i]==tmpJNE_Opcode[i])
			jneCount++;
		if (operation[i]==tmpJEQ_Opcode[i])
			jeqCount++;
		if (operation[i]==tmpDMP_Opcode[i])
			dmpCount++;

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
		if (operand1[i]==tmpL1_Opcode[i])
			operand1L1Count++;
		if (operand1[i]==tmpL2_Opcode[i])
			operand1L2Count++;
		if (operand1[i]==tmpL3_Opcode[i])
			operand1L3Count++;

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
	  	if (operand3[i]==tmpL1_Opcode[i])
	  		operand3L1Count++;
	  	if (operand3[i]==tmpL2_Opcode[i])
	  		operand3L2Count++;
	  	if (operand3[i]==tmpL3_Opcode[i])
	  		operand3L3Count++;
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
	else if (mviCount==8)
		instructionOperation=136;
	else if (jmpCount==8)
		instructionOperation=137;
	else if (jgtCount==8)
		instructionOperation=138;
	else if (jltCount==8)
		instructionOperation=139;
	else if (jneCount==8)
		instructionOperation=140;
	else if (jeqCount==8)
		instructionOperation=141;
	else if (dmpCount==8)
		instructionOperation=142;

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
	else if (operand1L1Count==8)
		instructionOperand1=L1;
	else if (operand1L2Count==8)
		instructionOperand1=L2;
	else if (operand1L3Count==8)
		instructionOperand1=L3;

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
	else if (operand3L1Count==8)
		instructionOperand3=L1;
	else if (operand3L2Count==8)
		instructionOperand3=L2;
	else if (operand3L3Count==8)
		instructionOperand3=L3;

	return;
}

/*******************************************
 * This function decodes the MVI Instruction
 * and its operands.
 * Input:
 * Output:
 */
void decodeMVIInstructionOperand(){
	int mviCount = 0;

	int operand1R0Count = 0, operand1R1Count = 0, operand1R2Count = 0, operand1R3Count = 0,
		operand1R4Count = 0, operand1R5Count = 0, operand1R6Count = 0, operand1R7Count = 0,
		operand1R8Count = 0, operand1R9Count = 0, operand1R10Count = 0, operand1R11Count = 0,
		operand1R12Count = 0, operand1R13Count = 0, operand1R14Count = 0,operand1R15Count = 0;

	char tmpMVI_Opcode[8],
		 tmpR0_Opcode[8], tmpR1_Opcode[8], tmpR2_Opcode[8], tmpR3_Opcode[8],
		 tmpR4_Opcode[8], tmpR5_Opcode[8], tmpR6_Opcode[8], tmpR7_Opcode[8],
		 tmpR8_Opcode[8], tmpR9_Opcode[8], tmpR10_Opcode[8], tmpR11_Opcode[8],
		 tmpR12_Opcode[8], tmpR13_Opcode[8], tmpR14_Opcode[8], tmpR15_Opcode[8];

	for (int i = 0; i< 8; i++) {
		tmpMVI_Opcode[i] = MVI_Opcode[i] ? '1' : '0';
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
		if (operation[i]==tmpMVI_Opcode[i])
			mviCount++;

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
	}

	if (mviCount==8)
		instructionOperation=136;

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

}
/*******************************************
 * This function decodes the JMP Instruction
 * and its operands.
 * Input:
 * Output:
 ******************************************/
void decodeJMPInstructionOperand(){
	int jmpCount = 0;

	int operand1L1Count = 0, operand1L2Count = 0, operand1L3Count = 0;

	char tmpJMP_Opcode[8], tmpL1_Opcode[8], tmpL2_Opcode[8], tmpL3_Opcode[8];

	for (int i = 0; i< 8; i++) {
		tmpJMP_Opcode[i] = JMP_Opcode[i] ? '1' : '0';
		tmpL1_Opcode[i] = L1_Opcode[i] ? '1' : '0';
		tmpL2_Opcode[i] = L2_Opcode[i] ? '1' : '0';
		tmpL3_Opcode[i] = L3_Opcode[i] ? '1' : '0';
	}

	for (int i = 0; i < 8; i++){
		if (operation[i]==tmpJMP_Opcode[i])
			jmpCount++;

		if (operand1[i]==tmpL1_Opcode[i])
			operand1L1Count++;
		if (operand1[i]==tmpL2_Opcode[i])
			operand1L2Count++;
		if (operand1[i]==tmpL3_Opcode[i])
			operand1L3Count++;
	}

	if (jmpCount==8)
		instructionOperation=137;

	if (operand1L1Count==8)
		instructionOperand1=L1;
	else if (operand1L2Count==8)
		instructionOperand1=L2;
	else if (operand1L3Count==8)
		instructionOperand1=L3;

	return;

}
/*******************************************
 * This function decodes the DMP Instruction
 * and its operands.
 * Input:
 * Output:
 *******************************************/
void decodeDMPInstructionOperand(){

}
/*******************************************
 * This function decodes the RET Instruction
 * and its operands.
 * Input:
 * Output:
 *******************************************/
void decodeRETInstructionOperand(){
	int retCount = 0;

	char tmpRET_Opcode[8];

	for (int i = 0; i< 8; i++)
		tmpRET_Opcode[i] = RET_Opcode[i] ? '1' : '0';

	for (int i = 0; i < 8; i++){
		if (operation[i]==tmpRET_Opcode[i])
			retCount++;
	}

	if (retCount==8)
		instructionOperation=143;

	return;

}
/*******************************************
 * This function decodes the JAL Instruction
 * and its operands.
 * Input:
 * Output:
 *******************************************/
void decodeJALInstructionOperand(){
	int jalCount = 0;

	int operand1L1Count = 0, operand1L2Count = 0, operand1L3Count = 0;

	char tmpJAL_Opcode[8], tmpL1_Opcode[8], tmpL2_Opcode[8], tmpL3_Opcode[8];

	for (int i = 0; i< 8; i++) {
		tmpJAL_Opcode[i] = JAL_Opcode[i] ? '1' : '0';
		tmpL1_Opcode[i] = L1_Opcode[i] ? '1' : '0';
		tmpL2_Opcode[i] = L2_Opcode[i] ? '1' : '0';
		tmpL3_Opcode[i] = L3_Opcode[i] ? '1' : '0';
	}

	for (int i = 0; i < 8; i++){
		if (operation[i]==tmpJAL_Opcode[i])
			jalCount++;

		if (operand1[i]==tmpL1_Opcode[i])
			operand1L1Count++;
		if (operand1[i]==tmpL2_Opcode[i])
			operand1L2Count++;
		if (operand1[i]==tmpL3_Opcode[i])
			operand1L3Count++;
	}

	if (jalCount==8)
		instructionOperation=144;

	if (operand1L1Count==8)
		instructionOperand1=L1;
	else if (operand1L2Count==8)
		instructionOperand1=L2;
	else if (operand1L3Count==8)
		instructionOperand1=L3;

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
	else if (instructionOperation==129)
		sub(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==130)
		mul(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==131)
		div(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==132)
		mod(instructionOperand1, instructionOperand2, instructionOperand3);
	/*
	else if (instructionOperation==133)
		lda();
	else if (instructionOperation==134)
		sta();
	*/
	else if (instructionOperation==135)
		mov(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==136)
		mvi(instructionOperand1, operand2);
	else if (instructionOperation==137)
		JMP (instructionOperand1);
	else if (instructionOperation==138)
		JGT(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==139)
		JLT(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==140)
		JNE(instructionOperand1, instructionOperand2, instructionOperand3);
	else if (instructionOperation==141)
		JEQ(instructionOperand1, instructionOperand2, instructionOperand3);
	/*
	else if (instructionOperation==142)
		dmp();
	 */
	else if (instructionOperation==143)
		RET();
	else if (instructionOperation==144)
		JAL(instructionOperand1);
	return;
}

/******************************************************************
 Functions for ALU Operation i.e. ADD, SUB, MUL, DIV, MOD, etc...
 ******************************************************************/
// Jump-and-Link function. This function will push the return address onto the stack and go to the  instruction specified by the label L
void JAL(int L){

    bool tempR1[32];
    //int tempStackPointer = startMemLocation+4;
    int tempStackPointer = jal;
    convertDecimalToBinary_N(tempStackPointer,tempR1);

    for(int i=0; i<32; i++){
        cout<<tempR1[i];
    }
    cout<<endl;

    for(int i=0; i<32; i++){
        if(i>=0 && i<=7)
            memory[stackPointer][i] = tempR1[i];
        if(i>=8 && i<=15)
            memory[stackPointer+1][i-8] = tempR1[i];
        if(i>=16 && i<=23)
            memory[stackPointer+2][i-16] = tempR1[i];
        if(i>=24 && i<=31)
            memory[stackPointer+3][i-24] = tempR1[i];
    }
    stackPointer = stackPointer+4;
    startMemLocation = L;
}

// Return Function Pops the top of the stack and takes control to the address stored on the top of the stack
void RET(){

    bool tempR1[32];
    for(int i=0; i<32; i++) {
        if(i>=0 && i<=7)
            tempR1[i] = memory[stackPointer-4][i];
        if(i>=8 && i<=15)
            tempR1[i] = memory[stackPointer-3][i-8];
        if(i>=16 && i<=23)
            tempR1[i] = memory[stackPointer-2][i-16];
        if(i>=24 && i<=31)
            tempR1[i] = memory[stackPointer-1][i-24];
    }

    int returnAddress = convertBinaryToDecimal_N(tempR1,32);
    stackPointer -= 4;
    startMemLocation = returnAddress;
}

void mvi(int x, char* temp){
	bool tempR1[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, tempR2[32], tempA[32];

	for(int i=0; i<8; i++){
		if(temp[i]=='0')
			tempR1[i+24] = 0;
		else
			tempR1[i+24] = 1;
	}


//	function to decide which register is defined by x
	findDestinationRegister(x, tempR1);
}
//Jump to label address L
void JMP (int L){
	startMemLocation = L;
}
// Jump if equal function - Takes three parameters as input i.e., two register numbers (x,y) and boolean array pointer.
// If content of register x is equal to content of register y then stores the array L content into program counter register
void JEQ(int x, int y, int L){ //Jump if equal

	sub(0,x,y);
	unsigned int n = convertBinaryToDecimal_N(R0,32);
	if(n == 0){
		startMemLocation = L;
	}
}

// Jump if less than function - Takes three parameters as input i.e., two register numbers (x,y) and boolean array pointer.
// If content of register x is less than the content of register y then stores the array L content into program counter register
void JLT(int x, int y, int L){ //Jump if less than

	sub(0,y,x);
	unsigned int n = convertBinaryToDecimal_N(R0,32);
	if((n > 0) && (overflowFlag==0)){
		startMemLocation = L;
	}
}

// Jump if greater than function - Takes three parameters as input i.e., two register numbers (x,y) and boolean array pointer.
// If content of register x is greater than the content of register y then stores the array L content into program counter register
void JGT(int x, int y, int L){ //Jump if greater than

	sub(0,x,y);
	unsigned int n = convertBinaryToDecimal_N(R0,32);
	if((n > 0) && (overflowFlag==0)){
		startMemLocation = L;
	}
}

// Jump if not equal function - Takes three parameters as input i.e., two register numbers (x,y) and boolean array pointer.
// If content of register x is greater than the content of register y then stores the array L content into program counter register
void JNE(int x, int y, int L){ //Jump if not equal

	sub(0,x,y);
	unsigned int n = convertBinaryToDecimal_N(R0,32);
	if(n != 0) {
		startMemLocation = L;
	}
}

// Move function - Takes three register numbers as input (z,x,y) and moves content of 'x' register and to 'y' register
void mov(int z, int x, int y){
	bool tempR1[32], tempR2[32], tempA[32];
//	function to decide which register is defined by x
	findSourceRegister(x,tempR1);

	for(int i=0; i<32; i++){
		tempR2[i] = tempR1[i];
	}

//function to decide which register is defined by z
	findDestinationRegister(z, tempR2);

}

// Modular function - Takes three register numbers as input (z,x,y) and take Modulos between 'x' register and 'y' register and stores the result in 'z' register
void mod(int z, int x, int y){

	int i=32, rim, q=0, divisor, dividend;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA[32], tempA1[32], tempA2[32], finalResult[32];

//	function to decide which register is defined by x
	findSourceRegister(x,tempR1);
//	function to decide which register is defined by y
	findSourceRegister(y,tempR2);

	dividend = convertBinaryToDecimal_N(tempR1,32);
	int n = dividend;
	divisor = convertBinaryToDecimal_N(tempR2,32);
	int m = divisor;
	bool tempRX[32], tempRY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};

//	Calculating 2's compliment
	for(int i=0; i<32; i++){
		tempRX[i] = !tempR2[i];
	}

	for(int i=31; i>=0; i--){
		sum = (tempRX[i] ^ tempRY[i]) ^ carry;
		temp1 = tempRX[i] & tempRY[i];
		temp2 = (tempRX[i] ^ tempRY[i]) & carry;
		carry = temp1 | temp2;
		tempA1[i] = sum;
	}

//	Division loop
	carry=0; temp1=0; temp2=0; sum=0;
	while(dividend >= divisor){

		carry=0; temp1=0; temp2=0; sum=0;
		for(int i=31; i>=0; i--){
			sum = (tempR1[i] ^ tempA1[i]) ^ carry;
			temp1 = tempR1[i] & tempA1[i];
			temp2 = (tempR1[i] ^ tempA1[i]) & carry;
			carry = temp1 | temp2;
			tempA2[i] = sum;
		}

		q++;

		for(int k=0; k<32; k++){
			tempR1[k]=tempA2[k];
		}

		dividend = 	convertBinaryToDecimal_N(tempA2,32);

	}

	convertDecimalToBinary_N(dividend,finalResult);

//	function to decide which register is defined by z
	findDestinationRegister(z, finalResult);

//	Updating flags
	if(dividend == 0) {zeroFlag = 1; overflowFlag = 0; signFlag = 0; carryFlag = 0;}

}

// Divition function - Takes three register numbers as input (z,x,y) and divides 'x' register and 'y' register and stores the result in 'z' register
void div(int z, int x, int y){

	int i=32, rim, q=0, divisor, dividend;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA[32], tempA1[32], tempA2[32], finalResult[32];

//	function to decide which register is defined by x
	findSourceRegister(x,tempR1);
//	funtion to decide which register is defined by y
	findSourceRegister(y,tempR2);

	dividend = convertBinaryToDecimal_N(tempR1,32);
	int n = dividend;
	divisor = convertBinaryToDecimal_N(tempR2,32);
	int m = divisor;
	bool tempRX[32], tempRY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};

//	Calculating 2's compliment
	for(int i=0; i<32; i++){
		tempRX[i] = !tempR2[i];
	}

	for(int i=31; i>=0; i--){
		sum = (tempRX[i] ^ tempRY[i]) ^ carry;
		temp1 = tempRX[i] & tempRY[i];
		temp2 = (tempRX[i] ^ tempRY[i]) & carry;
		carry = temp1 | temp2;
		tempA1[i] = sum;
	}

//	Division loop
	carry=0; temp1=0; temp2=0; sum=0;
	while(dividend >= divisor){

		carry=0; temp1=0; temp2=0; sum=0;
		for(int i=31; i>=0; i--){
			sum = (tempR1[i] ^ tempA1[i]) ^ carry;
			temp1 = tempR1[i] & tempA1[i];
			temp2 = (tempR1[i] ^ tempA1[i]) & carry;
			carry = temp1 | temp2;
			tempA2[i] = sum;
		}

		q++;

		for(int k=0; k<32; k++){
			tempR1[k]=tempA2[k];
		}

		dividend = 	convertBinaryToDecimal_N(tempA2,32);

	}

	convertDecimalToBinary_N(q,finalResult);
// function to decide which register is defined by z
	findDestinationRegister(z, finalResult);

//	Updating flags
	if(q == 0) {zeroFlag = 1; carryFlag = 0; overflowFlag = 0; signFlag = 0;}
}

// Multiply function - Takes three register numbers as input (z,x,y) and multiply 'x' register and 'y' register and stores the result in 'z' register
void mul(int z, int x, int y){

	int i;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempR11[64], tempR22[64], tempA[32], tempAA[64];

//	function to decide which register is defined by x
	findSourceRegister(x,tempR1);
//	function to decide which register is defined by y
	findSourceRegister(y,tempR2);

	int n = convertBinaryToDecimal_N(tempR2,32);

//	Debugging
	int n1 = convertBinaryToDecimal_N(tempR1, 32);
	int n2 = convertBinaryToDecimal_N(tempR2, 32);

//	Assigning 32 array to 64 bit array by padding 0 at the 32 MSBs for multiplication
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

	bool temp[32];
	for(int i=0; i<32; i++){
		temp[i]=tempAA[i+32];
	}
// function to decide which register is defined by z
	findDestinationRegister(z, temp);

// flag updating
	for(int i=0; i<32; i++) {if(tempAA[i] == 1){ overflowFlag = 1; carryFlag = 0; signFlag = 0; zeroFlag = 0; printf("There is a overflow after MUL operation !!!");break;} }
	if(convertBinaryToDecimal_N(temp,32) == 0){ zeroFlag = 1;}
}

// Addition function - Takes three register numbers as input (z,x,y) and addition 'x' register and 'y' register and stores the result in 'z' register
void add(int z, int x, int y) {

	int i = 31;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA[32];

//	function to decide which register is defined by x
	findSourceRegister(x,tempR1);

//	funtion to decide which register is defined by y
	findSourceRegister(y,tempR2);

	while(i>=0){
		sum = (tempR1[i]^tempR2[i]) ^ carry;
		temp1 = tempR1[i] & tempR2[i];
		temp2 = (tempR1[i]^tempR2[i]) & carry;
		carry = temp1 | temp2;
		tempA[i] = sum;
		i--;
	}

// function to decide which register is defined by z
	findDestinationRegister(z, tempA);

// flag updating
	if(carry == 1){carryFlag = 1; overflowFlag = 1; zeroFlag = 0; signFlag = 0; printf("There is a overflow and carry after ADD operation !!!");}
}

// Substract function - Takes three register numbers as input (z,x,y) and substract 'x' register and 'y' register and stores the result in 'z' register
void sub(int z, int x, int y){

	int i = 31;
	bool carry=0, temp1=0, temp2=0, sum=0;
	bool tempR1[32], tempR2[32], tempA1[32], tempA2[32];

//	function to decide which register is defined by x
	findSourceRegister(x,tempR1);
//	funtion to decide which register is defined by y
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

// function to decide which register is defined by z
	findDestinationRegister(z, tempA2);

// flag updating
	if(convertBinaryToDecimal_N(tempR2,32) > convertBinaryToDecimal_N(tempR1,32)) {
		signFlag = 1; overflowFlag = 1; carryFlag = 0; zeroFlag = 0;
		printf("There is a overflow after SUB operation !!!");
	}
	if(convertBinaryToDecimal_N(tempR2,32) == convertBinaryToDecimal_N(tempR1,32)) { zeroFlag = 1; signFlag = 0; overflowFlag = 1; carryFlag = 0;}

}

/******************************************************************
 Supporting functions for ALU Operation
 ******************************************************************/

// Converts hexadecimal input to binary

void convertDecimalToBinary_N(int dec, bool *result){

	bool bit; bool temp[32];

	for(int i=0; i<32; i++){
		result[i] = 0;
		temp[i] = 0;
	}

	int x=dec, count=0;
	while(x != 0) {
		if(bool(x%2) == 1)
			temp[count] = 1;
		else
			temp[count] = 0;
		count++;
		x=x/2;
	}

	for(int i=0, j=31; i<32; i++,j--){
		result[j]=temp[i];
	}

}


// Converts binary input to decimal

int convertBinaryToDecimal_N(bool *bits, int loc){

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

//	Switch case to deside which register is defined by x
	switch(x){

		case 0:
			for(int i=0; i<32; i++){
				tempR1[i] = R0[i];
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

//	Switch case to deside which register is defined by z
	switch(z){

		case 0:
			for(int i=0; i<32; i++){
				R0[i] = tempA[i];
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
