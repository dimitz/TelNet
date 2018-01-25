// DateTime.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "conio.h"
#include "string"
#include "windows.h"
#include <stdlib.h>
//#include "locale"
#include <string>
//#include <iostream>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include "TelNet.h"
//#define request "\r\n\r\n"

enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

void GetTime(char*);				//ф-ия получения даты времени
void Sender(SOCKET, char*);			//ф-ия отправки в сокет
void Receiver(SOCKET, char*);		//ф-ия приёма из сокета
void SearchNum(char*, char*);		//ф-ия поиска чисел в строке, передаём ей указатель на строку и указатель на буфер с числом, указатель смещаем на конец числа

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hSTDOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//CONSOLE_SCREEN_BUFFER_INFO SCRN_INFO;
	//GetConsoleScreenBufferInfo(hSTDOut, &SCRN_INFO);
	/*  FillConsoleOutputCharacter(hOutput, ' ',
	SCRN_INFO.dwSize.X*SCRN_INFO.dwSize.Y, POS, NULL);*/
	//FillConsoleOutputAttribute(hSTDOut,
	//	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE /*цвет*/,
	//	SCRN_INFO.dwSize.X*SCRN_INFO.dwSize.Y, SCRN_INFO.dwCursorPosition, NULL);
	//WORD wColour = FOREGROUND_BLUE;
	system("color 80");
	//SetConsoleTextAttribute(hSTDOut, 2);
	SetConsoleTextAttribute(hSTDOut, (WORD)((DarkGray << 4) | Blue));				//wColour | FOREGROUND_INTENSITY);
	//GetConsoleFontSize(hSTDOut, 14);


	char t[20];
	int iIpAdr;
	int iPort;
	char cIpAdr[100];
	char cPort[8];
	char fullIpAdr[30];
	char title[35];
	char template1[] = "75.";
	char template2[] = "175.";
	char cTemp[30] = "";
	int vvod = 0;
	int lenstr;
	//DWORD written;
	//char request[] = "\r\n";
	char buff[4096];
	setlocale(LC_ALL, "Russian");
	//cout << "Ура" << endl;
	//printf("Ура!\n");
	//printf("Текущее время: ");
	GetTime(t);
	//printf(t);
	//printf("\n");
	cout << "Текущее время: " << t << endl;

	
	//printf("Введите ip-адрес MSAN: 10.15.");
	//scanf("%7s", cIpAdr);
	//strcpy(fullIpAdr, "10.15.");
	//strcat(fullIpAdr, cIpAdr);
	//printf("Введите номер порта: ");
	//scanf("%i", &iPort);
	//_itoa_s(iPort, cPort, 10);
	//printf("Подключаемся к ");
	//printf(fullIpAdr);
	//printf("\n");
	while (vvod == 0)
	{
		cout << "Введите ip-адрес: 10.15.";
		cin >> cIpAdr;
		lenstr = strlen(cIpAdr);

		if (strncmp(cIpAdr, template1, 3) == 0)
		{
			vvod = 1;
			strncpy(cTemp, cIpAdr + 3, lenstr - 3);
			cTemp[lenstr - 3] = 0;
		}
		else{
			if (strncmp(cIpAdr, template2, 4) == 0){
				vvod = 1;
				strncpy(cTemp, cIpAdr + 4, lenstr - 4);
				cTemp[lenstr - 4] = 0;
			}
		}
		iIpAdr = atoi(cTemp);
		if (iIpAdr < 1 || iIpAdr > 255)
		{
			vvod = 0;
		}

	}

	vvod = 0;
	while (vvod == 0)
	{
		cout << "Введите порт: ";
		cin >> iPort;
		if (iPort >= 0 && iPort < 65)
		{
			vvod = 1;
			//cout << "Порт: " << iPort << endl;
		}
	}



	strcpy(fullIpAdr, "10.15.");
	strcat(fullIpAdr, cIpAdr);
	_itoa(iPort, cPort, 10);
	cout << "Подключаемся к " << fullIpAdr << " Порт: " << cPort << endl;
	strcpy(title, "Explore ");
	strcat(title, fullIpAdr);
	strcat(title, " port ");
	strcat(title, cPort);
	SetConsoleTitleA(title);



	char OutFile[20];
	ZeroMemory(OutFile, strlen(OutFile));
	strcpy(OutFile, fullIpAdr);
	strcat(OutFile, "_");
	strcat(OutFile, cPort);
	strcat(OutFile, ".txt");
	//cout << OutFile << endl;
	HANDLE hFile = CreateFileA(OutFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);




	/*int y = WriteFile(hFile, t, strlen(t), &written, NULL);
		cout << "записано " << written << " байт" << endl;
		if (y == true)
		{
			cout << "запись в файл успешна" << endl;
		}*/

	//Создаём файл
	//char OutFile[20];
	//ZeroMemory(OutFile, strlen(OutFile));
	//strcpy(OutFile, fullIpAdr);
	//strcat(OutFile, "_");
	//strcat(OutFile, cPort);
	//strcat(OutFile, ".txt");
	//cout << OutFile << endl;
	//HANDLE hFile = CreateFileA(OutFile, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	//Займёмся сокетом

	WSADATA wsaData;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0){
		cout << "WinSock не инициализируется." << endl;
		return 1;
	}
	//cout << "WinSock инициализирован!" << endl;
	struct addrinfo *result;


	//Преобразуем адрес сервера

	iResult = getaddrinfo("10.10.10.2", "23", NULL, &result);
	if (iResult != 0)
	{
		cout << "Адрес не преобразуется!" << endl;
		WSACleanup();
		return 1;
	}

	SOCKET sock;
	// Создаём сокет
	if (INVALID_SOCKET == (sock = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP)))
	{
		// Error
		return -1;
	}
	//cout << "Создали сокет!" << endl;

	//Подключаемся к серверу
	iResult = connect(sock, result->ai_addr, result->ai_addrlen);
	if (iResult != 0)
	{
		cout << "Невозможно подключиться к серверу!" << endl;
		return -1;
	}

	TelNet telnet;
	//Посылаем запрос
	/*memset(buff, NULL, sizeof(buff));
	strcpy(buff, "\r\n");
	iResult = _countof(buff);
	telnet.Sender(sock, buff);*/
	//Sender(sock, buff);

	Sleep(1000);
	//Получаем ответ
	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;

	Sleep(1000);
	//Получаем ответ
	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;


	Sleep(1000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "admin\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;

	Sleep(500);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "ton2Pcl\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;

	//char B[40];
	//memset(B, NULL, sizeof(buff));
	//strcpy(B, "telnet ");
	//strcat(B, fullIpAdr);
	//strcat(B, "\r\n");
	//cout << B << endl;


	Sleep(100);
	char podklSi[] = "telnet ";
	strcat(podklSi, fullIpAdr);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, podklSi);
	telnet.Sender(sock, buff);
	Sleep(500);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;

	Sleep(5000);
	//memset(buff, NULL, sizeof(buff));
	//strcpy(buff, B);
	//telnet.Sender(sock, buff);
	//Sleep(1500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;


	Sleep(2000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;

	Sleep(5000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "admin\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff;

	Sleep(5000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "admin\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	//cout << buff << endl;

	Sleep(500);
	/*memset(buff, NULL, sizeof(buff));
	strcpy(buff, "show dsl port current values 0/19\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	cout << buff;

	Sleep(15000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "cd home\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	cout << buff;

	Sleep(1000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "cd httpd\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	cout << buff;

	Sleep(1000);
	memset(buff, NULL, sizeof(buff));
	strcpy(buff, "cd css\r\n");
	telnet.Sender(sock, buff);
	Sleep(500);

	memset(buff, NULL, sizeof(buff));
	telnet.Receiver(sock, buff);
	cout << buff;*/

	//Создаём файл
	//Создаём файл
	//char OutFile[20];
	ZeroMemory(OutFile, strlen(OutFile));
	strcpy(OutFile, fullIpAdr);
	strcat(OutFile, "_");
	strcat(OutFile, cPort);
	strcat(OutFile, ".txt");
	//cout << OutFile << endl;
	//HANDLE hFile = CreateFileA(OutFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	char znachen1[10];
	char znachen2[10];
	char znachen3[10];
	char znachen4[10];
	char znachen5[10];
	char znachen6[20];
	char znachen7[20];
	char OutString[256];
	char Command1[38] = "show dsl port current values 0/";
	strcat(Command1, cPort);
	strcat(Command1, "\r\n");
	char Command2[30] = "show dsl port state 0/";
	strcat(Command2, cPort);
	strcat(Command2, "\r\n");
	char Tit[] = "Date             Time        Profile      State      DRate   URate  DSNR  USNR  Duration\r\n";
	cout << Tit;

	while (true)
	{
		Sleep(100);
		ZeroMemory(znachen1, strlen(znachen1));
		ZeroMemory(znachen2, strlen(znachen2));
		ZeroMemory(znachen3, strlen(znachen3));
		ZeroMemory(znachen4, strlen(znachen4));
		ZeroMemory(znachen5, strlen(znachen5));
		ZeroMemory(znachen6, strlen(znachen6));
		ZeroMemory(znachen6, strlen(znachen7));
		memset(buff, NULL, sizeof(buff));
		strcpy(buff, Command1);							//"show dsl port current values 0/19\r\n");
		telnet.Sender(sock, buff);
		Sleep(1500);

		Sleep(1000);
		memset(buff, NULL, sizeof(buff));
		strcpy(buff, " ");
		telnet.Sender(sock, buff);
		Sleep(1500);


		memset(buff, NULL, sizeof(buff));
		telnet.Receiver(sock, buff);
		//cout << buff;

		


		//Преобразуем char[] в string
		/*string buffer;
		buffer = buff;
		int len;
		len = buffer.length();
		cout << "длина буфера - " << len << endl;
		//cout << buffer;*/

		//Разбиваем буфер на лексемы

		char mystr[4096];
		//char tempstr[4096];
		char number[10];
		char *pch = strtok(buff, "\n");
		while (pch != NULL)
		{
			strcpy(mystr, pch);
			memset(number, NULL, sizeof(number));
			//Ищем вхождение
			if (strstr(mystr, "Data rate:                  ") != 0)
			{
				//cout << "Есть вхождение " << mystr << endl;
				SearchNum(mystr, number);							//ищем первое число
				strcpy(znachen1, number);
				//cout << znachen1;
				//cout << "     ";
				SearchNum(mystr, number);							//ищем второе число
				strcpy(znachen2, number);
				//cout << znachen2;
				//cout << endl;
			}
			if (strstr(mystr, "SNR") != 0)
			{
				//cout << "Есть вхождение " << mystr << endl;
				SearchNum(mystr, number);							//ищем первое число
				strcpy(znachen3, number);
				//cout << znachen3;
				//cout << "     ";
				SearchNum(mystr, number);							//ищем второе число
				strcpy(znachen4, number);
				//cout << znachen4;
				//cout << "     ";
				//SearchNum(mystr, number);							//ищем третье число
				//strcpy(znachen5, number);
				//cout << znachen5;

				//cout << endl;
			}

			if (strstr(mystr, "Operational") != 0)
			{
				//cout << "Есть вхождение " << mystr << endl;
				strncpy(znachen5, mystr + 23, 6);
				//cout << znachen5;
				//cout << endl;
			}

			if (strstr(mystr, "Profile:          ") != 0)
			{
				//cout << "Есть вхождение " << mystr << endl;
				strncpy(znachen6, mystr + 23, 20);
				//cout << znachen6;
				//cout << endl;
			}


			pch = strtok(NULL, "\n");
		}


		Sleep(100);
		memset(buff, NULL, sizeof(buff));
		strcpy(buff, Command2);							//"show dsl port state 0/19\r\n");
		telnet.Sender(sock, buff);
		Sleep(1500);

		Sleep(1000);
		memset(buff, NULL, sizeof(buff));
		strcpy(buff, " ");
		telnet.Sender(sock, buff);
		Sleep(1500);


		memset(buff, NULL, sizeof(buff));
		telnet.Receiver(sock, buff);
		//cout << buff;

		char *pcl = strtok(buff, "\n");
		while (pcl != NULL){
			strcpy(mystr, pcl);
			memset(number, NULL, sizeof(number));
			//Ищем вхождение
			if (strstr(mystr, "Time from") != 0)
			{
				//cout << "Есть вхождение " << mystr << endl;
				SearchNum(mystr, number);							//ищем первое число
				strcpy(znachen7, number);
				cout << znachen7;
				//cout << "     ";
				//SearchNum(mystr, number);							//ищем второе число
				//strcpy(znachen2, number);
				//cout << znachen2;
				//cout << endl;
			}
			pcl = strtok(NULL, "\n");
		}

		GetTime(t);
		memset(OutString, NULL, sizeof(OutString));
		strcpy(OutString, t);					//пишем выходные данные в одну строку
		strcat(OutString, "    ");
		strcat(OutString, znachen6);
		strcat(OutString, "    ");
		strcat(OutString, znachen5);
		strcat(OutString, "    ");
		strcat(OutString, znachen1);
		strcat(OutString, "    ");
		strcat(OutString, znachen2);
		strcat(OutString, "    ");
		strcat(OutString, znachen3);
		strcat(OutString, "    ");
		strcat(OutString, znachen4);
		strcat(OutString, "    ");
		strcat(OutString, znachen7);
		strcat(OutString, "\r\n");

		cout << OutString << endl;
		//int y = WriteFile(hFile, OutString, strlen(OutString), &written, NULL);
		//cout << "записано " << written << " байт" << endl;
		//if (y == true)
		//{
		//	cout << "запись в файл успешна" << endl;
		//}

		//WriteFile(hFile, "\r\n", 4, &written, NULL);

		


	}



	_getch();
	return 0;
}

void GetTime(char *sBuff){
	SYSTEMTIME strTime;
	GetSystemTime(&strTime);
	int iY = strTime.wYear;
	int iMo = strTime.wMonth;
	int iD = strTime.wDay;
	int iH = strTime.wHour + 3;
	int iM = strTime.wMinute;
	int iS = strTime.wSecond;
	char cY[] = "0000";
	char cMo[] = "00";
	char cD[] = "00";
	char cH[] = "00";
	char cM[] = "00";
	char cS[] = "00";



	if (iH >= 24)
	{
		iH = iH - 24;
	}

	_itoa_s(iY, cY, 10);
	_itoa_s(iMo, cMo, 10);
	_itoa_s(iD, cD, 10);
	_itoa_s(iH, cH, 10);
	_itoa_s(iM, cM, 10);
	_itoa_s(iS, cS, 10);

	strcpy(sBuff, "");
	if (iD < 10)
	{
		strcat(sBuff, "0");
	}
	strcat(sBuff, cD);
	strcat(sBuff, "-");
	if (iMo < 10)
	{
		strcat(sBuff, "0");
	}
	strcat(sBuff, cMo);
	strcat(sBuff, "-");
	strcat(sBuff, cY);
	strcat(sBuff, " ");
	if (iH < 10)
	{
		strcat(sBuff, "0");
	}
	strcat(sBuff, cH);
	strcat(sBuff, ":");
	if (iM < 10)
	{
		strcat(sBuff, "0");
	}
	strcat(sBuff, cM);
	strcat(sBuff, ":");
	if (iS < 10)
	{
		strcat(sBuff, "0");
	}
	strcat(sBuff, cS);


	//printf(sBuff);
	//printf("\n");

}
void SearchNum(char *mystr, char *number){
	char simv = 'a';
	int a = 0;								//текущая позиция в строке
	int b = 0;								//кол-во знаков в числе
	bool find_it = false;					//признак нахождения числовой константы

	memset(number, NULL, sizeof(number));
	int mystr_len = strlen(mystr);			//длина строки
	while (simv != NULL && find_it == false)
	{
		simv = mystr[a];
		if (simv >= '0' && simv <= '9')		//если нашли первую цифру
		{
			number[b] = simv;				//записываем её в number
			b++;
			a++;
			while (simv != NULL)
			{
				simv = mystr[a];			//и ищем остальные
				if (simv >= '0' && simv <= '9' || simv == '.')		//в том числе и десятичную точку
				{
					number[b] = simv;
					b++;
					a++;
				}
				else
				{
					break;
				}
			}

			char tempstr[256];										//временный текстовый буфер
			memset(tempstr, NULL, sizeof(tempstr));					//очищаем его
			for (int i = 0; i < (mystr_len - a); i++)				//если считали число полностью, сдвигаем указатель в строке на позицию после числа
			{
				tempstr[i] = mystr[i + a];
			}
			strcpy(mystr, tempstr);
			find_it = true;
			break;

		}
		a++;
	}
	return;								//возвращаемся
}
/*void Sender(SOCKET sock, char* buf){
int i = send(sock, buf, strlen(buf), 0);
if (i == SOCKET_ERROR)
{
cout << "Запрос не посылается!" << endl;
}
}
void Receiver(SOCKET sock, char* buf){
int i = recv(sock, buf, 4096, 0);
buf[i] = 0;
}*/
