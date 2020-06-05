//
//  tcp_echo_cli.c
//  NetworkLab1
//
//  Created by una on 2020/5/29.
//  Copyright © 2020 cetacis. All rights reserved.
//

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <signal.h>
# include <errno.h>
# define MAX_CMD_STR 100
int echo_rqt(int ClientSocket) {
    char buf[MAX_CMD_STR + 1];
    int res;
   while(fgets(buf, MAX_CMD_STR, stdin) {
        if (strncmp(buf, "exit", 4) == 0) {
            return 0;
        }

        int len = (int )strlen(buf);
        buf[len - 1] = '\0';
        write(ClientSocket, &len, 4);
        char *p = buf;
        int le = len;
        for (;;) {
            res = write(ClientSocket, p, le);
            if (res == 0) break;
            if (res == -1) {
                return 0;
            }
            le -= res;
            p += res;
        }
        res = read(ClientSocket, &len, 4);
        if (res < 0) {
            puts("read error!");
            return 0;
        }
        p = buf;
        le = len;
        while(1) {
            res = read(ClientSocket, p, le);
            if (res == 0) break;
            if (res == -1) {
                return 0;
            }
            le -= res;
            p += res;
        }
        printf("[echo_rep] %s\n", buf);
    }
    return 0;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage:%s <IP><RORT>\n",argv[0]);
        return 0;
    }
    //1. creat socket
    int res;
    struct sockaddr_in srv_addr;
    srv_addr.sin_family=AF_INET;
    srv_addr.sin_port=htons(atoi(argv[2]));
    srv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    int connfd = socket(PF_INET, SOCK_STREAM, 0);
    if (connfd<0) {
        printf("creat cli_socket error");
        return 1;
    }
    //2. connect
    do{
        res = connect(connfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
        if (res == 0){
            printf("[cli] server[%s:%s] is connected!\n", argv[1], argv[2]);
            if(echo_rqt(connfd)==0)
                break;
        }else if(res == -1 && errno == EINTR){
            continue;
        }
    }while (1);
    close(connfd);
    printf("[cli] connfd is closed!\n");
    printf("[cli] client is exiting!\n");
    return 0;
}
