/*
	This function writes values from memory to accumulator. 
	We check in main whether or not LDA will read a valid memory sector. 
	
	Input: int value for finalMemoryLocation
	Output: none (writing to a global array)
*/
void LDA (int finalMemoryLocation){
	
	//Hardcoding value of 32. accumulator is 32 bits. 
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
	
	return ;
}
