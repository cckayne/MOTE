// m: Check output from the MOTE8 CSPRNG 
#include "mote8.h"
#include <stdio.h>
#include <stddef.h>
// mixing rounds: 7 recommended minimum
#define ROUNDS 7
//#define STREAM

int main(int argc, char *argv[]) {
	int i;
	char seed[512]="Bacon";
	// pull a seed from the command line
	if (argc>=2) strcpy(seed,argv[1]);
	// print some information
	printf("CSPRNG        : %s\nKey length    : %u bits\nInternal state: %u bits\nMinimum cycle : 2**%u\nMaximum cycle : 2**%u\nAverage cycle : 2**%u\n",
		mote8_Name(),mote8_KeyLength(),mote8_StateBits(),mote8_Cycle(),mote8_StateBits(),mote8_StateBits()-2);
	// seed MOTE and mix
	mote8_SeedW(seed,8*ROUNDS);
	#ifdef STREAM
	// show some MOD 26 stream output
	for (i=0; i<672; i++) printf("%c",mote8_Random() % 26 + 'A');
	printf("\n");
	#endif
	return 0;
}
