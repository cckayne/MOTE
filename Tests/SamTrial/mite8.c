#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "mite8.h"

// MITE defines
//#define TEST
//#define VERBOSE
#define NAME "MITE8"
#define STSZ 8
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 160+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
#define FLEASEED 0xf1ea5eed
#define rot(x,k) ((x<<k)|(x>>(32-k)))

static u4 rcnt=0;
static u4 state[STSZ], rsl[STSZ], a, b, c, d, e;

// MITE8 ROT switcher
typedef struct Rsw { u4 iii; u4 jjj; u4 kkk; };
static u4 ri=0;
static const struct Rsw rsw[4] = {
 6,  25, 14, // avalanche: 21.0 bits (worst case)
18,  25, 14, // avalanche: 21.0 bits (worst case)
26,   9, 10, // avalanche: 21.0 bits (worst case)
28,  29, 28  // avalanche: 21.0 bits (worst case)
};

#ifdef TEST
static void statepeek(void);
#endif


// MITE8 is refilled every 8 rounds
static void mite8(void) {
	register u4 i;
	for (i=0; i<STSZ; i++) {
		state[c & STM1] = b + d;
		a = b - c;
		b = c ^ (rot(e,rsw[ri].iii) | rot(a,rsw[ri].jjj));
		c = d - rot(b,rsw[ri].kkk);
		d = state[c & STM1] + b;
		e = d + c;
		rsl[i] = e;
	}
	#ifdef TEST
	statepeek();
	#endif
	ri = a & 3;
}


// obtain a MITE pseudo-random value in [0..2**32]
u4 mite8_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		mite8();
		rcnt = 0;
	}
	return r;
}


void mite8_Reset(void) {
	register u4 i,r;
	rcnt = 0;
	a = b = c = d = e = FLEASEED;
	for (i=0; i<STSZ; i++) { state[i]=GOLDEN; rsl[i]=0; }
}


// mix in all the key and state bytes
static void mix(void) {
	register u4 i;
	for (i=0; i<STSZ; i++) {
		a = b - c;
		b = c ^ (rot(e,rsw[ri].iii) | rot(a,rsw[ri].jjj));
		c = d - rot(b,rsw[ri].kkk);
		d = state[i] + b;
		e = d + c;
		state[i] = e;
	}
}

// seed MITE with a 256-bit block of 4-byte words (Bob Jenkins method) 
void mite8_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	strcpy(s,seed);
	mite8_Reset();
	memcpy((char *)state, (char *)s, l);
	mix();
	mite8();
	for (i=0; i<rounds; i++) mite8_Random();  
}


// MITE8 # of bits internal state
u4 mite8_StateBits(void) {
	return STBITS;
}


// MITE8 expected cycle length
u4 mite8_Cycle(void) {
	return (STBITS+1)/2;
}


// MITE8 Name
char* mite8_Name(void) {
	return NAME;
}


#ifdef TEST
static u4 bcnt=0;
static void statepeek(void) {
	register u4 i, gcnt=0;
	++bcnt;
	printf("%3u) mite8 using rsw[%1u]...\n",bcnt,ri);
	for (i=0; i<STSZ; i++) {
		#ifdef VERBOSE
		printf("rsl %3u: %11u %c %02X  | state %3u: %11u %c %02X\n",
			i,rsl[i],rsl[i] % 26 + 'A',rsl[i] & 255,
			i,state[i],state[i] % 26 + 'A',state[i] & 255);
		#endif
		if (state[i]==GOLDEN) ++gcnt;
	}
	printf("     a = %11u %c %02X\n     b = %11u %c %02X\n     c = %11u %c %02X\n     d = %11u %c %02X\n     e = %11u %c %02X\n",
		a,a % 26+'A',a & 255,b,b % 26+'A',b & 255,c,c % 26+'A',c & 255, d,d % 26+'A',d & 255,e,e % 26+'A',e & 255);
	printf("     %u GOLDEN\n",gcnt);
}
#endif
