////导入模块
//#include <string>
//#include <cstdio>
//#include <cstdlib>
//#include <iostream>
//#include <WinSock2.h>
//#include <Windows.h>
////加载lib
//#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
////使用命名空间
//using namespace std;
//
////初始化dll
//WSADATA wsaData;
//WSAStartup(MAKEWORD(2, 2), &wsaData);
////           ↑                    ↑
////    WinSock规范的版本号   指向WSAData的指针
//
////创建套接字
//SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
////                      ↑         ↑                  ↑
////              IP地址类型 套接字类型（数据传输方式） 传输协议
////                 IPv4       流格式套接字          TCP传输协议
////注：sock是在cilent.cpp中的，在server.cpp中是servSock（只是变量名不同，其他无异）
//
//
