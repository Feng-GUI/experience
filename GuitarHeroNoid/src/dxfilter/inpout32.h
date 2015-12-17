//
// output to parallel
//

#include "math.h"

short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);
short encodeFeedByte(char ByteArray[]);

short encodeFeedByte(char* ByteArray)
{
	//FIXME: don't hard coded the size of the array.
	short a=0;
	for (int i=0;i<6;i++)
	{
		a+=(ByteArray[i]-'0')*(short)pow(2,i);
	}
	return a;
}