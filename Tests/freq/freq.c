/*
 * Measure whether all values appear with equal frequency
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef  unsigned char      u1;
typedef  unsigned long      u4;
typedef  unsigned long long u8;

// freq globals
u4 loglen = 0;
u4 terms  = 5;

#define BUCKETS (1<<8)

// MOTE defines
//#define VERBOSE
#define TABLE

//#define MOTE8
#define MOTE16
//#define MOTE32
//#define MOTE64

#ifdef MOTE8
#define STSZ 8
#endif
#ifdef MOTE16
#define STSZ 16
#endif
#ifdef MOTE32
#define STSZ 32
#endif
#ifdef MOTE64
#define STSZ 64
#endif
#define STM1 STSZ-1

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
#define FLEASEED 0xf1ea5eed
//#define NOT
#define DORSW
#define RI 0

typedef struct ranctx { u4 state[STSZ]; u4 rsl[STSZ]; u4 b; u4 c; u4 d; u4 e; } ranctx;

#define rot(x,k) ((x<<k)|(x>>(32-k)))

u4 rcnt=0; u8 fcount=0; u4 icount=0;

// MOTE ROT switcher
typedef struct Rsw { u4 iii; u4 jjj; u4 kkk; };
static u4 ri=0;
#ifdef MOTE8
#define RSW 3
static const struct Rsw rsw[RSW+1] = {
#ifndef NOT
28,  19, 16, // avalanche: 21.0 bits (worst case) OK countx 2-12 (2**30)
17,  15, 28, // avalanche: 21.0 bits (worst case) OK countx 2-12 (2**29)
15,  28, 12, // avalanche: 21.0 bits (worst case) OK countx 2-12 (2**29)
25,   5, 20  // avalanche: 21.0 bits (worst case) OK in combo
// 5,  24, 20, // avalanche: 21.0 bits (worst case)
// 4,  27,  8, // avalanche: 21.0 bits (worst case)
// 3,  26, 20  // avalanche: 21.0 bits (worst case)
//26,  21, 28, // avalanche: 21.0 bits (worst case) OK countx 2-12 (2**29) BIAS in Combo
#else
 5,  29, 14, // avalanche: 21.0 bits (worst case)
 9,  20, 18, // avalanche: 21.0 bits (worst case)
10,   3, 26, // avalanche: 21.0 bits (worst case)
29,   5,  6  // avalanche: 21.0 bits (worst case)
// 8,  27, 20, // avalanche: 21.0 bits (worst case)
//18,   3, 24, // avalanche: 21.0 bits (worst case)
//27,  16, 24, // avalanche: 21.0 bits (worst case)
//28,  25, 26, // avalanche: 21.0 bits (worst case)
#endif
#endif
#ifdef MOTE16
#define RSW 3
static const struct Rsw rsw[RSW+1] = {
#ifdef NOT
25, 10, 10, // avalanche: 20.50 bits (worst case)
 8, 13, 20, // avalanche: 20.00 bits (worst case)
16,  9, 24, // avalanche: 20.00 bits (worst case)
 3, 18, 10, // avalanche: 19.50 bits (worst case)
 6, 13,  6, // avalanche: 19.50 bits (worst case)
10,  3, 18, // avalanche: 19.50 bits (worst case)
13, 28, 22, // avalanche: 19.50 bits (worst case)
23,  6, 20 // avalanche: 19.50 bits (worst case)
#else
29, 12, 16, // avalanche: 20.50 bits - OK 10/10 10/10 10/10; 2C:10/10
26, 16, 20, // avalanche: 19.50 bits - OK 10/10 10/10 10/10; 2C:10/10
15, 18, 12, // avalanche: 19.00 bits - OK 7/10 9/10 10/10; 3C:10/10; not w/ 14,8,28
20, 20, 16, // avalanche: 19.00 bits - OK 4C:9/10,9/10,9/10,9/10,8/10,9/10,10/10
#endif
#endif
#ifdef MOTE32
#define RSW 3
static const struct Rsw rsw[4] = {
 3,  27, 30, // avalanche: 19.00 bits (worst case)
19,  19, 14, // avalanche: 18.75 bits (worst case)
15,  23, 22, // avalanche: 19.00 bits (worst case)
10,  18, 10  // avalanche: 18.75 bits (worst case)
#endif
#ifdef MOTE64
#define RSW 3
static const struct Rsw rsw[4] = {
13,  11, 11, // avalanche: 17.75 bits (worst case)
19,  24,  6, // avalanche: 17.75 bits (worst case)
14,   9, 22, // avalanche: 17.75 bits (worst case)
17,  20, 16  // avalanche: 17.87 bits (worst case)
#endif
};


static u4 ranval(ranctx *x) {
	register u4 i,r;
	r = x->rsl[rcnt];
	++rcnt; ++fcount;
	if (rcnt==STSZ) {
		for (i=0; i<STSZ; i++) {
			#ifndef NOT
			x->state[x->c & STM1] = x->e;
			#else
			x->state[x->c & STM1] = ~x->e;
			#endif
			x->b = x->c ^ ((x->e>>rsw[ri].iii) | (x->d<<rsw[ri].jjj));
			x->c = x->d - rot(x->b,rsw[ri].kkk);
			x->d = x->state[i] + x->b;
			x->e = x->c + x->d;
			x->rsl[i] = x->c;
		}
		rcnt = 0;
		#ifdef DORSW
		ri = x->d & RSW;
		#endif
	}
	return r;
}

static void raninit(ranctx *x, u4 seed) {
	register u4 i,r;
	icount++;
	x->e = x->b = x->c = x->d = FLEASEED;
	for (i=0; i<STSZ; i++) { x->state[i]=GOLDEN; x->rsl[i]=0; }
	x->state[0]=seed;
	// init with u4 seed
	for (i=0; i<20; i++) r = ranval(x);
}


/* initialize the data collection array */
static void datainit( u8 *data)
{
  u4 i;
  for (i=0; i<BUCKETS; ++i) data[i] = (u8)0;
}

