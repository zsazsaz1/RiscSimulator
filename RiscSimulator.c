#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <inttypes.h>
#include "singltons.h";
#include "opcodes.h";


int readFileAsHex(char filename[], int out_array[], int size)
{
	int instruction;
	FILE* instructionFile = fopen(filename, "r");

	if (NULL == instructionFile) {
		printf("Error! opening file\n");

		return -1;
	}

	int i = 0;
	int scanned;
	do {

		scanned = fscanf(instructionFile, "%X", &instruction);
		if (scanned == EOF) {
			continue;
		}
		out_array[i] = instruction;

		i++;
	} while (scanned != EOF);

	while (i < INSTRUCTION_RAM_SIZE) {
		out_array[i] = 0;
		i++;
	}

	fclose(instructionFile);

	return 0;
}

int main(int argc, char* argv[])
{
	readFileAsHex(argv[1], InstructionRam, INSTRUCTION_RAM_SIZE);
	readFileAsHex(argv[2], Ram, RAM_SIZE);

	int32_t a = -((0x80001 ^ 0xFFFFF) + 1);

	while (1) {
		int32_t currentInstruction = InstructionRam[PC];
		int8_t currentOpCode = (currentInstruction & 0xFF000) >> 12;
		int8_t rd = (currentInstruction & 0xF00) >> 8;
		int8_t rs = (currentInstruction & 0xF0) >> 4;
		int8_t rt = (currentInstruction & 0xF) ;
		OpcodeMap[currentOpCode](rd, rs, rt);
	}

	return 0;
}