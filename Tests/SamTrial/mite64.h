#ifndef MITE64_H_
#define MITE64_H_

typedef unsigned long int u4;
 
// obtain a MITE pseudo-random value in [0..2**32]
u4  mite64_Random(void);
// seed MITE with a 2048-bit block of 4-byte words (Bob Jenkins method) 
void mite64_SeedW(char *seed, int rounds);
// reset/initialize MITE
void mite64_Reset(void);
// MITE64 # of bits internal state
u4 mite64_StateBits(void);
// MITE64 expected cycle length
u4 mite64_Cycle(void);
// MITE64 Name
char* mite64_Name(void);

#endif
