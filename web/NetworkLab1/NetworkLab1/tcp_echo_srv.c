//
//  tcp_echo_srv.c
//  NetworkLab1
//
//  Created by una on 2020/5/29.
//  Copyright © 2020 cetacis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#define MAX_CMD_STR 100

int sig_to_exit = 0;
int sig_num = 0;

void sig_int(int signal) {
    sig_num = signal;
    sig_to_exit = 1;
    printf("[srv] SIGINT is coming!\n");
}

void sig_pipe(int signal) {
    sig_num = signal;
    printf("[srv] SIGPIPE is coming!\n");
}

void echo_rep(int sockfd) {
    int res;
    char *buf;
    int len;
    
    do{
        res = (int)read(sockfd, &len, 4);
        if(res < 0){
            printf("[srv] read len return %d and errno is %d\n", res, errno); if(errno == EINTR)
                continue;
            return;
        }
        if(res == 0) return;
        buf = (char *) malloc((len + 1) * sizeof(char ));
        do{
            
            res =(int)read(sockfd, buf, len);
            if(res < 0){
                printf("[srv] read data return %d and errno is %d\n", res, errno); if(errno == EINTR)
                    continue;
                free(buf);
                return;
            }
            break;
        }while(1);
        printf("[echo_rqt] %s\n", buf);
        res = (int)write(sockfd, &len, 4);
        if (res < 0) {
            printf("write error!");
            return;
        }
        res = (int)write(sockfd, buf, len);

        free(buf);

    }while(1);
}
void Init(){
    // SIGPIPE
    struct sigaction sigact_pipe, old_sigact_pipe;
    sigact_pipe.sa_handler = sig_pipe;
    sigemptyset(&sigact_pipe.sa_mask);
    sigact_pipe.sa_flags = 0;
    sigact_pipe.sa_flags |= SA_RESTART;//设置受影响调用立刻重启
    sigaction(SIGPIPE, &sigact_pipe, &old_sigact_pipe);
    // SIGINT
    struct sigaction sig_act_int, old_sig_act_int;
    sig_act_int.sa_handler = sig_int;
    sigemptyset(&sig_act_int.sa_mask);
    sig_act_int.sa_flags = 0;
    sigaction(SIGINT, &sig_act_int, &old_sig_act_int);
}
int main(int argc, char* argv[]){
    int res;
    Init();
    
    //1. socket
    int serverfd= socket(PF_INET, SOCK_STREAM, 0);
    if (serverfd < 0) {
        printf("Create Socket error!");
        return 0;
    }
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    srv_addr.sin_port = htons(atoi(argv[2]));
    printf("[srv] server[%s:%s] is initializing!\n", argv[1], argv[2]);

    //2. bind
    res =  bind(serverfd, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
    if (res < 0) {
        printf("Bind error!");
        return 0 ;
    }

    //3. lisen
    res = listen(serverfd, 8);
    if (res < 0) {
        printf("Listen error!");
        return 0;
    }
    
    struct sockaddr_in cli_addr;
    socklen_t add_size = sizeof(struct sockaddr_in);
    while (!sig_to_exit) {
        int connfd = accept(serverfd, (struct sockaddr*) &cli_addr, &add_size);
        if(connfd == -1 && errno == EINTR && sig_num == SIGINT) break;
        if (connfd < 0) continue;
        printf("[srv] client[%s:%d] is accepted!\n", inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));

        echo_rep(connfd);
        close(connfd);
        printf("[srv] connfd is closed!\n");


    }
    close(serverfd);
    printf("[srv] listenfd is closed!\n");
    printf("[srv] server is exiting\n");
    return 0;
}
