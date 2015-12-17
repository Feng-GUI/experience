// guitarSOck.cpp : Defines the entry point for the console application.
//



#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include "windows.h"
#include "stdafx.h"
#include "math.h"
#include "guitarSOck.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ----Prototypes of Inp and Outp--- */

short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);
UINT  ServerThread(LPVOID pParam);
char tunePrinter(unsigned char tune[8]);
int encodeFeedByte(char ByteArray[]);
/*--------------------------------*/

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{

	int nRetCode = 0;
	if (argc > 1 && strcmp(argv[1],"debug") == 0)
	{
		int dataByte;
		scanf("%d",&dataByte);
		
		while (dataByte != 999)
		{
			printf("you entered: %d\n",dataByte);
			Out32(0x3BC,dataByte);
			//Out32(0x378,dataByte);
			scanf("%d",&dataByte);
		}
		printf("see ya\n");
	} else
	{
	    cout << "Press ESCAPE to terminate program\r\n";
	    AfxBeginThread(ServerThread,0);
	    while(_getch()!=27);
	}
	
    return nRetCode;
}


UINT  ServerThread(LPVOID pParam)
{	
    cout << "Starting up TCP server\r\n";

    //A SOCKET is simply a typedef for an unsigned int.
    //In Unix, socket handles were just about same as file 
    //handles which were again unsigned ints.
    //Since this cannot be entirely true under Windows
    //a new data type called SOCKET was defined.
    SOCKET server;

    //WSADATA is a struct that is filled up by the call 
    //to WSAStartup
    WSADATA wsaData;

    //The sockaddr_in specifies the address of the socket
    //for TCP/IP sockets. Other protocols use similar structures.
    sockaddr_in local;

    //WSAStartup initializes the program for calling WinSock.
    //The first parameter specifies the highest version of the 
    //WinSock specification, the program is allowed to use.
    int wsaret=WSAStartup(0x101,&wsaData);

    //WSAStartup returns zero on success.
    //If it fails we exit.
    if(wsaret!=0)
    {
        return 0;
    }

    //Now we populate the sockaddr_in structure
    local.sin_family=AF_INET; //Address family
    local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
    local.sin_port=htons((u_short)20248); //port to use

    //the socket function creates our SOCKET
    server=socket(AF_INET,SOCK_STREAM,0);

    //If the socket() function fails we exit
    if(server==INVALID_SOCKET)
    {
        return 0;
    }

    //bind links the socket we just created with the sockaddr_in 
    //structure. Basically it connects the socket with 
    //the local address and a specified port.
    //If it returns non-zero quit, as this indicates error
    if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
    {
        return 0;
    }

    //listen instructs the socket to listen for incoming 
    //connections from clients. The second arg is the backlog
    if(listen(server,10)!=0)
    {
        return 0;
    }

    //we will need variables to hold the client socket.
    //thus we declare them here.
    SOCKET client;
    sockaddr_in from;
    int fromlen=sizeof(from);
	bool isConnected = false;

    while(!isConnected)//we are looping endlessly
    {
        char temp[512];

        //accept() will accept an incoming
        //client connection
        client=accept(server,
            (struct sockaddr*)&from,&fromlen);
		
        sprintf(temp,"Your IP is %s\r\n",inet_ntoa(from.sin_addr));

		cout << "Connection from " << inet_ntoa(from.sin_addr) <<"\r\n";

		int bytesRecv = 0;
		char sendbuf[32] = "Client: Sending data.";
		char recvbuf[6] = "";


		while( bytesRecv != SOCKET_ERROR )
		{
			printf("before recv\n");
			bytesRecv = recv( client, recvbuf, sizeof(recvbuf), 0 );
			printf("after recv\n");

			if (bytesRecv <= 0) 
			{
				printf( "Connection Closed.\n");
				break;
			}

			printf( "Recv: %ld bytes which are:", bytesRecv);
			for (int i=0;i<sizeof(recvbuf);i++)
			{
				printf("%c",recvbuf[i]);
			}
			printf("\n");
			int dataByte = encodeFeedByte(recvbuf);
			printf("ecoded to: %d\n", dataByte);
			//Out32(0x378,dataByte+32);
			//Sleep(150);
			//Out32(0x378,dataByte);
			Out32(0x3BC,dataByte);
			
		}

        //we simply send this string to the client
//        send(client,temp,strlen(temp),0);

        //close the client socket
		printf("closing client socket....");
        closesocket(client);
		printf(" closed.\n");

    }

    //closesocket() closes the socket and releases the socket descriptor
	printf("closing server socket....");
    closesocket(server);
	printf(" closed.\n");

    //originally this function probably had some use
    //currently this is just for backward compatibility
    //but it is safer to call it as I still believe some
    //implementations use this to terminate use of WS2_32.DLL 
    WSACleanup();

    return 0;
}

int encodeFeedByte(char* ByteArray)
{
	int a=0;
	for (int i=0;i<6;i++)
	{
		a+=(ByteArray[i]-'0')*pow(2,i);
	}
	return a;
}
/*char tunePrinter(unsigned char tune[8])
{
	const char SCORE_PRESS_CHR = '+';
	const char SCORE_RAISE_CHR = '-';
	const char STRUMMING_CHR = '#';

	char outStr[9] = "--------";

	for (int i=0;i<8;i++)
	{
		if (tune[i] == '1') 
		{
			outStr[i] = SCORE_PRESS_CHR;
		}
		else 
		{
			outStr[i] = SCORE_RAISE_CHR;
		}

	} 
	return outStr[0];
}*/