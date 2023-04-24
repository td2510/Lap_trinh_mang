#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
// Server nhan file tu client

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    char buf[20];

    int ret, count = 0;
    char *search = "0123456789";
    bool check[9];

    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        // printf("%d\n", (int)buf[19] - 48);

        for (int i = 0; i < 9; i++)
        {
            if (check[i])
            {
                bool tmp;
                for (int j = 0; j < 9 - i; j++)
                {
                    if (((int)buf[j] - 48) == (i + 1 + j))
                        tmp = true;
                    else
                    {
                        tmp = false;
                        break;
                    }
                }
                if (tmp)
                    count++;
            }
        }

        for (int i = 0; i < strlen(buf); i++)
        {
            if (strstr(&buf[i], search) == &buf[i])
            {
                count++;
            }
        }

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                if (((int)buf[19 - j] - 48) == (i - j))
                    check[i] = true;
                else
                {
                    check[i] = false;
                    break;
                }
            }
        }
    }

    printf("So lan xuat hien xau 0123456789 : %d\n", count);
    
    close(client);
    close(listener);
}