/* gather statistics on len overlapping subsequences of length 5 each */
#define INCREMENT 0x10000
static void gather( ranctx *r, u8 *data, u8 len)
{
  u8 i;
  u4 j;
  for (i=0; i<len; i+=INCREMENT) {
    for (j=0; j<INCREMENT; j+=4) {
      ++data[ranval(r)&(BUCKETS-1)];
      ++data[ranval(r)&(BUCKETS-1)];
      ++data[ranval(r)&(BUCKETS-1)];
      ++data[ranval(r)&(BUCKETS-1)];
    }
  }
}

static void chi( u8 *data, u8 len)
{
  u4 i;
  double var = 0.0;         /* total variance */
  double temp;              /* used to calculate variance of a bucket */
  double expect = ((double)len)/BUCKETS;
  
  for (i=0; i<BUCKETS; ++i) {
    double temp = (double)data[i] - expect;
    var += temp*temp/expect;
  }

  /* calculate the total variance and chi-square measure */
  #ifdef TABLE
  printf("2**%2d	%2d	%10.2f	%10.2f	%8.4f",loglen,terms,(float)BUCKETS-1,(float)var, 
	 (float)((var-(BUCKETS-1))/sqrt((float)(BUCKETS-1))));
  #else
  printf("expected variance: %11.4f   got: %11.4f   chi-square: %6.4f\n",
         (float)(BUCKETS-1), (float)var, 
	 (float)((var-(BUCKETS-1))/sqrt((float)(BUCKETS-1))));
  #endif
}


int main( int argc, char **argv)
{
  u8 len;
  u8 data[BUCKETS];
  ranctx r;
  time_t a,z;
  u4 i;
  
  printf("%s: ",argv[0]);
  #ifdef MOTE8
  printf("MOTE8");
  #endif
  #ifdef MOTE16
  printf("MOTE16");
  #endif
  #ifdef MOTE32
  printf("MOTE32");
  #endif
  #ifdef MOTE64
  printf("MOTE64");
  #endif
  #ifdef NOT
  printf("N");
  #endif
  #ifdef DORSW
  printf(" RSW(%d) ",RSW+1);
  for (i=0;i<=RSW;i++) printf("{%2d,%2d,%2d}",rsw[i].iii,rsw[i].jjj,rsw[i].kkk);
  #else
  printf(" (RI=%d) {%2d,%2d,%2d}",RI,rsw[RI].iii,rsw[RI].jjj,rsw[RI].kkk);
  #endif
  puts("");

	#ifdef TABLE
	printf("Trial	Terms	Expected V	Observed V	Chi-square	Time\n");
	printf("---------------------------------------------------------------------\n");
	#endif

	time(&a);
  if (argc == 2) {
    sscanf(argv[1], "%d", &loglen);
    #ifdef VERBOSE
	printf("log_2 sequence length: %d\n", loglen);
    #endif
	len = (((u8)1)<<loglen);
  } else {
    fprintf(stderr, "usage: \"freq 24\" means run for 2^^24 values\n");
    return 1;
  }

  datainit(data);
  raninit(&r, 0);
  gather(&r, data, len);
  chi(data, len);

  time(&z);
  
  #ifdef TABLE
	printf("	%3d s\n",(size_t)(z-a));
  #endif
  #ifdef VERBOSE
  printf("\nnumber of seconds      : %12d\n", (size_t)(z-a));
  printf("number of ranval calls : %12llu\n", fcount);
  printf("number of raninit calls: %12u\n", icount);
  #endif
  return 0;
}
