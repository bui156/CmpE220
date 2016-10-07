// Please save this as ".cpp" file

# include <conio.h>
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
bool accumulator [32] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1};
bool memoryAddressRegister [32];
bool memoryDataRegister [32];
bool programCounterRegister [32];
int startMemLocation = 1024;

//Opcodes for Registers
bool R0_Opcode [8] = {0,0,0,0,0,0,0,1};
bool R1_Opcode [8] = {0,0,0,0,0,0,1,0};
bool R2_Opcode [8] = {0,0,0,0,0,0,1,1};
bool R3_Opcode [8] = {0,0,0,0,0,1,0,0};
bool R4_Opcode [8] = {0,0,0,0,0,1,0,1};
bool R5_Opcode [8] = {0,0,0,0,0,1,1,0};
bool R6_Opcode [8] = {0,0,0,0,0,1,1,1};
bool R7_Opcode [8] = {0,0,0,0,1,0,0,0};
bool R8_Opcode [8] = {0,0,0,0,1,0,0,1};
bool R9_Opcode [8] = {0,0,0,0,1,0,1,0};
bool R10_Opcode [8] = {0,0,0,0,1,0,1,1};
bool R11_Opcode [8] = {0,0,0,0,1,1,0,0};
bool R12_Opcode [8] = {0,0,0,0,1,1,0,1};
bool R13_Opcode [8] = {0,0,0,0,1,1,1,0};
bool R14_Opcode [8] = {0,0,0,0,1,1,1,1};
bool R15_Opcode [8] = {0,0,0,1,0,0,0,0};

//Opcodes for ALU Operations
bool ADD_Opcode [8] = {1,0,0,0,0,0,0,0};
bool SUB_Opcode [8] = {1,0,0,0,0,0,0,1};
bool MUL_Opcode [8] = {1,0,0,0,0,0,1,0};
bool DIV_Opcode [8] = {1,0,0,0,0,0,1,1};

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
void validDataMemoryAddressCheck(int memoryLocation);
void memoryDump();
void readFromFile(string fileName);
void decodeInstruction(string textLine);
void instructionIntoMemory(char* token,int memLocation);

int main(){

	string fileName;

	cout<<"Enter the fileName"<<endl;
	//cin>>fileName;

	fileName = "inputFile.txt";

	readFromFile(fileName);

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

	for(int i = 1024; i<=2055; i++) {
		printf(" Memory Location: %X: ",i);
		char vOut[9];
		for(int j=0;j<8;j++){
			vOut[j] = memory[i][j] ? '1' : '0';
		}
		vOut[8] = '\0';
		printf("%d\t\t\t",convertBinaryTodecimal(vOut));

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

			decodeInstruction(textLine);

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

void decodeInstruction(string textLine){

	char* pch;

	pch = strtok((char*)textLine.c_str()," ,");

	while (pch != NULL)
	{
		instructionIntoMemory(pch,startMemLocation);
		startMemLocation++;
		pch = strtok (NULL, " ,");
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
																				}
}

