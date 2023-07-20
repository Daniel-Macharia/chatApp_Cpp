#include<iostream>
#include<ws2tcpip.h>
#include<string>
#include<bitset>
#include<chrono>
#include<thread>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::bitset;

int sendData(SOCKET &s, char* buf,int size, int flags)
{
	cout<<"Enter Message: ";
	cin.getline(buf, size);
	if( send(s, buf, size, flags) > 0)
	{
		cout<<"Message sent: "<<buf<<endl;
		return 0;
	}
	else
	{
		cout<<"Error sending message: "<<WSAGetLastError()<<endl;
		return 1;
	}
}

int recieveData(SOCKET &s, char* buf, int size, int flags)
{
	if( recv(s, buf, size, flags) > 0)
	{
		cout<<"Message Recieved: "<<buf<<endl;
		return 0;
	}
	else
	{
		cout<<"No Message Recieved.."<<endl;
		return 1;
	}
}

void chat(SOCKET &s)
{
	
	//start chatting untill the keyword shutdown is typed
   
   char rBuf[1000] = "";
   char sBuf[1000] = "";
   int i = 1;
   
   //chat so long as neither the server nor the client sends the message 'shutdown'
   while( strcmp(rBuf, "shutdown") != 0 && strcmp(sBuf, "shutdown") != 0 )
   {
   	recieveData(s, rBuf, 1000, 0);
   	sendData(s, sBuf,1000,0);
   }
   
}

int main()
{
	//load winsock api
	WSADATA wsadata;
	int wsaerr;
	WORD version = MAKEWORD(2, 2);
	
	wsaerr = WSAStartup( version, &wsadata);
	
	if( wsaerr == 0)
	{
		cout<<"Successfully loaded the winsock api"<<endl;
	}
	else
	{
		cout<<"failed to load the winsock api"<<WSAGetLastError()<<endl;
		return -1;
	}
	
	//create a socket
	SOCKET client = INVALID_SOCKET;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( client == INVALID_SOCKET )
	{
		cout<<"failed to create client socket"<<WSAGetLastError()<<endl;
		WSACleanup();
		return -1;
	}
	else
	{
		cout<<"successfully created client socket"<<endl;
	}
	
	//connect to the server
	
	//specify the ip of the server
	//Inet_Pton(AF_INET, _T("127.0.0.1"), &address.sin_addr.s_addr);
	//inetaton("127.0.0.1", &address.sin_addr.s_addr);
	//        "    1    .  0    .   0   .  127  "    //127.0.0.1 in binary    
	string s = "00000001000000000000000001111111";
	bitset<32> b(s);
	
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = b.to_ulong();
	int port = 55555;
	address.sin_port = htons(port);
	
	if( connect(client, (SOCKADDR*) &address, sizeof(address) ) == 0)
	{
		cout<<"successfully connected to the server"<<endl;
		//on successful connection to the server, start chatting
		chat(client);
		
	}
	else
	{
		cout<<"failed to connect to the server "<<WSAGetLastError()<<endl;
		closesocket(client);
		WSACleanup();
		return -1;
	}
	
	cout<<"Bye"<<endl;
	std::this_thread::sleep_for( std::chrono::milliseconds(5000));
	
	return 0;
}
