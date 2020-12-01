// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>

#define PORT 8800

using namespace std;

int main(int argc, char const *argv[])
{
    WSADATA WSA_Socket;
    int iResult = WSAStartup(MAKEWORD(2,2), &WSA_Socket);
    if (iResult!=0)
    {
        printf("\nSocket_Server()> open WSAStartup is invalid!");
        return -1;
    }
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }



    ifstream fin ("put.txt");
    string arr[1000];
    int arrlen = 0;
    while (!fin.eof())
    {
        fin >> arr[arrlen++];
    }
    while (true)
    {
        char hi[1024] = {0};
        char ex[15] = "exit";
        string s;
        cin >> s;
        if (s == "update")
        {
            char checkm[15] = "check_change";
            strcpy(hi, checkm);
            send(sock, hi, strlen(hi), 0);

            char nochange[25] = "There are no changes";
            valread = recv(sock, buffer, 1024, 0);
            int i = 0;
            while (i < valread)
            {
                if (buffer[i] == '<') {
                    i++;
                    int num = 0;
                    while (buffer[i] != ';') {
                        num = num * 10 + ((int) buffer[i] - 48);
                        i++;
                    }
                    if (num >= arrlen)
                        arrlen = num + 1;
                    i++;
                    string newstr = "";
                    while (buffer[i] != '>') {
                        newstr += buffer[i];
                        i++;
                    }
                    arr[num] = newstr;

                }
                if (buffer[i] == 'F') {
                    break;
                }
                i++;
            }
            printf("%s\n", buffer);
            memset(buffer, 0, sizeof(*buffer));

        }
        if (s == "exit")
        {

            send(sock, ex, strlen(ex), 0);
            cout << "Finish" << '\n';
            break;
        }

    }
    system("pause");
    return 0;
}
