#include"stdafx.h"
#include"ServerRequest.h"
#include<vector>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#define SERVER_PORT 6000
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib,"Ws2_32")
int rc;
char *error;
char **results = NULL;
int rows, columns;
sqlite3 *db;
string nguoidung;

void ServerSock() {
	//khoi tao winsock

	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &wsaData);
	//khoi tao socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		cout << "ERROR! Cannot bend this address." << endl;
		return;
	}
	if (listen(listenSock, 10)) {
		cout << "Error, Canot listen.";
		return;
	}
	sockaddr_in clientAddr;
	char buff[BUFF_SIZE];
	int ret, clientAddrlen = sizeof(clientAddr);
	int index = 0;
	while (1) {
		 index = 0;
		SOCKET connSock;
		connSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrlen);
		ret = recv(connSock, buff, BUFF_SIZE, 0);

		if (ret == SOCKET_ERROR) {
			cout << "Error:" << WSAGetLastError();
			break;
		}
		else {
			buff[ret] = 0;
			char *p;
			char **b = (char **)malloc(1000 * sizeof(char));;
			p = strtok(buff, ","); //cat chuoi bang cac ky tu  ,
			while (p != NULL)
			{
				b[index] = p;
				index++;
				p = strtok(NULL, ","); //cat chuoi tu vi tri dung lai truoc do
			}
			switch (atoi(b[0]))
			{
			case 1: {
				ret = send(connSock, DangNhap(string(b[1]), string(b[2])).c_str(), 1, 0);
				if (ret == SOCKET_ERROR)
					cout << "Error:" << WSAGetLastError();


				break;
			}
			case 2: {
				ret = send(connSock, DangKyTK(string(b[1]), string(b[2]), string(b[3]), string(b[4])).c_str(), 1, 0);
				if (ret == SOCKET_ERROR)
					cout << "Error:" << WSAGetLastError();



				break;
			}
			case 4: {
				ret = send(connSock, AddFriend(string(b[1]), string(b[2])).c_str(), 1, 0);
				if (ret == SOCKET_ERROR)
					cout << "Error:" << WSAGetLastError();
				break;
			}
			case 5: {
				ret = send(connSock, Block(string(b[1]),string(b[2])).c_str(), 1, 0);
				if (ret == SOCKET_ERROR)
					cout << "Error:" << WSAGetLastError();
				break;
			}
			case 6: {
				ret = send(connSock, GuiTinNhan1(string(b[1]),string(b[2]),string(b[3])).c_str(), 1, 0);
				if (ret == SOCKET_ERROR)
					cout << "Error:" << WSAGetLastError();
				
				break;
			}
			default:
				break;
			}
			
		}
		shutdown(connSock, SD_SEND);
		closesocket(connSock);
	}
	closesocket(listenSock);
	WSACleanup();
}
void ConnectDB() {
	rc = sqlite3_open("MyDb.db", &db);
}
void DisConnectDB() {
	sqlite3_close(db);
}
void setNguoidung(string s) {
	nguoidung = s;
}
string getNguoidung() {
	return nguoidung;
}
 string DangNhap(string taikhoan,string matkhau) {
	
	string sqlSelect = "SELECT taiKhoan FROM Nguoidung WHERE taiKhoan=";
	sqlSelect = sqlSelect + "'" + taikhoan + "'" + " AND matKhau='" + matkhau + "';";
	ConnectDB();

	rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);

	if (rc)
	{	
		
		DisConnectDB();
		return "0";
	}
	if (rows == 1) {
		setNguoidung(taikhoan);
		DisConnectDB();
		
		return "1";

	}
	DisConnectDB();
	return "-1";
}
string DangKyTK(string taikhoan, string matkhau, string hoten, string diachi) {

	ConnectDB();
	string sqlInsert = "INSERT INTO Nguoidung VALUES('" + taikhoan + "','" + matkhau + "','" + hoten + "','" + diachi + "');";
	rc = sqlite3_get_table(db, sqlInsert.c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		DisConnectDB();
		return "0";
	}

	DisConnectDB();
	return "1";
}

