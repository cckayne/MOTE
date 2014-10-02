#ifndef MITE32_H_
#define MITE32_H_

typedef unsigned long int u4;
 
// obtain a MITE pseudo-random value in [0..2**32]
u4  mite32_Random(void);
// seed MITE with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void mite32_SeedW(char *seed, int rounds);
// reset/initialize MITE
void mite32_Reset(void);
// MITE32 # of bits internal state
u4 mite32_StateBits(void);
// MITE32 expected cycle length
u4 mite32_Cycle(void);
// MITE32 Name
char* mite32_Name(void);

#endif
