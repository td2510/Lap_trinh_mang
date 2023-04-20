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
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    char *serverIP = argv[1];
    int portNum = atoi(argv[2]);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverIP);
    addr.sin_port = htons(portNum);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    char buf[256];
    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        printf("Connection closed\n");
        return 1;
    }

    if (ret < sizeof(buf))
        buf[ret] = 0;
    printf("%d bytes received: %s\n", ret, buf);

    int mssv;
    printf("Nhap thong tin sinh vien : \n");
    printf("MSSV : "); scanf("%d", &mssv);
    char hoTen[50], ngaySinh[20], tmp[5];
    fgets(tmp, sizeof(tmp), stdin);
    // fflush(stdin);
    printf("Ho ten : ");
    fgets(hoTen, sizeof(hoTen), stdin);
    hoTen[strlen(hoTen) - 1] = 0;
    // fflush(stdin);
    printf("Ngay sinh : ");
    fgets(ngaySinh, sizeof(ngaySinh), stdin);
    ngaySinh[strlen(ngaySinh) - 1] = 0;
    double diem;
    printf("Diem trung binh cac mon hoc : "); 
    scanf("%lf", &diem);

    char packet[1024];
    sprintf(packet, "%d %s %s %.2lf", mssv, hoTen, ngaySinh, diem);
    send(client, packet, strlen(packet), 0);

    // while (1)
    // {
    //     printf("Enter string:");
    //     fgets(buf, sizeof(buf), stdin);

    //     send(client, buf, strlen(buf), 0);

    //     if (strncmp(buf, "exit", 4) == 0)
    //         break;
    // }

    close(client);    
}