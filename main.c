#include <stdio.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;


typedef struct SCpu {
	WORD PC;
	BYTE SP;
	
	BYTE A, X, Y;

	BYTE C;
	BYTE Z;
	BYTE I;
	BYTE D;
	BYTE B;
	BYTE V;
	BYTE N;
} CPU;

typedef unsigned int u32;
#define MAX_MEM (1024 * 64)

typedef struct SMem {
	BYTE Data[MAX_MEM];	
} MEM;

void initialise(MEM *mem)
{
	for (u32 i = 0; i < MAX_MEM; i++){
		mem->Data[i] = 0;
	}
}

void reset(CPU *cpu, MEM *mem)
{
	cpu->PC = 0xFFFC;
	cpu->SP = 0xFF;

	cpu->A = 0;
	cpu->X = 0;
	cpu->Y = 0;

	cpu->C = 0;
	cpu->Z = 0;
	cpu->I = 0;
	cpu->D = 0;
	cpu->B = 0;
	cpu->V = 0;
	cpu->N = 0;
	
	initialise(mem);
}

BYTE fetch(CPU *cpu, MEM *mem)
{
	BYTE data = mem->Data[cpu->PC];
	cpu->PC++;
	return data;
}

static const BYTE INS_LDA_IM = 0xA9;

BYTE mem_operator(MEM *mem, u32 address, u32 value)
{
	mem->Data[address] = value;
	return mem->Data[address];
}

void execute(CPU *cpu, MEM *mem, u32 cycles)
{
	while(cycles > 0){
		BYTE Instruction = fetch(cpu, mem);
		cycles--;

		switch(Instruction){
			case INS_LDA_IM: {
				BYTE value = fetch(cpu, mem);
				cpu->A = value;
				cpu->Z = (cpu->A==0);
				cpu->N = (cpu->A & 0b10000000) > 0;

				cycles--;
			} break;
			default:
				cycles = 0;
				break;
		}
	}
}

int main(void)
{
	CPU cpu;
	MEM mem;
	reset(&cpu, &mem);

	printf("The CPU PC is now: 0x%X\n", cpu.PC);

	// Begin
	mem_operator(&mem, 0xFFFC, INS_LDA_IM);
	mem_operator(&mem, 0xFFFD, 0x42);
	// End
	execute(&cpu, &mem, 2);

	printf("The CPU PC ends at: 0x%X\n", cpu.PC);
	printf("The value held is now: 0x%X\n", mem.Data[0xFFFD]);

	return 0;
}