string AddFriend(string taikhoan,string nguoidung) {
	
	ConnectDB();
	//KIEM TRA XEM TAI KHOAN CAN ADDFRIEND CO TON TAI KO
	string sql = "SELECT taiKhoan FROM Nguoidung WHERE taiKhoan='" + taikhoan + "';";
	rc = sqlite3_get_table(db, sql.c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		return "0";
	}
	else if (rows == 0) {
		return "-1";
	}
	//NEU TAI KHOAN CAN ADDFRIEND TON TAI THI TIEN HANH LAM CAC BUOC DUOI
	else if (rows == 1) {
		//KIEM TRA XEM TAI KHOAN CAN ADDFRIEND DA CO TRONG BANG TRANG THAI VOI TAI KHOAN DANG NHAP HAY CHUA NEU CO THI TIEN HANH UPDATE THANH BAN BE
		string sqlSelect = "SELECT nguoiDung1,nguoiDung2 FROM Trangthai WHERE nguoiDung1='" + nguoidung + "' AND nguoiDung2='" + taikhoan + "';";
		rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
		//NEU 2 TAI KHOAN NAY DA CO TRONG 1 HANG CUA BANG TRANG THAI THI TIEN HANH UPDATE
		if (rows == 1) {
			string sqlUpdate1 = "UPDATE Trangthai SET trangThai='ban be' WHERE nguoiDung1='" + nguoidung + "' AND nguoiDung2='" + taikhoan + "';";
			rc = sqlite3_exec(db, sqlUpdate1.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}
			string sqlUpdate2 = "UPDATE Trangthai SET trangThai='ban be' WHERE nguoiDung2='" + nguoidung + "' AND nguoiDung1='" + taikhoan + "';";
			rc = sqlite3_exec(db, sqlUpdate2.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}
			return "1";
		}
		//NEU 2 TAI KHOAN NAY CHUA CO TRONG BANG TRANGTHAI THI TIEN HANH INSERT VOI TRANG THAI LA BAN BE
		else if (rows == 0) {
			string sqlInsert1 = "INSERT INTO Trangthai VALUES('" + nguoidung + "','" + taikhoan + "','ban be');";
			rc = sqlite3_exec(db, sqlInsert1.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}
			string sqlInsert2 = "INSERT INTO Trangthai VALUES('" + taikhoan + "','" + nguoidung + "','ban be');";
			rc = sqlite3_exec(db, sqlInsert2.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}

		}

	}
	DisConnectDB();
	return "1";
}
string Block(string taikhoan,string nguoidung) {
	

	ConnectDB();
	//KIEM TRA XEM TAI KHOAN CAN BLOCK CO TON TAI KO
	string sql = "SELECT taiKhoan FROM Nguoidung WHERE taiKhoan='" + taikhoan + "';";
	rc = sqlite3_get_table(db, sql.c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		return "0";
	}
	else if (rows == 0) {
		return "-1";
	}
	//NEU TAI KHOAN CAN BLOCK TON TAI THI TIEN HANH LAM CAC BUOC DUOI
	else if (rows == 1) {
		//KIEM TRA XEM TAI KHOAN CAN BLOCK DA CO TRONG BANG TRANG THAI VOI TAI KHOAN DANG NHAP HAY CHUA NEU CO THI TIEN HANH UPDATE THANH BAN BE
		string sqlSelect = "SELECT nguoiDung1,nguoiDung2 FROM Trangthai WHERE nguoiDung1='" + nguoidung + "' AND nguoiDung2='" + taikhoan + "';";
		rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
		//NEU 2 TAI KHOAN NAY DA CO TRONG 1 HANG CUA BANG TRANG THAI THI TIEN HANH UPDATE
		if (rows == 1) {
			string sqlUpdate1 = "UPDATE Trangthai SET trangThai='block' WHERE nguoiDung1='" + nguoidung + "' AND nguoiDung2='" + taikhoan + "';";
			rc = sqlite3_exec(db, sqlUpdate1.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}
			string sqlUpdate2 = "UPDATE Trangthai SET trangThai='block' WHERE nguoiDung2='" + nguoidung + "' AND nguoiDung1='" + (string)taikhoan + "';";
			rc = sqlite3_exec(db, sqlUpdate2.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}
			return "1";
		}
		//NEU 2 TAI KHOAN NAY CHUA CO TRONG BANG TRANGTHAI THI TIEN HANH INSERT VOI TRANG THAI LA BAN BE
		else if (rows == 0) {
			string sqlInsert1 = "INSERT INTO Trangthai VALUES('" + nguoidung + "','" + taikhoan + "','block');";
			rc = sqlite3_exec(db, sqlInsert1.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}
			string sqlInsert2 = "INSERT INTO Trangthai VALUES('" + (string)taikhoan + "','" + nguoidung + "','block');";
			rc = sqlite3_exec(db, sqlInsert2.c_str(), NULL, NULL, &error);
			if (rc) {
				return "0";
			}

		}

	}
	DisConnectDB();
	return "1";
}
string GuiTinNhan1(string taikhoan,string noidung,string nguoidung) {
	ConnectDB();
	//KIEM TRA XEM TAI KHOAN CAN GUI TIN NHAN CO TON TAI KO
	string sql = "SELECT taiKhoan FROM Nguoidung WHERE taiKhoan='" + taikhoan + "';";
	rc = sqlite3_get_table(db, sql.c_str(), &results, &rows, &columns, &error);
	if (rc)
	{
		DisConnectDB();
		return "0";
	}
	else if (rows == 0) {
		DisConnectDB();
		return "-1";
	}
	//kiem tra xem tai khoan co bi block hay ko, neu bi block thi ko the gui tin nhan
	string sqlSelect = "SELECT * FROM Trangthai WHERE nguoiDung1='" + nguoidung + "' AND nguoiDung2='" + taikhoan + "' AND trangThai='block';";
	rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
	if (rc) {
		DisConnectDB();
		return "0";

	}
	if (rows == 1) {
		DisConnectDB();
		return "-2";
	}
	

	
	string sqlMess = "INSERT INTO Messenger VALUES('" + nguoidung + "','" + taikhoan + "','" + noidung + "','chua doc');";

	rc = sqlite3_exec(db, sqlMess.c_str(), NULL, NULL, &error);
	if (rc) {
		DisConnectDB();
		return "0";
	}
	return "1";
	DisConnectDB();
}
int XemCacTinNhanDaGui() {
	ConnectDB();
	string sqlSelect = "SELECT * FROM MESSENGER WHERE nguoiDung1='" + nguoidung + "';";
	rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
	if (rc) {
		DisConnectDB();
		return 0;
	}
	else
	{
		// Display Table

		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				if (colCtr == 0) {
					cout << "Nguoi gui:";
				}
				if (colCtr == 1) {
					cout << "Nguoi nhan:";
				}
				if (colCtr == 2) {
					cout << "Noi dung tin nhan:";
				}
				if (colCtr == 3) {
					cout << "Trang thai tin nhan:";
				}
				cout << results[cellPosition] << endl;
			}

			// End Line
			cout << endl;


		}
		DisConnectDB();
	}
	return 1;
}
int GuiTinNhan2() {
	ConnectDB();
	cout << "Bam Ctrl +L de chon nguoi muon gui tin nhan:" << endl;
	char c = getch();
	if (c == 12) {
		string sqlSelect = "SELECT nguoiDung2 FROM Trangthai WHERE nguoiDung1='" + getNguoidung() + "' AND trangThai='ban be';";
		rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
		if (rc) {
			DisConnectDB();
			return 0;
		}
		if (rows == 0) {
			cout << "Khong co ban be nao" << endl;
			return -1;
		}
		cout << "Danh sach ban be:" << endl;
		int cellPosition;
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				cellPosition = (rowCtr * columns) + colCtr;
				cout << cellPosition << ". " << results[cellPosition] << endl;

			}

			// End Line
			cout << endl;


		}
		cout << "Chon so thu tu cua nguoi can gui tin nhan:";
		int chon;
		while (true) {
			cin >> chon;
			if (chon >= 1 && chon <= cellPosition) break;
		}
		cout << "TO: " << (string)results[chon] << endl;
		string taikhoan = (string)results[chon];
		string noidung;
		cout << "Nhap noi dung tin nhan:";
		cin.ignore();
		getline(cin, noidung);
		string sqlMess = "INSERT INTO Messenger VALUES('" + getNguoidung() + "','" + taikhoan + "','" + noidung + "','chua doc');";

		rc = sqlite3_exec(db, sqlMess.c_str(), NULL, NULL, &error);
		if (rc) {
			DisConnectDB();
			return 0;
		}
		return 1;
	}
	DisConnectDB();
	return -1;
}
int XemTinNhanDen() {
	ConnectDB();
	string sqlSelect = "SELECT * FROM MESSENGER WHERE nguoiDung2='" + getNguoidung() + "';";
	rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
	if (rc) {
		DisConnectDB();
		return 0;
	}
	else
	{
		// Display Table

		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);
				if (colCtr == 0) {
					cout << "Nguoi gui:";
				}
				if (colCtr == 1) {
					cout << "Nguoi nhan:";
				}
				if (colCtr == 2) {
					cout << "Noi dung tin nhan:";
				}
				if (colCtr == 3) {
					cout << "Trang thai tin nhan:";
				}
				cout << results[cellPosition] << endl;
			}

			// End Line
			cout << endl;


		}
		// update lai bang messsenger vi cac tin nhac deu da duoc doc
		string sqlUpdate = "UPDATE Messenger SET Trangthai='da doc' WHERE nguoiDung2='" + getNguoidung() + "';";
		rc = sqlite3_exec(db, sqlUpdate.c_str(), NULL, NULL, &error);
		if (rc) {
			DisConnectDB();
			return 0;
		}

	}
	DisConnectDB();
	return 1;
}
void DanhSachBanbe() {
	//Danh sach ban be mac dinh
	string sql = "SELECT nguoiDung2 FROM Trangthai WHERE nguoiDung1='" + getNguoidung() + "' AND " + "trangThai='ban be';";
	ConnectDB();
	rc = sqlite3_get_table(db, sql.c_str(), &results, &rows, &columns, &error);
	if (rc) {
		cout << "Loi ko lay duoc data: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		cout << "Danh sach ban be:" << endl;
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				cout.width(12);
				cout.setf(ios::left);

				cout << results[cellPosition] << endl;
			}

			// End Line
			cout << endl;


		}
	}
	cout << "An ctrl+C de xap xep lai danh sach ban be:" << endl;
	char c = getch();
	if (c == 3) {
		string sqlSelect = "SELECT DISTINCT  thanhPho FROM Nguoidung;";

		rc = sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
		vector<string> thanhpho;
		if (rc) {
			cout << "Loi: " << sqlite3_errmsg(db) << endl << endl;
			sqlite3_free(error);
		}
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			thanhpho.push_back(results[rowCtr]);
		}
		for (int i = 0; i < thanhpho.size(); i++) {
			string sqlSelectND = "SELECT Trangthai.nguoiDung2 FROM Nguoidung,Trangthai WHERE Trangthai.nguoiDung1='" + getNguoidung() + "' AND Trangthai.nguoiDung2=Nguoidung.taiKhoan AND Nguoidung.thanhPho='" + thanhpho[i] + "' AND Trangthai.trangThai='ban be';";
			rc = sqlite3_get_table(db, sqlSelectND.c_str(), &results, &rows, &columns, &error);
			if (rc) {
				cout << "Loi: " << sqlite3_errmsg(db) << endl << endl;
				sqlite3_free(error);
			}
			if (rows > 0) {
				cout << thanhpho[i] << endl;
			}
			for (int rowCtr = 1; rowCtr <= rows; ++rowCtr) {
				cout << rowCtr << "." << results[rowCtr] << endl;
			}

		}

	}

}

void Error() {
	cout << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
	sqlite3_free(error);
}
