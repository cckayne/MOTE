// m: Check output from the MOTE8 CSPRNG 
program m;
uses mote8;
{ $define STREAM}
// mixing rounds: 7 is recommended minimum
const ROUNDS = 7;
var n: Cardinal;
	seed: string = 'Bacon';
begin
	// pull a seed from the command line
	if ParamCount>0 then seed:=ParamStr(1);
	// print some information
	Writeln('CSPRNG        : ',moName);
	Writeln('Internal state: ',moStateBits,' bits');
	Writeln('Minimum cycle : 2**',moCycle);
	Writeln('Maximum cycle : 2**',moStateBits);
	Writeln('Average cycle : 2**',moStateBits-2);
	// seed MOTE and mix
	moSeedW(seed,8*ROUNDS);
	{$ifdef STREAM}
	// show some MOD 26 stream output
	for n:=1 to 672 do
		Write(chr(moRandom mod 26 + ord('A')));
	Writeln;
	{$endif}
end.
