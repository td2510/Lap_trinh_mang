#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int a;
    sscanf(argv[1], "%d", &a);
    char file1[256], file2[256];
    sprintf(file1, "%s", argv[2]);
    sprintf(file2, "%s", argv[3]);

    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(a);

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
    printf("yo\n");
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr);
    
    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    // if (client != -1)
    // {
    //     FILE *f1 = fopen(file1,"a");
    //     fprintf(f1, "Hello client\n");
    //     fclose(f1);
    // }
    printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    char *string = (char*)malloc(sizeof(char) * 100);
    FILE *f1 = fopen(file1,"rt");    
    while(!feof(f1)){
        fgets(string,100,f1); //Lệnh đọc xâu
    }
    fclose(f1);

    send(client, string, strlen(string), 0);
    free(string);

    char buf[256];

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        if (ret < sizeof(buf))
            buf[ret] = 0;

        printf("%d bytes received\n", ret);
        printf("%s\n", buf);

        FILE *f2 = fopen(file2,"a");
        if (strncmp(buf, "exit", 4))
        {
            fprintf(f2, "%s", buf);
        }
        fclose(f2);
    }
    
    close(client);
    close(listener);
}