	MOTE - A family of small-state CSPRNGs and Stream Ciphers
	MOTE may be seeded with a 256-, 512-, or 1024-bit key
	MOTE Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
	MOTE is inspired by Bob Jenkins' PRNG insights (Public Domain).

What is MOTE?

MOTE is a fast, small-state, cryptographically secure pseudo-random number generator (CSPRNG) and stream cipher. MOTE exhibits uniform distribution, mixes extremely rapidly, has no detected bias, and comes in three variants: MOTE8, with an internal state array of 8+4 32-bit words; MOTE16, with an internal state of 16+4 words; and MOTE32 with a 32+4-word state. The former permit seeding with a key of up to 256 or 512 bits, the latter with a 1024-bit key.

Reduced to essentials, MOTE is only 5 lines of code:

		a = e;
		b = c ^ (e >> iii | d << jjj);
		c = d - rot(b, kkk);
		d = a + b;
		e = c + d;
		
<a> being an index into MOTE's internal state array.
		
Two shifts, one rotation, two additions, one subtraction, one OR, one XOR, one pseudo-random lookup. Ten arithmetic and bitwise operations in all. It's called MOTE because a CSPRNG can't be made much smaller (or faster) without introducing bias or severely reducing avalanche.

MOTE's default RSW configuration alternates the shift and rotation constants unpredictably. These four sets of three values differ between MOTE8, MOTE16 and MOTE32 and were selected, tuned and tested in each case for absence of bias and optimal avalanche, which is never less than 21 bits (MOTE8), 20 bits (MOTE16) and 19 bits (MOTE32). The quantities are:

MOTE8 -

1. iii = 28, jjj = 19, kkk = 16, // avalanche: 21.0 bits (worst case)
2. iii = 17, jjj = 15, kkk = 28, // avalanche: 21.0 bits (worst case)
3. iii = 15, jjj = 28, kkk = 12, // avalanche: 21.0 bits (worst case)
4. iii = 25, jjj =  5, kkk = 20  // avalanche: 21.0 bits (worst case)

MOTE16 -

1. iii = 29, jjj = 12, kkk = 16, // avalanche: 20.50 bits (worst case)
2. iii = 26, jjj = 16, kkk = 20, // avalanche: 19.50 bits (worst case)
3. iii = 15, jjj = 18, kkk = 12, // avalanche: 19.00 bits (worst case)
4. iii = 20, jjj = 20, kkk = 16  // avalanche: 19.00 bits (worst case)

MOTE32 -

1. iii = 30, jjj =  4, kkk = 20, // avalanche: 19.00 bits (worst case)
2. iii = 11, jjj =  2, kkk = 26, // avalanche: 18.75 bits (worst case)
3. iii = 19, jjj = 19, kkk = 14, // avalanche: 18.75 bits (worst case)
4. iii =  6, jjj = 16, kkk = 12  // avalanche: 18.25 bits (worst case)


The three MOTE variants presented here have been implemented identically in C and Pascal and can be found in their respective directories along with m, mm and mmm - three short programs to check their output. MOTE was developed and tested with the Microsoft Visual C compiler (cl) and Free Pascal (fpc) under Win32. The NIST statistical tests were run under Linux.

To view MOTE's abbreviated internal state, please uncomment "#define TEST" in the C files and "{$define TEST}" in the Pascal files and "#define VERBOSE" and "{$define VERBOSE}" to display the full result & state arrays as they are generated on each call to moteXXX.

PRNG tests passed by MOTE:

* NIST suite of statistical tests for randomness: all tests for all seeds
* George Marsaglia's DIEHARD suite: all tests for all seeds
* Bob Jenkins' countx for sub-sequences of length 2-12 (2**35 trials)
* Bob Jenkins' rngav (avalanche better than 20.0 bits average)
* John Walker's ENT test for entropy
* C.C.Kayne's GapTrial (value-spacings normal over all modulos)
* C.C.Kayne's ModTrial (value-distributions uniform over all modulos)
* C.C.Kayne's SeqTrial (same-value sequences normal over all modulos)
* C.C.Kayne's PopTrial (bit-populations iid-normal over all trial ranges)
* C.C.Kayne's Visual 1 & 2

(Please view the Results.txt files in the relevant folders or verify the recorded results by re-running the tests themselves. The principal MOTE repositories are listed at the foot of this document.)

Have fun with MOTE, and don't hesitate to get in touch if you encounter issues with any of the files included in this distribution. As always, your comments, suggestions, criticism and general feedback are welcomed.

C.C.Kayne
cckayne@gmail.com
https://github.com/cckayne/MOTE
https://sourceforge.net/projects/mote-csprng-stream-cipher/
https://code.google.com/p/mote-csprng-stream-cipher/
https://code.google.com/p/bedbug-csprng-stream-cipher/
https://sourceforge.net/projects/bedbug-csprng-stream-cipher/
