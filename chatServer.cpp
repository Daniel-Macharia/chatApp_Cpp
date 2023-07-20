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
   //int i = 1;
   
   //chat so long as neither the client nor the server sends the message 'shutdown'
   while( strcmp(rBuf, "shutdown") != 0 && strcmp(sBuf, "shutdown") != 0 )
   {
   	sendData(s, sBuf,1000,0);
   	recieveData(s, rBuf, 1000, 0);
   	
   }
   
}

int main()
{
	//load the winsock dll
	WSADATA wsadata;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	
   wsaerr = WSAStartup(wVersionRequested, &wsadata);
   
   if( wsaerr == 0)
   {
   	cout<<"winsock dll loaded successfully"<<endl;
   }
   else
   {
   	cout<<"failed to load the winsock dll"<<WSAGetLastError()<<endl;
   	return -1;
   }
   
   //create a socket for the server
   SOCKET server = INVALID_SOCKET;
   
   server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
   if( server == INVALID_SOCKET )
   {
   	cout<<"Failed to create server socket"<<WSAGetLastError()<<endl;
   	WSACleanup();
   	return -1;
   }
   else
   {
   	cout<<"Successfully created server socket"<<endl;
   }
   
   //bind the socket to some ip address and a port number
   sockaddr_in address;
   address.sin_family = AF_INET;
   
   //represent the loopback ip as bits
   //              1    .    0   .   0    .   127
   string s = "00000001000000000000000001111111";
   bitset<32> b(s);
   address.sin_addr.s_addr = b.to_ulong();
   int port = 55555;
   address.sin_port = htons(port);
   
   if( bind(server, (SOCKADDR*) &address, sizeof(address)) == 0)
   {
   	cout<<"successfully bound server to ip and port numer"<<endl;
   }
   else
   {
   	cout<<"failed to bind server to ip and port "<<WSAGetLastError()<<endl;
   	closesocket(server);
   	WSACleanup();
   	return -1;
   }
   
   //listen on that ip and port for client(s)
   
   if( listen(server, 1) == 0)
   {
 	cout<<"Listening..."<<endl;
   }
   else
   {
   	cout<<"Failed to listen "<<WSAGetLastError()<<endl;
   	closesocket(server);
   	WSACleanup();
   	return -1;
   }
   
   //accept connections from the client
   SOCKET acceptCon = INVALID_SOCKET;
   
   acceptCon = accept(server, NULL, NULL);
   
   if( acceptCon == INVALID_SOCKET )
   {
   	cout<<"failed to accept connection from client"<<WSAGetLastError()<<endl;
   	closesocket(server);
   	WSACleanup();
   	return -1;
   }
   else
   {
   	cout<<"Succesfully connected to client\n"<<"Ready to chat"<<endl;
   	//on successful connection begin chatting
   	chat(acceptCon);
   
   }
   
   
   
   std::this_thread::sleep_for( std::chrono::milliseconds(5000));
   
	return 0;
}
