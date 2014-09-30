#ifndef MITE16_H_
#define MITE16_H_

typedef unsigned long int u4;
 
// obtain a MITE pseudo-random value in [0..2**32]
u4  mite16_Random(void);
// seed MITE with a 512-bit block of 4-byte words (Bob Jenkins method) 
void mite16_SeedW(char *seed, int rounds);
// reset/initialize MITE
void mite16_Reset(void);
// MITE16 # of bits internal state
u4 mite16_StateBits(void);
// MITE16 expected cycle length
u4 mite16_Cycle(void);
// MITE16 Name
char* mite16_Name(void);

#endif
