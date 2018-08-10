#include"stdafx.h"
#include"ClientSocket.h"
#include<stdio.h>
#include<string>
#pragma comment(lib,"Ws2_32")
#define SERVER_PORT 6000
#define SERVER_ADDR "127.0.0.1"
using namespace std;
string nguoidung;

int trangthaidangnhap = 0;
void Request(char *a) {
	//khoi tao winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2); // phien ban ho tro 2.2
	if (WSAStartup(wVersion, &wsaData) != 0) {
		cout << "Version is not Support" << endl;
		return;
	}
	// khoi tao socket
	SOCKET client;
	//AF_INET dung dia chi ip de truyen du lieu
	//SOCK_STREAM su dung TCP
	//IPPROTO_TCP di theo cap vs SOCK_STREAM
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//time-out cho viec giao tiep
	int tv = 10000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char *)(&tv), sizeof(int));
	// chi dinh dia chi server
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT); //cong
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);//dia chi server
	//Request de connect toi server
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		cout << "ERROR! Cannot connect server.";
		_getch();
		return;
	}

	int ret;
	//send de gui thong diep kieu char * cho server
	char buff[2048];

	switch (atoi(a))
	{
	case 1: {
		if (trangthaidangnhap == 1) {
			system("cls");
			cout << "-----------------------" << endl;
			cout << "Ban da dang nhap roi" << endl;
			cout << "-----------------------" << endl;
			break;
		}
		string taikhoan;
		string matkhau;
		cout << "Nhap tai khoan:";
		cin >> taikhoan;
		cin.ignore();
		cout << "Nhap mat khau";
		cin >> matkhau;
		string s = "";
		s = s + (string)a + "," + taikhoan + "," + matkhau;
		ret = send(client, s.c_str(), strlen(s.c_str()), 0);
		ret = recv(client, buff, 2048, 0);
		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT)
				cout << "Time-out!";
			else cout << ("ERROR! Cannot receive message.") << endl;
			return;

		}
		buff[ret] = 0;
		system("cls");

		if (atoi(buff) == 1) {
			nguoidung = taikhoan;
			cout << "-----------------------" << endl;
			cout << "Dang nhap thanh cong" << endl;
			cout << "-----------------------" << endl;

			trangthaidangnhap = 1;
			break;

		}
		if (atoi(buff) == 0) {
			cout << "Xay ra loi";
			break;
		}
		if (atoi(buff) == -1) {
			cout << "-----------------------" << endl;
			cout << "Sai ten dang nhap hoac mat khau" << endl;
			cout << "-----------------------" << endl;
		}
		break;
	}
	case 2: {
		string taikhoan;
		string matkhau;
		string hoten;
		string diachi;
		cout << "Nhap tai khoan can dang ky:";
		cin >> taikhoan;
		cout << "Nhap mat khau can dang ky:";
		cin >> matkhau;
		cin.ignore();
		cout << "Nhap ho ten:";
		getline(cin, hoten);
		cout << "Nhap dia chi:";
		getline(cin, diachi);
		string s = "";
		s = s +(string)a+","+ taikhoan + "," + matkhau + "," + hoten + "," + diachi;
		
		ret = send(client, s.c_str(), strlen(s.c_str()), 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		ret = recv(client, buff, 2048, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		buff[ret] = 0;
		system("cls");
	
		if (atoi(buff) == 0) {
			cout << "Error!" << endl;
			break;
		}
		else if (atoi(buff) == 1) {
			cout << "-----------------------" << endl;
			cout << "Dang ky thanh cong" << endl;
			cout << "-----------------------" << endl;
			break;
		}
		break;
	}
	case 3: {
		system("cls");
		trangthaidangnhap = 0;
		nguoidung = "";
		cout << "---------------------------" << endl;
		cout << "Ban da dang xuat thanh cong"<<endl;
		cout << "---------------------------" << endl;
		break;
	}
	case 4: {
		system("cls");
		if (trangthaidangnhap == 0) {
			cout << "-----------------------" << endl;
			cout << "Ban chua dang nhap " << endl;
			cout << "-----------------------" << endl;
			break;
		}
		string taikhoan;
		cout << "Nhap chinh xac tai khoan can ADDFRIEND:";
		cin >> taikhoan;
		string s = "";
		s = s + (string)a + "," + taikhoan+","+nguoidung;

		ret = send(client, s.c_str(), strlen(s.c_str()), 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		ret = recv(client, buff, 2048, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		buff[ret] = 0;
		system("cls");
		if (atoi(buff) == 0) {
			cout << "Error!";
			break;
		}
		if (atoi(buff) == -1) {
			cout << "-----------------------" << endl;
			cout << "Nguoi dung can AddFriend nay ko ton tai " << endl;
			cout << "-----------------------" << endl;
			break;
		}

		cout << "-----------------------" << endl;
		cout << "Addfriend thanh cong " << endl;
		cout << "-----------------------" << endl;
		break;
	}
	case 5: {
		system("cls");
		if (trangthaidangnhap == 0) {
			cout << "-----------------------" << endl;
			cout << "Ban chua dang nhap " << endl;
			cout << "-----------------------" << endl;
			break;
		}
		string taikhoan;
		cout << "Nhap chinh xac tai khoan can BLOCK:";
		cin >> taikhoan;
		string s = "";
		s = s + (string)a + "," + taikhoan+","+nguoidung;

		ret = send(client, s.c_str(), strlen(s.c_str()), 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		ret = recv(client, buff, 2048, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		buff[ret] = 0;
		system("cls");

		if (atoi(buff) == 0) {
			cout << "Error!";
			break;
		}
		if (atoi(buff) == -1) {
			cout << "-----------------------" << endl;
			cout << "Nguoi dung can Block nay ko ton tai " << endl;
			cout << "-----------------------" << endl;
			break;
		}
		cout << "-----------------------" << endl;
		cout << "Block thanh cong " << endl;
		cout << "-----------------------" << endl;
		break;
	}
	case 6: {
		system("cls");
		if (trangthaidangnhap == 0) {
			cout << "------------------------" << endl;
			cout << "Ban chua dang nhap" << endl;
			cout << "------------------------" << endl;
			break;
		}
		string taikhoan;
		
		string noidung;
		
		string s = "";
		cout << "Nhap tai khoan can gui tin nhan:";
		cin >> taikhoan;
		cout << "Nhap noi dung tin nhan can gui:";
		cin.ignore();
		getline(cin, noidung);
		s = s + (string)a + "," + taikhoan+","+noidung+","+nguoidung;
		
		
		ret = send(client, s.c_str(), strlen(s.c_str()), 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		ret = recv(client, buff, 2048, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error!" << WSAGetLastError();
			return;
		}
		buff[ret] = 0;
		system("cls");
		
		if (atoi(buff) == 0) {
			cout << "Error!";
			break;
		}
		if (atoi(buff) == -1) {
			cout << "------------------------------------------" << endl;
			cout << "Nguoi dung can gui tin nhan nay ko ton tai " << endl;
			cout << "------------------------------------------" << endl;
			break;
		}
		if (atoi(buff) == -2) {
			cout << "------------------------------------------" << endl;
			cout << "Khong the gui tin nhan cho tai khoan block " << endl;
			cout << "------------------------------------------" << endl;
			break;
		}
		cout << "------------------------" << endl;
		cout << "Gui tin nhan thanh cong" << endl;
		cout << "------------------------" << endl;
		
		break;
	}
	case 7: {
		system("cls");
		if (trangthaidangnhap == 0) {
			cout << "------------------------" << endl;
			cout << "Ban chua dang nhap" << endl;
			cout << "------------------------" << endl;
			break;
		}
	}
	}

	//close socket
	closesocket(client);
	WSACleanup();
	
}