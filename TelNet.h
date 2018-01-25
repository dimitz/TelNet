#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>


using namespace std;
class TelNet
{
public:
	
	TelNet(){
		sock = NULL;
		buf = NULL;
	};
	~TelNet(){
		
	};
	void set_socket(SOCKET S);
	SOCKET get_socket();
	void set_buf(char* b);
	char* get_buf();
	void Sender(SOCKET sock, char* buf){
		int i = send(sock, buf, strlen(buf), 0);
		if (i == SOCKET_ERROR)
		{
			cout << "Запрос не посылается!" << endl;
		}
	}

	void Receiver(SOCKET sock, char* buf){
		int i = recv(sock, buf, 8192, 0);
		buf[i] = 0;
	}



private:
	SOCKET sock;
	char* buf;

};
void TelNet::set_socket(SOCKET S){
	sock = S;
}
SOCKET TelNet::get_socket(){
	return sock;
}
void TelNet::set_buf(char* b){
	buf = b;
}
char* TelNet::get_buf(){
	return buf;
}

