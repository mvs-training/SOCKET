#pragma once
#include"stdafx.h"
void ServerSock();

void ConnectDB();
void DisConnectDB();
string DangKyTK(string,string,string,string);
string DangNhap(string ,string);
void DangXuat();
void SuaTTBanBe();
string AddFriend(string taikhoan,string nguoidung);
string Block(string,string);
void DanhSachBanbe();
string GuiTinNhan1(string,string,string);
int GuiTinNhan2();
int XemCacTinNhanDaGui();
int XemTinNhanDen();
void ReplyTinNhan();
void ThongTinBanBe();
void DanhSachBanBeTime();
void DanhSachBanBeCity();
void setNguoidung(string);
string getNguoidung();
void Error();
