// InpoutTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>

/* ----Prototypes of Inp and Outp--- */

short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);

/*--------------------------------*/

int main(int argc, char* argv[])
{


// Be sure to include winsock.h stdio.h and string.h

//in your .c or .cpp file and be sure to link the project to

//wsock32.lib




	/*int dataByte;
	scanf("%d",&dataByte);
	
	while (dataByte != 999)
	{
		printf("you entered: %d\n",dataByte);
		Out32(0x378,dataByte);
		scanf("%d",&dataByte);
	}
	printf("see ya\n");
	*/

	return 0;
}
