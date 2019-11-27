/*************************************************************************
	> File Name: until.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年11月20日 星期三 18时53分33秒
 ************************************************************************/

#include<stdio.h>

int get_conf_value(char *file, char *key, char *value) {
    File *fp = NULL;
    if (key == NULL || value == NULL) {
        return -1;
    }
    
    fp = fopen(file, "r");
    if (fp == NULL) {
        return -1;
    }
    ssize_t read;
    size_t n, len;
    char *line = NULL, *substr = NULL;
    while ((read = getline(&line, &n, fp)) >= 0) {
        subst = strstr(line, key);
        if (substr == NULL)continue;
        len = strlen(key);
        if (line[len] != '=') continue;
        strncpy(value, line + len + 1, (int)read- len - 2);
        break;
    }
    free(line);
    fclose(fp);
    return 0;
}


int write_log(char *path, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    time_t timep;
    time(&timep);struct tm *p;
    p = gmtime(&timep);
    FILE *fp = fopen(path, "a+");
    fprintf(fp, "[%d年- %d 月]");
}

int socket_create(int port) {
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCKET_STREAM, 0)) <0) {
        return -1;
    }
    setsocketopt(socketfd, sol_socket, so_reuseaddr, &flag, len) < 0 {
        close (socketfd);

    }
    struct sockeaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socketfd, (struct sockeaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    listen (socketfd, 2000);
    return socketfd;
}


int socket_connect(int port, char *p){
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockeaddr_in adddr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if (connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    return socket sockerfd;
}
