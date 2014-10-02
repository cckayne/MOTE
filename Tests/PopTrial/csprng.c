/* Universal wrapper unit for CSPRNGs. Copyright C.C.Kayne 2014, GNU GPL V.3. */
#include <stdio.h>
#include <stddef.h>
#include "cktype.h"
#include "ISAAC.h"
#include "BB128.h"
#include "BB256.h"
#include "BB512.h"
#include "mite8.h"
#include "mite16.h"
#include "mite32.h"
#include "mite64.h"
#include "mote8.h"
#include "mote16.h"
#include "mote32.h"
#include "csprng.h"


// obtain the size of the CSPRNG's internal state array
ub4 rStateSize(enum CSPRNG rng) {
	if (rng==ISAAC)  return 256; else
	if (rng==BB128)  return 128; else
	if (rng==BB256)  return 256; else
	if (rng==BB512)  return 512; else
	if (rng==MITE8)  return   8; else
	if (rng==MITE16) return  16; else
	if (rng==MITE32) return  32; else
	if (rng==MITE64) return  64; else
	if (rng==MOTE8)  return   8; else
	if (rng==MOTE16) return  16; else
	if (rng==MOTE32) return  32; else
	return 0;
}


// Initialize and seed a given CSPRNG
void rSeed(enum CSPRNG rng, char* seed, ub4 rounds) {
	if (rng==ISAAC)  ISAAC_SeedW(seed,TRUE);
	if (rng==BB128)  bb128_SeedW(seed,rounds);
	if (rng==BB256)  bb256_SeedW(seed,rounds);
	if (rng==BB512)  bb512_SeedW(seed,rounds);
	if (rng==MITE8)  mite8_SeedW(seed,rounds);
	if (rng==MITE16) mite16_SeedW(seed,rounds);
	if (rng==MITE32) mite32_SeedW(seed,rounds);
	if (rng==MITE64) mite64_SeedW(seed,rounds);
	if (rng==MOTE8)  mote8_SeedW(seed,rounds);
	if (rng==MOTE16) mote16_SeedW(seed,rounds);
	if (rng==MOTE32) mote32_SeedW(seed,rounds);
}

// Reset/initialize a given CSPRNG
void rReset(enum CSPRNG rng) {
	if (rng==ISAAC) ISAAC_Reset();
	if (rng==BB128) bb128_Reset();
	if (rng==BB256) bb256_Reset();
	if (rng==BB512) bb512_Reset();
	if (rng==MITE8)  mite8_Reset();
	if (rng==MITE16) mite16_Reset();
	if (rng==MITE32) mite32_Reset();
	if (rng==MITE64) mite64_Reset();
	if (rng==MOTE8)  mote8_Reset();
	if (rng==MOTE16) mote16_Reset();
	if (rng==MOTE32) mote32_Reset();
}

// Initialize and seed available CSPRNGs
void rSeedAll(char* seed, ub4 rounds) {
	ISAAC_SeedW(seed,TRUE);
	bb128_SeedW(seed,rounds);
	bb256_SeedW(seed,rounds);
	bb512_SeedW(seed,rounds);
	mite8_SeedW(seed,rounds);
	mite16_SeedW(seed,rounds);
	mite32_SeedW(seed,rounds);
	mite64_SeedW(seed,rounds);
	mote8_SeedW(seed,rounds);
	mote16_SeedW(seed,rounds);
	mote32_SeedW(seed,rounds);
}

// Reset/initialize available CSPRNGs
void rResetAll(void) {
	ISAAC_Reset();
	bb128_Reset();
	bb256_Reset();
	bb512_Reset();
	mite8_Reset();
	mite16_Reset();
	mite32_Reset();
	mite64_Reset();
	mote8_Reset();
	mote16_Reset();
	mote32_Reset();
}

// obtain a pseudo-random unsigned 32-bit quantity
//  from an available CSPRNG 
ub4 rRandom(enum CSPRNG rng) {
	if (rng==MOTE8)  return mote8_Random();  else
	if (rng==MOTE16) return mote16_Random(); else
	if (rng==MOTE32) return mote32_Random(); else
	if (rng==BB512)  return bb512_Random();  else
	if (rng==BB128)  return bb128_Random();  else
	if (rng==BB256)  return bb256_Random();  else
	if (rng==ISAAC)  return ISAAC_Random();  else
	if (rng==MITE8)  return mite8_Random();  else
	if (rng==MITE16) return mite16_Random(); else
	if (rng==MITE32) return mite32_Random(); else
	if (rng==MITE64) return mite64_Random(); else
	return 0;
}
