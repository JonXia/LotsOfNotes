////����ģ��
//#include <string>
//#include <cstdio>
//#include <cstdlib>
//#include <iostream>
//#include <WinSock2.h>
//#include <Windows.h>
////����lib
//#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
////ʹ�������ռ�
//using namespace std;
//
////��ʼ��dll
//WSADATA wsaData;
//WSAStartup(MAKEWORD(2, 2), &wsaData);
////           ��                    ��
////    WinSock�淶�İ汾��   ָ��WSAData��ָ��
//
////�����׽���
//SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
////                      ��         ��                  ��
////              IP��ַ���� �׽������ͣ����ݴ��䷽ʽ�� ����Э��
////                 IPv4       ����ʽ�׽���          TCP����Э��
////ע��sock����cilent.cpp�еģ���server.cpp����servSock��ֻ�Ǳ�������ͬ���������죩
//
//
