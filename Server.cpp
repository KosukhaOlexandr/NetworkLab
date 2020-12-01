// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#define PORT 8800
#include <fstream>
#include <iostream>

#include <string>

const int MaxCharLength = 1023;// для більшої довжини потрібно додати серіалізацію даних

using namespace std;

int main(int argc, char const *argv[])
{
    ifstream fin ("put.txt");
    string arr[1000];
    int arrlen = 0;
    while (!fin.eof())
    {
        fin >> arr[arrlen++];
    }
    fin.close();
    ifstream ch_in("change.txt");
    string change[1000];
    int changelen = 0;
    while (!ch_in.eof())
    {
        ch_in >> change[changelen++];
    }



    WSADATA WSA_Socket;
    SOCKET server_fd;

    if ( WSAStartup(MAKEWORD(2,2), &WSA_Socket)!=0)
    {
        printf("\nSocket_Server()> open WSAStartup is invalid!");
        return -1;
    }

    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        perror("socket failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    //   u_long on = 1;
    //ioctlsocket(server_fd,FIONBIO,&on);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) == SOCKET_ERROR)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    SOCKET new_socket;

    while(true) {
        if ((new_socket = accept(server_fd, nullptr,
                                 nullptr)) == SOCKET_ERROR)
        {
            cout << WSAGetLastError() << '\n';
            perror("accept");
            exit(EXIT_FAILURE);
        }
        int flag = 1;
        while (flag) {
            valread = recv(new_socket, buffer, 1023, 0);
            if (valread > 0) {
                char ch[15] = "check_change";
                if (strcmp(buffer, ch) == 0) {
                    memset(buffer, 0, sizeof(*buffer));
                    int k = 0;
                    int s = 0;
                    string notificate = ""; // довжина не більше 1024 символів
                    while (s != max(arrlen, changelen)) {
                        if (arr[s] != change[s]) {
                            arr[s] = change[s];
                            notificate += "<" + to_string(s) + "; " + change[s] + "> ";
                            k++;
                        }
                        s++;
                    }
                    char nochange[25] = "There are no changes";
                    string sf = "Finish";

                    if (k == 0) {
                        send(new_socket, nochange, strlen(nochange), 0);
                        cout << nochange << '\n';
                    } else {
                        notificate += sf;
                        char *pchar = new char[MaxCharLength + 1];
                        strcpy(pchar, notificate.c_str());
                        send(new_socket, pchar, strlen(pchar), 0);
                        cout << notificate << '\n';
                    }
                }
                char ex[15] = "exitk_change";// Буфер чомусь не очищується
                if (strcmp(buffer, ex) == 0) {
                    memset(buffer, 0, sizeof(*buffer));
                    printf("Exit\n");
                    break;
                }
            } else
                flag = 0;
        }
    }
    closesocket(server_fd);
    closesocket(new_socket);
    fin.close();
    ch_in.close();
    WSACleanup();
    system("pause");

    return 0;
}
