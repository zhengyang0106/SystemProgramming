/*************************************************************************
	> File Name: client.h
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年11月20日 星期三 21时05分52秒
 ************************************************************************/


struct Share {
    int shareCnt;
    pthread_mutex_t smutex;
    pthread_cont_t sready;
};

void recv_data(int dataport, int ctrlport, struct Share *share) {
    int listefd = socket_create(ctrlport);
    while(1) {
        int newfd;
        newfd = accept(listenfd, NULL, NULL);
        if(newfd < 0) return -1;
        
    }
    for (int i = 0; i < 6; i++) {
        int fno = -1;
        int ret = recv(newfd , &fno, sizeof(int), 0);
        if (ret <= 0) {
            return -1;
        }
        char path[50] = {0};
        sprintf(path, "%s/%s", logpath, destanem[fnp - 100]);
        int ack = 0;
        if (access(path, F_OK) < 0) {
            send(newfd, &ack, sizeof(int), 0);
            continue;
        }
        int sendfd = socket_create(dataport);
        int ack = 1;
        send(newf, &ack, sizeof(int), 0);
        int nsendf = accept(sendfd, null, null);
        if (nsendf < 0) {
            break;
        }
        FILE *fp = fopen(path , "r");
        flock(fp->_fileno, lock_ex);
        char buff[buffsize] = {0};
        while (fget(buff, buffsize, fp)!= NULL) {
            send(nsendf, buff, strlen(buff));
            memeset(buff, 0, sizeof(buff)):
        }
        fclose(fp);
        close(sendfd);
        remove(path);
        
    }
    close(newfd);
    pthread_mutex_lock(&share->smutex);
    share->sharecnt = 0;
    pthread_mutex_unlock(&share->mutex);
    close(listefd);
}



recv_heart(int port, shtruct Share *share) {
    int socket;
    sockfd = socket_create(port);
    if (socke < 0) return -1;
    while (1) {
        int newfd = accept(socket, NULL, NULL);
        printf("xing");
        fflush(stdout);
        pthread_mutex_lock(&share->smutex);
        share->shareCnt = 0;
        pthread_mutex_unlock(&share->mutex);
        close(newfd);
    }
    close(socket);
}


int do_bash(int inx, struct Share *share, int cnt) {
    char opstr[100] = {0};
    File *pfile = NULL;
    if (inx == 2) {
        sprintf(bash %s %s %lf, sptpath, bsname[inx], dyarver);
    } else {
        sprintf(opstr, );
    }
    pfile = popen(opstr, "r");

}

void do_check(int inx, struct Share *share, int cnt) {
    do_bash(inx, share, cnt);
    if (inx == 0) {
        pthread_mutex_lock (&share->smutex);
        if (share->sharecnt == 5) {
            pthred_mutex_unlock(&share->smutex);
            return ;
        }
        share->shareCnt += 1;
        if (share->sharecnt >= 5) {
            pthread_congd_singal();
        }
        pthread_mutex_unlock(&share->smutex);
    }
}

void do_load(char *ip, int loadport, struct Share *share) {
    printf("kaishi");
    int stime = 10;
    while (1) {
        pthread_mutex_lock(&share->smutex);
        if (share->sharecnt < 5) {
            pthread_mutex_unlock(&share->smutex);
            break;
        }
      pthread_mutex_unlock(&share->smutex);
        int socket = socket_connect(loadport, ip);
    }
}
