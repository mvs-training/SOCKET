// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{	
	char a[10];
	char b[10];
	int select;
	do {
		cout << "chon cac chuc nang sau:" << endl;
		cout << "1.Dang nhap" << endl;
		cout << "2.Dang ky" << endl;
		cout << "3.Dang xuat" << endl;
		cout << "4.Add Friend" << endl;
		cout << "5.Block" << endl;
		cout << "6.Gui tin nhan theo cach nhap ten Tai Khoan" << endl;
		cout << "7.Gui tin nhan bang to hop phim Ctrl+L" << endl;
		cout << "8.Xem tin nhan da gui" << endl;
		cout << "9.Xem tin nhan den" << endl;
		cout << "10.Xem danh sach ban be" << endl;

		cout << "11.Exit" << endl;
		cout << "Ban chon chuc nang:";
		cin >> a;
		a[2] = 0;
		strcpy(b, a);
		 select = atoi(a);
		 if (1 <= select && select <= 10)
			 Request(b);
	} while (select!=11);
    return 0;
}

