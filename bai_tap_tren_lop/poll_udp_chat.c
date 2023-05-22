#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

int main(int argc, char *argv[])
{
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr2;
    addr2.sin_family = AF_INET;
    addr2.sin_addr.s_addr = inet_addr(argv[1]);
    addr2.sin_port = htons(atoi(argv[2]));

    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = htonl(INADDR_ANY);
    addr1.sin_port = htons(atoi(argv[3]));

    bind(receiver, (struct sockaddr *)&addr1, sizeof(addr1));

    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);

    struct pollfd fds[2];
    
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = receiver;
    fds[1].events = POLLIN;

    char buf[256];

    while (1)
    {
        int ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            perror("poll() failed");
            break;
        }

        if (ret == 0)
        {
            printf("Timed out.\n");
            continue;
        }

        if (fds[0].revents & POLLIN)
        {
            fgets(buf, sizeof(buf), stdin);
            sendto(sender, buf, strlen(buf), 0,(struct sockaddr *)&addr2, sizeof(addr2));
        }

        if (fds[1].revents & POLLIN)
        {
            //ret = recv(receiver, buf, sizeof(buf), 0);
            ret = recvfrom(receiver, buf, sizeof(buf), 0,
            (struct sockaddr *)&sender_addr, &sender_addr_len);
            if (ret <= 0)
            {
                break;
            }
            buf[ret] = 0;
            printf("Received: %s\n", buf);
        }


    }

}