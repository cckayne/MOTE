// m: Check output from the MOTE8 CSPRNG 
#include "mote8.h"
#include <stdio.h>
#include <stddef.h>
//#define STREAM

int main(int argc, char *argv[]) {
	int i;
	u4 seed=0xFFFFFFFF;
	// pull a numeric seed from the command line
	if (argc>=2) seed=atoi(argv[1]);
	// print some information
	printf("CSPRNG        : %s\nInternal state: %u bits\nMinimum cycle : 2**%u\nMaximum cycle : 2**%u\nAverage cycle : 2**%u\n",
		mote8_Name(),mote8_StateBits(),mote8_Cycle(),mote8_StateBits(),mote8_StateBits()-2);
	// seed MOTE and mix
	testinit(seed);
	for (i=0;i<56;i++) mote8_Random();
	#ifdef STREAM
	// show some MOD 26 stream output
	for (i=0; i<672; i++) printf("%c",mote8_Random() % 26 + 'A');
	printf("\n");
	#endif
	return 0;
}
