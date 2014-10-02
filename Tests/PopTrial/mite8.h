#ifndef MITE8_H_
#define MITE8_H_

typedef unsigned long int u4;
 
// obtain a MITE pseudo-random value in [0..2**32]
u4  mite8_Random(void);
// seed MITE with a 256-bit block of 4-byte words (Bob Jenkins method) 
void mite8_SeedW(char *seed, int rounds);
// reset/initialize MITE
void mite8_Reset(void);
// MITE8 # of bits internal state
u4 mite8_StateBits(void);
// MITE8 expected cycle length
u4 mite8_Cycle(void);
// MITE8 Name
char* mite8_Name(void);

#endif
